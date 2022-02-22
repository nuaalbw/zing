include(CheckFunctionExists)

check_function_exists(accept4 HAVE_ACCEPT4)
if(NOT HAVE_ACCEPT4)
  set_source_files_properties(SocketsOps.cc PROPERTIES COMPILE_FLAGS "-DNO_ACCEPT4")
endif()

set(net_SRCS
  Acceptor.cpp
  Buffer.cpp
  Channel.cpp
  EventLoop.cpp
  EventLoopThread.cpp
  EventLoopThreadPool.cpp
  InetAddress.cpp
  Poller.cpp
  PollPoller.cpp
  EPollPoller.cpp
  DefaultPoller.cpp
  Socket.cpp
  SocketsOps.cpp
  TcpConnection.cpp
  TcpServer.cpp
  Timer.cpp
  TimerQueue.cpp
  )

add_library(zing_net ${net_SRCS})
target_link_libraries(zing_net zing_base)

install(TARGETS zing_net DESTINATION lib)

set(HEADERS
  Buffer.h
  Callbacks.h
  Channel.h
  Endian.h
  EventLoop.h
  EventLoopThread.h
  EventLoopThreadPool.h
  InetAddress.h
  TcpClient.h
  TcpConnection.h
  TcpServer.h
  TimerId.h
  )
install(FILES ${HEADERS} DESTINATION include/muduo/net)

# add_subdirectory(http)
# add_subdirectory(inspect)

if(ZING_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()