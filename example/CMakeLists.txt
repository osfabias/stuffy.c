add_executable(example main.c)
target_link_libraries(example PRIVATE stuffy)
target_compile_options(example PRIVATE ${STUFFY_COMPILE_OPTIONS})