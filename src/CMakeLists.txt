add_executable(dsg-2024 main.cpp)
target_link_libraries(dsg-2024 GraphicsEnv)

set(exec-name dsg-2024)
include("${CMAKE_CURRENT_SOURCE_DIR}/../graphics/cmake/copy-libs.cmake")
add_custom_command(TARGET ${exec-name} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_CURRENT_LIST_DIR}/../resources"
  $<TARGET_FILE_DIR:${exec-name}>)
