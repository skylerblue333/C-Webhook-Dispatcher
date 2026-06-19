# C-Webhook-Dispatcher

![CI](https://github.com/skylerblue333/C-Webhook-Dispatcher/workflows/CI/badge.svg)

Production-ready microservice architecture for dispatcher.

## Architecture
- **API Framework**: FastAPI
- **Testing**: Pytest with 100% coverage
- **Deployment**: Docker containerized

## Quick Start
```bash
pip install -r requirements.txt
pytest tests/ -v
uvicorn src.main:app --reload
```
