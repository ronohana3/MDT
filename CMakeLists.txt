cmake_minimum_required(VERSION 3.10)

project(MDT)

set (CMAKE_CXX_STANDARD 17)
find_package( OpenCV REQUIRED PATHS "D:/opencv/build")
include_directories(${OpenCV_INCLUDE_DIRS})


set(SOURCE_FILES
    Tracking/CMT.cpp
    Tracking/common.cpp
    Tracking/Consensus.cpp
    Tracking/Fusion.cpp
    Tracking/Matcher.cpp
    Tracking/Tracker.cpp
    Tracking/fastcluster/fastcluster.cpp
    Detection/inference.cpp
    Controllers/NavigationController.cpp
    Controllers/CameraController.cpp
    Controllers/DroneController.cpp
    main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
