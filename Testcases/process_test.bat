is_in_path programs
start countdown.exe 3
add_path programs
is_in_path programs
start programs/countdown.exe 100 &
start programs/tictactoe.exe &
start programs/spinner.exe 
start countdown.exe 
list_proc
remove_path programs
