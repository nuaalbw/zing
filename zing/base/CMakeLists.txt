set(base_SRCS
  # AsyncLogging.cc
  # Condition.cc
  # CountDownLatch.cc
  CurrentThread.cpp
  # Date.cc
  Exception.cpp
  # FileUtil.cc
  # LogFile.cc
  # Logging.cc
  # LogStream.cc
  # ProcessInfo.cc
  Timestamp.cpp
  # Thread.cc
  # ThreadPool.cc
  # TimeZone.cc
  )

add_library(zing_base ${base_SRCS})
target_link_libraries(zing_base pthread rt)

#add_library(muduo_base_cpp11 ${base_SRCS})
#target_link_libraries(muduo_base_cpp11 pthread rt)
#set_target_properties(muduo_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS zing_base DESTINATION lib)
#install(TARGETS muduo_base_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/zing/base)

if(ZING_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()
