cmake_minimum_required( VERSION 2.12 )

project(kernel C ASM)

macro(add_sources SOURCE_GLOB)
	file(GLOB_RECURSE SOURCES ${SOURCE_GLOB})
  list(APPEND "KERNEL_SOURCES" ${SOURCES})
endmacro(add_sources)

macro(add_source SOURCE)
  list(APPEND "KERNEL_SOURCES" ${SOURCE})
endmacro(add_source)

add_sources("${PROJECT_SOURCE_DIR}/kernel/drivers/*.c")
add_sources("${PROJECT_SOURCE_DIR}/kernel/mm/*.c")
add_sources("${PROJECT_SOURCE_DIR}/kernel/utils/*.c")

add_source("${PROJECT_SOURCE_DIR}/kernel/interrupt.c")
add_source("${PROJECT_SOURCE_DIR}/kernel/main.c")
add_source("${PROJECT_SOURCE_DIR}/kernel/panic.c")

list(APPEND "KERNEL_INCLUDE_DIRS" ${PROJECT_SOURCE_DIR}/include)

if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm")
  LIST(APPEND "KERNEL_INCLUDE_DIRS" "${PROJECT_SOURCE_DIR}/kernel/arch/arm/include")
  add_subdirectory(kernel/arch/arm)
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${ARCH_KERNEL_LD_SCRIPT}")

add_executable(${PROJECT_NAME}.elf ${KERNEL_SOURCES})
target_link_libraries(${PROJECT_NAME}.elf ${ARCH_KERNEL_TARGET})
target_include_directories(${PROJECT_NAME}.elf PUBLIC
  "$<BUILD_INTERFACE:${KERNEL_INCLUDE_DIRS}>"
)

add_custom_command(
  TARGET ${PROJECT_NAME}.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ./${PROJECT_NAME}.elf -O binary ./${PROJECT_NAME}.img)
