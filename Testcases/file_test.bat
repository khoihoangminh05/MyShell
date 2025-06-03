touch file1.txt file2.txt "file with spaces.txt"
exists file1.txt non_existent_file.txt
write_file "Hello World" file1.txt
read_file file1.txt
write_file "First line" file1.txt ~HEAD
read_file file1.txt
write_file "Second line (inserted)" file1.txt ~LINE 2
read_file file1.txt
read_file file1.txt ~HEAD 2
read_file file1.txt ~FOOT 2


read_file file1.txt ~RANGE 2 3


copy_file file1.txt file1_copy.txt


rename_file file1_copy.txt file1_renamed.txt


file_info file1.txt


compare file1.txt file1_renamed.txt
write_file "different" file2.txt # Ghi đè file2.txt để khác
compare file1.txt file2.txt


abspath file1.txt


normalize file1.txt


rm file1.txt
rm file2.txt
rm "file with spaces.txt"
rm file1_renamed.txt

