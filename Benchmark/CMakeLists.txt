
add_executable(tpbench Main.cpp timer.cpp timer.h)
target_link_libraries(tpbench tp Threads::Threads)
