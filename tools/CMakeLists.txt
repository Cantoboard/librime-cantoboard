set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(cantoboard_console_src "cantoboard_console.cc")
add_executable(cantoboard_console ${cantoboard_console_src})
target_link_libraries(cantoboard_console
  ${rime_library}
  ${rime_dict_library}
  ${rime_gears_library}
  ${rime_levers_library})
