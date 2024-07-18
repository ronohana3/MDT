cmake_minimum_required(VERSION 3.10)

project(IronDrone)

set (CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
# include_directories(${OpenCV_INCLUDE_DIRS})

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
    Utils/SocketClient.cpp
    main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(FFMPEG REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES}  ${OpenCV_LIBS})

