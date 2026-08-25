#include "webhook_plan.h"

#include <stdio.h>

int main(void) {
    sky_webhook_plan plan;
    const sky_webhook_result result = sky_webhook_plan_init(
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
