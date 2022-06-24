add_library(utils publisher.cpp)
target_include_directories(utils PUBLIC inc)
target_link_libraries(utils PRIVATE project_options project_warnings)