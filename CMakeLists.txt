cmake_minimum_required( VERSION 2.12 )

project(kernel C ASM)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -specs=nosys.specs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

file(GLOB_RECURSE CORE_ASM     "${CMAKE_CURRENT_LIST_DIR}/src/*.s")
file(GLOB_RECURSE CORE_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB_RECURSE CORE_HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${PROJECT_SOURCE_DIR}/src/kernel.ld")

add_executable(${PROJECT_NAME}.elf ${CORE_ASM} ${CORE_SOURCES})

target_link_libraries(${PROJECT_NAME}.elf m)
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_custom_command(
  TARGET ${PROJECT_NAME}.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ./${PROJECT_NAME}.elf -O binary ./${PROJECT_NAME}.img)
