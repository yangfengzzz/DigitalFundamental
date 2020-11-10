CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

project(applications)
aux_source_directory(./ DIR_SRCS)

list(APPEND CMAKE_MODULE_PATH "../cmake")
find_package(glog)
find_package(TBB)

link_directories("../build/src.common")
set(CMAKE_CXX_FLAGS "-std=c++17 -O3 ${CMAKE_CXX_FLAGS}")
set(CMAKE_LD_FLAGS "-ldl ${CMAKE_LD_FLAGS}")

ADD_EXECUTABLE(apps ${DIR_SRCS})
target_link_libraries(apps Flex_Common TBB::tbb)
