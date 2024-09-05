set(LIBRARY_NAME drivers)

add_library(
  ${LIBRARY_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ssd1306.cpp
)

target_include_directories(
  ${LIBRARY_NAME} 
  PUBLIC
  ${CUBE_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/.. # todo: find cleaner way
)
