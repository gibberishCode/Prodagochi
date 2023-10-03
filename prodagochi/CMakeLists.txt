find_package(nlohmann_json REQUIRED)

add_executable(prodagochi src/prodgagochi.cpp src/main.cpp src/native.cpp)
target_include_directories(prodagochi PRIVATE include)
target_link_libraries(prodagochi PUBLIC engine nlohmann_json::nlohmann_json)
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_BINARY_DIR})
add_custom_target(copy-runtime-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/prodagochi/assets)
add_dependencies(prodagochi copy-runtime-files)
