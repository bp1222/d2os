project(kernel_arch_arm C ASM)

set(ARCH_KERNEL_LD_SCRIPT "${PROJECT_SOURCE_DIR}/kernel.ld" PARENT_SCOPE)
set(ARCH_KERNEL_TARGET ${PROJECT_NAME} PARENT_SCOPE)
set(ARCH_KERNEL_TARGET ${PROJECT_NAME})

set(ARCH_KERNEL_C_SOURCES 
  main.c
  memory.c
  mmio.c
  atags/atags.c
)
set(ARCH_KERNEL_ASM_SOURCES
  boot.S
  interrupt.S
  mutex.S
)

add_library(${ARCH_KERNEL_TARGET} ${ARCH_KERNEL_C_SOURCES} ${ARCH_KERNEL_ASM_SOURCES})
target_include_directories(${ARCH_KERNEL_TARGET} PUBLIC
  "$<BUILD_INTERFACE:${KERNEL_INCLUDE_DIRS}>"
)