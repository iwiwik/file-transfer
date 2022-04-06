cmake_minimum_required (VERSION 3.5)

project("FileTransfer" CXX)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.78.0 REQUIRED COMPONENTS log_setup log system thread)

add_executable(client client.cpp)

target_include_directories(client
  PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include

)

target_link_libraries(client
  PUBLIC
    Boost::log_setup
    Boost::log
    Boost::system
    Boost::thread
    Boost::filesystem
)

target_compile_options(client
  PUBLIC
    -DBOOST_LOG_DYN_LINK
  )
  
add_executable(server server.cpp)

target_include_directories(server
  PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include

  )

target_link_libraries(server
  PUBLIC
    Boost::log_setup
    Boost::log
    Boost::system
    Boost::thread
    Boost::filesystem
  )

target_compile_options(server
  PUBLIC
    -DBOOST_LOG_DYN_LINK
  )
  
