#ifndef SKY_WEBHOOK_PLAN_H
#define SKY_WEBHOOK_PLAN_H

#include <stddef.h>
#include <stdint.h>

#define SKY_WEBHOOK_URL_MAX 2048
#define SKY_WEBHOOK_EVENT_MAX 64
#define SKY_WEBHOOK_PAYLOAD_MAX 4096
#define SKY_WEBHOOK_MAX_ATTEMPTS 8
#define SKY_WEBHOOK_MAX_DELAY_MS 300000U

typedef enum {
    SKY_WEBHOOK_OK = 0,
    SKY_WEBHOOK_INVALID_URL = -1,
    SKY_WEBHOOK_INVALID_EVENT = -2,
    SKY_WEBHOOK_INVALID_PAYLOAD = -3,
    SKY_WEBHOOK_INVALID_RETRY_POLICY = -4
} sky_webhook_result;

typedef struct {
    char url[SKY_WEBHOOK_URL_MAX + 1];
    char event[SKY_WEBHOOK_EVENT_MAX + 1];
    char payload[SKY_WEBHOOK_PAYLOAD_MAX + 1];
    size_t payload_len;
    unsigned int max_attempts;
    unsigned int base_delay_ms;
} sky_webhook_plan;

sky_webhook_result sky_webhook_plan_init(
    sky_webhook_plan *plan,
    const char *url,
    const char *event,
    const char *payload,
    unsigned int max_attempts,
    unsigned int base_delay_ms
);

uint32_t sky_webhook_retry_delay_ms(const sky_webhook_plan *plan, unsigned int attempt_index);
const char *sky_webhook_result_string(sky_webhook_result result);

#endif
