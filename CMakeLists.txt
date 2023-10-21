cmake_minimum_required(VERSION 3.27)
project(prodgochi)

set(CMAKE_CXX_STANDARD 23)
list(APPEND CMAKE_PREFIX_PATH "/build")
list(APPEND CMAKE_MODULE_PATH "/build")
set(GLFWPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # disable building GLFWPP examples
add_definitions(-DGLFW_INCLUDE_NONE)
add_subdirectory(glfwpp)
add_subdirectory(engine)
add_subdirectory(prodagochi)

#
#find_package(OpenGL REQUIRED)
#find_package(glad REQUIRED)
#find_package(glfw3 REQUIRED)
#find_package(freetype REQUIRED)
#find_package(glm REQUIRED)
#find_package(X11 REQUIRED)
#
#set(RENDER_LIBS OpenGL::GL glad::glad glfw Freetype::Freetype glm::glm ${X11_LIBRARIES})
#
##add_executable(lol main.cpp)
##target_link_libraries(lol ${RENDER_LIBS})


#
#add_library(simple)
#target_sources(simple PRIVATE FILE_SET  cxx_modules TYPE CXX_MODULES FILES
#    module_test.ixx
#)
