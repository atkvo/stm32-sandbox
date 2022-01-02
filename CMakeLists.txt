cmake_minimum_required(VERSION 3.1)

project(
    stm32-test
    VERSION 1.0
    LANGUAGES C CXX ASM
)

set(EXECUTABLE ${PROJECT_NAME}.out)

set(MCU STM32F411XE)

set(CUBE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/STM32CubeF4)
set(CMSIS_STM32F4XX_TEMPLATE ${CUBE_ROOT_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates)

set(STM32CUBEMX_GENERATED_FILES
    ${CMSIS_STM32F4XX_TEMPLATE}/gcc/startup_stm32f411xe.s
    ${CMSIS_STM32F4XX_TEMPLATE}/system_stm32f4xx.c
)

set(SOURCES 
    ${STM32CUBEMX_GENERATED_FILES}
    app/main.cpp
)

add_executable(${EXECUTABLE} ${SOURCES})

target_compile_definitions(${EXECUTABLE} PRIVATE
        # -DUSE_HAL_DRIVER
        -DSTM32F411xE
)

set(CUBE_INCLUDES
        ${CUBE_ROOT_DIR}/Inc
        ${CUBE_ROOT_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
        ${CUBE_ROOT_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
        ${CUBE_ROOT_DIR}/Drivers/CMSIS/Include
)

target_include_directories(${EXECUTABLE} PUBLIC
        ${CUBE_INCLUDES}
        ${CMAKE_SOURCE_DIR}
        )

target_link_libraries(${EXECUTABLE} PUBLIC hal)
add_subdirectory(lib/hal)

target_compile_options(${EXECUTABLE} PUBLIC
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -fdata-sections
        -ffunction-sections
        -fno-rtti

        -Wall

        $<$<CONFIG:Debug>:-Og>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32F411CEUx_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -specs=nosys.specs
        -lstdc++
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)