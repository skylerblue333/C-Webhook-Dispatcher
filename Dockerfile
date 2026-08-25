FROM gcc:14 AS build
WORKDIR /src
COPY CMakeLists.txt ./
COPY include ./include
COPY src ./src
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --target sky_webhook_plan_cli --parallel 2

FROM alpine:3.22
RUN addgroup -S app && adduser -S app -G app
WORKDIR /app
COPY --from=build /src/build/sky_webhook_plan_cli ./sky_webhook_plan_cli
USER app
CMD ["./sky_webhook_plan_cli"]
