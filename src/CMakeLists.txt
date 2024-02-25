add_executable(orbbit
  main.cpp
  obj.cpp
  phys.cpp
  physmanager.cpp
  cursor.cpp
  game.cpp
)
target_link_libraries(orbbit GraphicsEnv)

set(exec-name orbbit)
if(GCC OR MINGW)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # get linux binary to check current dir for libraries
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath='${ORIGIN}'")
    if(GRAPHICS_BUILD_STATIC)
      message(WARNING "Static build!")
      target_link_libraries(${exec-name} -static -static-libgcc -static-libstdc++)
    endif()
  endif()
  target_link_libraries(${exec-name} -pthread)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(GCC OR MINGW)
    target_link_libraries(${exec-name} -mwindows)
  endif()
endif()

add_custom_command(TARGET ${exec-name} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_CURRENT_LIST_DIR}/../resources"
  $<TARGET_FILE_DIR:${exec-name}>)
