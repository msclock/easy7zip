set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)
find_package(7zip CONFIG REQUIRED)

pybind11_add_module(_core MODULE src/pybind.cpp)
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_link_libraries(_core PRIVATE 7zip::7zip)

add_executable(py7z src/main.cpp)
target_compile_definitions(py7z PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_link_libraries(py7z PRIVATE 7zip::7zip)

install(TARGETS _core py7z DESTINATION ${SKBUILD_PROJECT_NAME})
