set(CMAKE_SYSTEM_NAME          Generic)
set(CMAKE_SYSTEM_PROCESSOR     arm)

set(CROSS_COMPILE arm-none-eabi-)

set(CMAKE_C_COMPILER   "${CROSS_COMPILE}gcc")
set(CMAKE_ASM_COMPILER "${CROSS_COMPILE}gcc")
set(CMAKE_OBJCOPY      "${CROSS_COMPILE}objcopy")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -mfpu=neon-vfpv4")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -march=armv7-a")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -mtune=cortex-a9")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -DARCH=${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O0")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -g")

set(CMAKE_ASM_FLAGS ${CMAKE_C_FLAGS})

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -nostartfiles")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -mfloat-abi=hard")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -ffreestanding")

set(CMAKE_EXE_LINKER_FLAGS "-specs=nosys.specs")
