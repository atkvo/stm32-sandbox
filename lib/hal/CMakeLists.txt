set(LIBRARY_NAME hal)
if (MCU STREQUAL STM32F411XE)
    add_library(${LIBRARY_NAME}
        src/stm32f411xe/gpio.cpp
    )
else()
    message(FATAL_ERROR "Please define an MCU")
endif()

target_include_directories(hal PUBLIC
    ${CUBE_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# todo: is there a better way to compile this
# with the same compile options as defined in
# the "root" cmake file?
target_compile_options(${LIBRARY_NAME} PRIVATE
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
