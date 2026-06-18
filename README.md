# C-Webhook-Dispatcher

## Overview
A lightweight webhook dispatcher written in C with exponential backoff retry logic and dead-letter handling.

## Quick Start (1-Click Build)

```bash
git clone https://github.com/skylerblue333/C-Webhook-Dispatcher.git
cd C-Webhook-Dispatcher
mkdir build && cd build
cmake .. && make
./webhook_dispatcher
```

## Features
- Configurable retry attempts
- Simulated HTTP POST with failure rate
- Dead-letter queue for undeliverable webhooks
