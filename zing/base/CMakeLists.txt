set(base_SRCS
  CountDownLatch.cpp
  CurrentThread.cpp
  Date.cpp
  Exception.cpp
  FileUtil.cpp
  LogFile.cpp
  Logging.cpp
  LogStream.cpp
  ProcessInfo.cpp
  Timestamp.cpp
  Thread.cpp
  ThreadPool.cpp
  TimeZone.cpp
  )

add_library(zing_base ${base_SRCS})
target_link_libraries(zing_base pthread rt)

install(TARGETS zing_base DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/zing/base)

if(ZING_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()
