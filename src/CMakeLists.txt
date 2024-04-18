file(GLOB_RECURSE FILES *.h *.cpp *hpp)

add_library(${PROJECT_NAME} STATIC ${FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(${PROJECT_NAME} PUBLIC GeographicLib eigen sofa fast-cpp-csv-parser calceph)