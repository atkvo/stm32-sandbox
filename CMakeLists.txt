cmake_minimum_required(VERSION 3.1)

project(
    stm32-sandbox
    VERSION 1.0
    LANGUAGES C CXX ASM
)

set(EXECUTABLE ${PROJECT_NAME}.elf)

set(MCU STM32F411XE)

set(SOURCES
    ${SOURCES}
    app/main.cpp
)

# set(CXXFLAGS  " -Wall -std=c++17")
# set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

add_executable(${EXECUTABLE} ${SOURCES})

target_compile_definitions(${EXECUTABLE} PRIVATE
        # -DUSE_HAL_DRIVER
        -DSTM32F411xE
)

target_include_directories(${EXECUTABLE} PUBLIC
        ${CUBE_INCLUDES}
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${EXECUTABLE} PUBLIC hal)
add_subdirectory(lib/hal)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
