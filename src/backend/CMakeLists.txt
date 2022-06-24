add_library(backend stack.cpp)
target_include_directories(backend PUBLIC inc)
target_link_libraries(backend PRIVATE utils project_options project_warnings)