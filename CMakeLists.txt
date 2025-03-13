cmake_minimum_required(VERSION 3.10)

project(Arkanoid)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INC_PATH include/)
set(OpenCV_DIR "C:/opencv/opencv/build/x64/vc16/lib")
# set(OpenCV_LIB_PATH "C:/opencv/opencv/build/x64/vc16/lib")
# set(CMAKE_PREFIX_PATH "C:/opencv/opencv/build")
find_package(OpenCV REQUIRED)

file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})