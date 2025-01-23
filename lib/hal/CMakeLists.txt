set(LIBRARY_NAME hal)

if (MCU STREQUAL STM32F411XE)
    add_library(${LIBRARY_NAME}
        board/stm32f411xe/src/adc.cpp
        board/stm32f411xe/src/i2c.cpp
        board/stm32f411xe/src/gpio.cpp
        board/stm32f411xe/src/usart.cpp
    )

    target_include_directories(hal PUBLIC
        board/stm32f411xe/inc
    )
else()
    message(FATAL_ERROR "Please define an MCU")
endif()

target_include_directories(hal PUBLIC
    ${CUBE_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
