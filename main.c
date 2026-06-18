#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_RETRIES 3
#define MAX_WEBHOOKS 50

typedef struct {
    int id;
    char url[256];
    char payload[512];
    int retries;
    int delivered;
} Webhook;

Webhook queue[MAX_WEBHOOKS];
int queue_size = 0;

int mock_http_post(const char *url, const char *payload) {
    /* Simulate 70% success rate */
    return (rand() % 10) < 7 ? 200 : 500;
}

void enqueue_webhook(const char *url, const char *payload) {
    if (queue_size >= MAX_WEBHOOKS) return;
    Webhook *w = &queue[queue_size++];
    w->id = queue_size;
    strncpy(w->url, url, 255);
    strncpy(w->payload, payload, 511);
    w->retries = 0;
    w->delivered = 0;
}

void dispatch_all() {
    printf("=== Webhook Dispatcher ===\n\n");
    for (int i = 0; i < queue_size; i++) {
        Webhook *w = &queue[i];
        while (!w->delivered && w->retries < MAX_RETRIES) {
            int status = mock_http_post(w->url, w->payload);
            w->retries++;
            if (status == 200) {
                w->delivered = 1;
                printf("[OK]    Webhook #%d delivered to %s (attempt %d)\n",
                       w->id, w->url, w->retries);
            } else {
                printf("[RETRY] Webhook #%d failed (%d), retrying... (%d/%d)\n",
                       w->id, status, w->retries, MAX_RETRIES);
                usleep(100000); /* 100ms backoff */
            }
        }
        if (!w->delivered) {
            printf("[DEAD]  Webhook #%d to %s failed after %d attempts.\n",
                   w->id, w->url, MAX_RETRIES);
        }
    }
}

int main() {
    srand(42);
    enqueue_webhook("https://api.example.com/hook", "{\"event\":\"user.created\"}");
    enqueue_webhook("https://api.partner.io/events", "{\"event\":\"order.paid\"}");
    enqueue_webhook("https://hooks.slack.com/abc123", "{\"text\":\"Deploy complete\"}");
    dispatch_all();
    return 0;
}
