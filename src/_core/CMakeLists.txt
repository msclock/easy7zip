set(PYBIND11_FINDPYTHON ON)

find_package(7zip CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

set(target_name _core)

pybind11_add_module(${target_name} MODULE src/pybind.cpp)

target_compile_definitions(${target_name}
                           PRIVATE VERSION_INFO=${PROJECT_VERSION})

target_include_interface_directories(${target_name}
                                     ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${target_name} PRIVATE smooth7zip 7zip::7zip)

install(TARGETS ${target_name} DESTINATION ${SKBUILD_PROJECT_NAME})

install_dependency(TARGETS ${target_name} INSTALL_SYSTEM_LIBS
                   DEPENDS_DESTINATION ${SKBUILD_PROJECT_NAME})
