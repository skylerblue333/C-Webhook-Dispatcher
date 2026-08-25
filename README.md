# Sky Webhook Plan

**Status: engineering beta.**

This repository's historical name is **C-Webhook-Dispatcher**, but the verified implementation is a dependency-free C11 **webhook delivery planning core**. It validates webhook delivery inputs and calculates a bounded deterministic retry schedule. It does **not** perform network delivery.

## Implemented

- HTTPS-only endpoint validation with a 2,048-character bound
- bounded event names using alphanumeric, `.`, `_`, and `-`
- bounded non-empty payloads up to 4 KiB
- retry policies limited to 1–8 attempts
- base delays limited to 100–60,000 ms
- deterministic exponential retry delays capped at 300,000 ms
- reusable static C library and small CLI demonstration
- strict compiler warnings-as-errors
- deterministic CTest coverage
- AddressSanitizer/UndefinedBehaviorSanitizer verification
- non-root container packaging
- CLI truth signal: `network_delivery=false`

## Build and verify

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
./build/sky_webhook_plan_cli
```

The CLI prints a sample delivery plan. It deliberately states that network delivery was not performed.

## SKYCOIN4444 integration

Use this library as a validation/retry-policy boundary in front of a separately implemented webhook transport. A real dispatcher should independently enforce DNS/IP allow-lists, SSRF protections, TLS verification, authentication/signatures, connection and total timeouts, response-size limits, delivery idempotency, durable retry state, observability, and tenant controls.

## Scope limitations

This component does not resolve DNS, open sockets, send HTTP requests, sign payloads, persist retry state, process response codes, provide durable queues, isolate tenants, deliver alerts, guarantee exactly-once semantics, provide HA, or prove production deployment.

See `SECURITY.md` for security boundaries.
