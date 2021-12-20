cmake_minimum_required( VERSION 2.12 )

project(kernel C ASM)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -specs=nosys.specs")

file(GLOB_RECURSE CORE_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.c"
                               "${CMAKE_CURRENT_LIST_DIR}/*.s")
file(GLOB_RECURSE CORE_HEADERS "${CMAKE_CURRENT_LIST_DIR}/*.h")

add_executable(${PROJECT_NAME} ${CORE_SOURCES})

target_link_libraries(${PROJECT_NAME} m)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ./${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} -O binary ./${PROJECT_NAME}.img)