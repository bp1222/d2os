cmake_minimum_required( VERSION 2.12 )

project(kernel C ASM)

macro(add_source SOURCE_GLOB)
	file(GLOB_RECURSE SOURCES "${SOURCE_GLOB}")
  list(APPEND "KERNEL_SOURCES" "${SOURCES}")
endmacro(add_source)

file(GLOB_RECURSE KERNEL_HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

add_source("${CMAKE_CURRENT_LIST_DIR}/kernel/drivers/*.c")
add_source("${CMAKE_CURRENT_LIST_DIR}/kernel/utils/*.c")

add_source("${CMAKE_CURRENT_LIST_DIR}/kernel/interrupt.c")
add_source("${CMAKE_CURRENT_LIST_DIR}/kernel/main.c")
add_source("${CMAKE_CURRENT_LIST_DIR}/kernel/panic.c")

set(KERNEL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}.elf ${KERNEL_SOURCES})

if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm")
  add_subdirectory(kernel/arch/arm)
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${ARCH_KERNEL_LD_SCRIPT}")

target_link_libraries(${PROJECT_NAME}.elf ${ARCH_KERNEL_TARGET})
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${KERNEL_INCLUDE_DIR})

add_custom_command(
  TARGET ${PROJECT_NAME}.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ./${PROJECT_NAME}.elf -O binary ./${PROJECT_NAME}.img)
