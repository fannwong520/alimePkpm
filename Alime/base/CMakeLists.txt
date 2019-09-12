set(base_SRCS
  duration.cpp
  TimeStamp.cpp
  )

add_library(Alime_base ${base_SRCS})
#target_link_libraries(Alime_base pthread rt)

install(TARGETS Alime_base DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/Alime/base)
install(FILES ${HEADERS} DESTINATION include/Alime/base/build)
install(FILES ${HEADERS} DESTINATION include/Alime/base/file_util)
install(FILES ${HEADERS} DESTINATION include/Alime/base/third_party)
install(FILES ${HEADERS} DESTINATION include/Alime/base/thread)
install(FILES ${HEADERS} DESTINATION include/Alime/base/util)

if(NOT CMAKE_BUILD_NO_EXAMPLES)
  add_subdirectory(tests)
endif()
