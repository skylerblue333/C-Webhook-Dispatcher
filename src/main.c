#include "webhook_plan.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static int is_https_url(const char *url) {
    const char prefix[] = "https://";
    size_t prefix_len = sizeof(prefix) - 1U;
    size_t len;

    if (url == NULL) return 0;
    len = strnlen(url, SKY_WEBHOOK_URL_MAX + 2U);
    if (len <= prefix_len || len > SKY_WEBHOOK_URL_MAX) return 0;
    return strncmp(url, prefix, prefix_len) == 0;
}

static int is_valid_event(const char *event) {
    size_t len;
    size_t i;

    if (event == NULL) return 0;
    len = strnlen(event, SKY_WEBHOOK_EVENT_MAX + 2U);
    if (len == 0U || len > SKY_WEBHOOK_EVENT_MAX) return 0;

    for (i = 0U; i < len; ++i) {
        unsigned char c = (unsigned char)event[i];
        if (!(isalnum(c) || c == '.' || c == '_' || c == '-')) return 0;
    }
    return 1;
}

sky_webhook_result sky_webhook_plan_init(
    sky_webhook_plan *plan,
    const char *url,
    const char *event,
    const char *payload,
    unsigned int max_attempts,
    unsigned int base_delay_ms
) {
    size_t payload_len;

    if (plan == NULL) return SKY_WEBHOOK_INVALID_PAYLOAD;
    if (!is_https_url(url)) return SKY_WEBHOOK_INVALID_URL;
    if (!is_valid_event(event)) return SKY_WEBHOOK_INVALID_EVENT;
    if (payload == NULL) return SKY_WEBHOOK_INVALID_PAYLOAD;

    payload_len = strnlen(payload, SKY_WEBHOOK_PAYLOAD_MAX + 2U);
    if (payload_len == 0U || payload_len > SKY_WEBHOOK_PAYLOAD_MAX) {
        return SKY_WEBHOOK_INVALID_PAYLOAD;
    }
    if (max_attempts == 0U || max_attempts > SKY_WEBHOOK_MAX_ATTEMPTS ||
        base_delay_ms < 100U || base_delay_ms > 60000U) {
        return SKY_WEBHOOK_INVALID_RETRY_POLICY;
    }

    memset(plan, 0, sizeof(*plan));
    memcpy(plan->url, url, strlen(url) + 1U);
    memcpy(plan->event, event, strlen(event) + 1U);
    memcpy(plan->payload, payload, payload_len + 1U);
    plan->payload_len = payload_len;
    plan->max_attempts = max_attempts;
    plan->base_delay_ms = base_delay_ms;
    return SKY_WEBHOOK_OK;
}

uint32_t sky_webhook_retry_delay_ms(const sky_webhook_plan *plan, unsigned int attempt_index) {
    uint64_t delay;
    unsigned int retry_index;

    if (plan == NULL || attempt_index >= plan->max_attempts) return UINT32_MAX;
    if (attempt_index == 0U) return 0U;

    delay = plan->base_delay_ms;
    for (retry_index = 1U; retry_index < attempt_index; ++retry_index) {
        delay *= 2U;
        if (delay >= SKY_WEBHOOK_MAX_DELAY_MS) return SKY_WEBHOOK_MAX_DELAY_MS;
    }
    return (uint32_t)(delay > SKY_WEBHOOK_MAX_DELAY_MS ? SKY_WEBHOOK_MAX_DELAY_MS : delay);
}

const char *sky_webhook_result_string(sky_webhook_result result) {
    switch (result) {
        case SKY_WEBHOOK_OK: return "ok";
        case SKY_WEBHOOK_INVALID_URL: return "invalid_url";
        case SKY_WEBHOOK_INVALID_EVENT: return "invalid_event";
        case SKY_WEBHOOK_INVALID_PAYLOAD: return "invalid_payload";
        case SKY_WEBHOOK_INVALID_RETRY_POLICY: return "invalid_retry_policy";
        default: return "unknown";
    }
}

int main(void) {
    sky_webhook_plan plan;
    sky_webhook_result result = sky_webhook_plan_init(
        &plan,
        "https://hooks.example.test/events",
        "order.created",
        "{\"orderId\":\"demo-1\"}",
        5U,
        500U
    );
    unsigned int attempt;

    if (result != SKY_WEBHOOK_OK) {
        fprintf(stderr, "plan error: %s\n", sky_webhook_result_string(result));
        return 1;
    }

    printf("webhook delivery plan: event=%s payload_bytes=%zu attempts=%u network_delivery=false\n",
           plan.event, plan.payload_len, plan.max_attempts);
    for (attempt = 0U; attempt < plan.max_attempts; ++attempt) {
        printf("attempt=%u delay_ms=%u\n", attempt + 1U,
               sky_webhook_retry_delay_ms(&plan, attempt));
    }
    return 0;
}
