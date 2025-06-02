
set(BACKGROUND_PROGRAMS
    countdown
    tictactoe
    spinner
    logger
)

foreach(prog IN LISTS BACKGROUND_PROGRAMS)
    add_executable(${prog} ${prog}.cpp)
    # Đặt tên rõ ràng hơn nếu cần
    set_target_properties(${prog} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/programs)
endforeach()
