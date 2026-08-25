# Security

Sky Webhook Plan is an engineering-beta input-validation and retry-planning library. It is not a network dispatcher and must not be treated as an SSRF, authentication, delivery, or durable-queue boundary.

The library requires HTTPS-form URLs and bounds URL, event, payload, attempt count, and retry delay inputs. It does not resolve or connect to the supplied URL, so HTTPS syntax validation alone does not establish that a future destination is trusted or safe.

Any transport built on this library must independently implement DNS/IP destination policy, private/link-local/metadata-address blocking, TLS certificate verification, authentication/signature handling, connection and overall timeouts, bounded response reads, secret isolation, durable idempotency/retry state, rate limits, and tenant authorization.

Do not place credentials in sample payloads or public issues. Report vulnerabilities privately through GitHub security reporting when available.
