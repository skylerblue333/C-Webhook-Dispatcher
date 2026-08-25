cmake_minimum_required(VERSION 3.16)
project(SkyWebhookPlan C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(sky_webhook_plan STATIC src/webhook_plan.c)
target_include_directories(sky_webhook_plan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(sky_webhook_plan PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_executable(sky_webhook_plan_cli src/main.c)
target_link_libraries(sky_webhook_plan_cli PRIVATE sky_webhook_plan)
target_compile_options(sky_webhook_plan_cli PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_executable(test_webhook_plan tests/test_main.c)
target_link_libraries(test_webhook_plan PRIVATE sky_webhook_plan)
target_compile_options(test_webhook_plan PRIVATE -Wall -Wextra -Wpedantic -Werror)

enable_testing()
add_test(NAME webhook_plan_tests COMMAND test_webhook_plan)
