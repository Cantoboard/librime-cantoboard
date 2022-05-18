project(rime-cantoboard)
cmake_minimum_required(VERSION 3.10)

set(CANTOBOARD_VERSION 1.0.0)
set(CANTOBOARD_SOVERSION 1)

aux_source_directory(src cantoboard_src)

set(cantoboard_library rime-cantoboard)
set(cantoboard_deps ${rime_library})
set(cantoboard_modules "cantoboard")

# to build as a standard Rime plugin, link or copy the directory to plugins/
add_library(rime-cantoboard-objs OBJECT ${cantoboard_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-cantoboard-objs PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

add_subdirectory(tools)

set(plugin_name ${cantoboard_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-cantoboard-objs> PARENT_SCOPE)
set(plugin_deps ${cantoboard_deps} PARENT_SCOPE)
set(plugin_modules ${cantoboard_modules} PARENT_SCOPE)
