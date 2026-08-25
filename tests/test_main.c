#include "webhook_plan.h"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    sky_webhook_plan plan;
    sky_webhook_result result;
    char oversized_payload[SKY_WEBHOOK_PAYLOAD_MAX + 2U];

    result = sky_webhook_plan_init(
        &plan,
        "https://hooks.example.test/events",
        "order.created",
        "{\"id\":1}",
        5U,
        500U
    );
    assert(result == SKY_WEBHOOK_OK);
    assert(strcmp(plan.event, "order.created") == 0);
    assert(plan.payload_len == strlen("{\"id\":1}"));
    assert(sky_webhook_retry_delay_ms(&plan, 0U) == 0U);
    assert(sky_webhook_retry_delay_ms(&plan, 1U) == 500U);
    assert(sky_webhook_retry_delay_ms(&plan, 2U) == 1000U);
    assert(sky_webhook_retry_delay_ms(&plan, 4U) == 4000U);
    assert(sky_webhook_retry_delay_ms(&plan, 5U) == UINT32_MAX);

    assert(sky_webhook_plan_init(&plan, "http://example.test", "event", "{}", 3U, 100U)
           == SKY_WEBHOOK_INVALID_URL);
    assert(sky_webhook_plan_init(&plan, "https://example.test", "bad event", "{}", 3U, 100U)
           == SKY_WEBHOOK_INVALID_EVENT);
    assert(sky_webhook_plan_init(&plan, "https://example.test", "event", "", 3U, 100U)
           == SKY_WEBHOOK_INVALID_PAYLOAD);
    assert(sky_webhook_plan_init(&plan, "https://example.test", "event", "{}", 0U, 100U)
           == SKY_WEBHOOK_INVALID_RETRY_POLICY);
    assert(sky_webhook_plan_init(&plan, "https://example.test", "event", "{}", 9U, 100U)
           == SKY_WEBHOOK_INVALID_RETRY_POLICY);

    memset(oversized_payload, 'x', sizeof(oversized_payload));
    oversized_payload[sizeof(oversized_payload) - 1U] = '\0';
    assert(sky_webhook_plan_init(&plan, "https://example.test", "event", oversized_payload, 3U, 100U)
           == SKY_WEBHOOK_INVALID_PAYLOAD);

    assert(sky_webhook_plan_init(&plan, "https://example.test", "event", "{}", 8U, 60000U)
           == SKY_WEBHOOK_OK);
    assert(sky_webhook_retry_delay_ms(&plan, 4U) == SKY_WEBHOOK_MAX_DELAY_MS);
    assert(sky_webhook_retry_delay_ms(&plan, 7U) == SKY_WEBHOOK_MAX_DELAY_MS);

    puts("all webhook plan tests passed");
    return 0;
}
