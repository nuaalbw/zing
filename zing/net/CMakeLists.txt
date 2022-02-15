include(CheckFunctionExists)

check_function_exists(accept4 HAVE_ACCEPT4)
if(NOT HAVE_ACCEPT4)
  set_source_files_properties(SocketsOps.cc PROPERTIES COMPILE_FLAGS "-DNO_ACCEPT4")
endif()

set(net_SRCS
  # Acceptor.cc
  # Buffer.cc
  Channel.cpp
  # Connector.cc
  EventLoop.cpp
  EventLoopThread.cpp
  # EventLoopThreadPool.cc
  InetAddress.cpp
  Poller.cpp
  PollPoller.cpp
  EPollPoller.cpp
  DefaultPoller.cpp
  Socket.cpp
  SocketsOps.cpp
  # TcpClient.cc
  # TcpConnection.cc
  # TcpServer.cc
  Timer.cpp
  TimerQueue.cpp
  )

add_library(zing_net ${net_SRCS})
target_link_libraries(zing_net zing_base)

#add_library(muduo_net_cpp11 ${net_SRCS})
#target_link_libraries(muduo_net_cpp11 muduo_base_cpp11)
#set_target_properties(muduo_net_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS zing_net DESTINATION lib)
#install(TARGETS muduo_net_cpp11 DESTINATION lib)

set(HEADERS
  # Buffer.h
  # Callbacks.h
  # Channel.h
  # Endian.h
  # EventLoop.h
  # EventLoopThread.h
  # EventLoopThreadPool.h
  # InetAddress.h
  # TcpClient.h
  # TcpConnection.h
  # TcpServer.h
  # TimerId.h
  )
install(FILES ${HEADERS} DESTINATION include/muduo/net)

# add_subdirectory(http)
# add_subdirectory(inspect)

if(ZING_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()

# if(PROTOBUF_FOUND)
#   add_subdirectory(protobuf)
#   add_subdirectory(protorpc)
# else()
#   add_subdirectory(protobuf EXCLUDE_FROM_ALL)
#   add_subdirectory(protorpc EXCLUDE_FROM_ALL)
# endif()
