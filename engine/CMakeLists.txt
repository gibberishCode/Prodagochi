file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

find_package(OpenGL REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(freetype REQUIRED)
find_package(glm REQUIRED)
find_package(X11 REQUIRED)
find_package(assimp REQUIRED)

set(RENDER_LIBS OpenGL::GL glad::glad glfw Freetype::Freetype glm::glm ${X11_LIBRARIES} assimp::assimp)

#add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
add_library(engine ${SRC_FILES})
target_include_directories(engine PUBLIC include)
target_link_libraries(engine PUBLIC ${RENDER_LIBS} GLFWPP)
