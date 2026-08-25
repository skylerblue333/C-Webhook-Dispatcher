FROM gcc:14 AS build
RUN apt-get update \
    && apt-get install -y --no-install-recommends cmake \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY CMakeLists.txt ./
COPY include ./include
COPY src ./src
COPY tests ./tests
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --target sky_webhook_plan_cli --parallel 2

FROM alpine:3.22
RUN addgroup -S app && adduser -S app -G app
WORKDIR /app
COPY --from=build /src/build/sky_webhook_plan_cli ./sky_webhook_plan_cli
USER app
CMD ["./sky_webhook_plan_cli"]
