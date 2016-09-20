add_executable(mutex_main1 mutex_main1.cpp)
target_link_libraries(mutex_main1 pthread)

add_executable(mutex_main2 mutex_main2.cpp)
target_link_libraries(mutex_main2 pthread)