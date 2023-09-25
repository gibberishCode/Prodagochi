find_package(nlohmann_json REQUIRED)

add_executable(prodagochi src/prodgagochi.cpp src/main.cpp src/native.cpp)
target_include_directories(prodagochi PRIVATE include)
target_link_libraries(prodagochi PUBLIC engine nlohmann_json::nlohmann_json)

add_custom_target(copy-runtime-files ALL
        #        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/prodagochi/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/prodagochi
        DEPENDS prodagochi)