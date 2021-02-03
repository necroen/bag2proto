cmake_minimum_required(VERSION 2.8.3)
project(bag2proto)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

add_compile_options(-pthread)

find_package(catkin REQUIRED COMPONENTS
    roscpp 
    rospy 
    std_msgs 
    tf 
    sensor_msgs 
    nav_msgs 
)

find_package(Protobuf 3.0.0 REQUIRED)

catkin_package(
 INCLUDE_DIRS "."
#  LIBRARIES bag2proto
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)


## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
"."
${catkin_INCLUDE_DIRS}
)


file(GLOB_RECURSE bag2proto_HDRS "./ros_proto/*.h")
file(GLOB_RECURSE bag2proto_SRCS "./ros_proto/*.cc")

message(STATUS "bag2proto_HDRS: ${bag2proto_HDRS}")
message(STATUS "bag2proto_SRCS: ${bag2proto_SRCS}")

## ============================================

add_executable(bag2proto_node "./src/read.cc" ${bag2proto_HDRS} ${bag2proto_SRCS})  # 

target_include_directories(bag2proto_node SYSTEM PUBLIC 
                                                ${PROTOBUF_INCLUDE_DIR}
                                                )

target_link_libraries(bag2proto_node PUBLIC 
                                    ${PROTOBUF_LIBRARY} 
                                    pthread
                                    ${catkin_LIBRARIES}
                                    )

## ============================================

# 这个节点 将 laserscan pub 出去
add_executable(bag2proto_pubnode "./src/pub.cc" ${bag2proto_HDRS} ${bag2proto_SRCS})  # 

target_include_directories(bag2proto_pubnode SYSTEM PUBLIC 
                                                ${PROTOBUF_INCLUDE_DIR}
                                                )

target_link_libraries(bag2proto_pubnode PUBLIC 
                                    ${PROTOBUF_LIBRARY} 
                                    pthread
                                    ${catkin_LIBRARIES}
                                    )