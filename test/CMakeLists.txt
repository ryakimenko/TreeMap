include(CompileOptions)


set(treemapTest treemapTest)

add_executable(${treemapTest} libcsc/treemap.cpp)

add_test(NAME ${treemapTest} COMMAND ${treemapTest})

set_compile_options(${treemapTest})

target_sources(
  ${treemapTest}
  PRIVATE
    libcsc/treemap.cpp
)

target_link_libraries(${treemapTest} PRIVATE treemap gtest  gtest_main)

target_include_directories(${treemapTest} PRIVATE ${CMAKE_SOURCE_DIR}/src/libcsc/)


