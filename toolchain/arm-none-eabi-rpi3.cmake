set( CMAKE_SYSTEM_NAME          Generic )
set( CMAKE_SYSTEM_PROCESSOR     BCM2837 )

set( CROSS_COMPILE arm-none-eabi- )

set( CMAKE_C_COMPILER   "${CROSS_COMPILE}gcc" )
set( CMAKE_ASM_COMPILER "${CROSS_COMPILE}gcc" )

set( CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY )

set( CMAKE_OBJCOPY      "${CROSS_COMPILE}objcopy"
    CACHE FILEPATH "The toolchain objcopy command " FORCE )

set( CMAKE_C_FLAGS      "${CMAKE_C_FLAGS} -mfloat-abi=hard" )
set( CMAKE_C_FLAGS      "${CMAKE_C_FLAGS} -mfpu=neon-fp-armv8" )
set( CMAKE_C_FLAGS      "${CMAKE_C_FLAGS} -march=armv8-a" )
set( CMAKE_C_FLAGS      "${CMAKE_C_FLAGS} -mtune=cortex-a53" )

set( CMAKE_C_FLAGS      "${CMAKE_C_FLAGS}" CACHE STRING "" )
set( CMAKE_ASM_FLAGS    "${CMAKE_C_FLAGS}" CACHE STRING "" )
