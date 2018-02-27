find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_BINARY_DIR}/include)

set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(${CMAKE_SOURCE_DIR}/io/io/inc/root_io.i PROPERTIES CPLUSPLUS ON)

swig_add_module(swig_example python ${CMAKE_SOURCE_DIR}/io/io/inc/root_io.i)
swig_link_libraries(swig_example Core Thread ${PYTHON_LIBRARIES})
