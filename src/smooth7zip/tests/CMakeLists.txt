file(GLOB files "*.cpp")

foreach(_file ${files})
  get_filename_component(file_basename ${_file} NAME_WE)
  add_executable(${file_basename} ${_file})
  target_link_libraries(${file_basename} PRIVATE GTest::gtest_main smooth7zip
                                                 7zip::7zip spdlog::spdlog)
  gtest_discover_tests(${file_basename})
endforeach()
