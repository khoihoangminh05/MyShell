# --- Running File Commands Test ---

# === Test 1: Create Files ===
touch file1.txt file2.txt "file with spaces.txt"

# === Test 2: Check File Existence ===
exists file1.txt non_existent_file.txt

# === Test 3: Write to File (append) ===
write_file "Hello World" file1.txt
read_file file1.txt

# === Test 4: Write to File (~HEAD) ===
write_file "First line" file1.txt ~HEAD
read_file file1.txt

# === Test 5: Write to File (~LINE 2) ===
write_file "Second line (inserted)" file1.txt ~LINE 2
read_file file1.txt

# === Test 6: Read File (~HEAD 2) ===
read_file file1.txt ~HEAD 2

# === Test 7: Read File (~FOOT 2) ===
read_file file1.txt ~FOOT 2

# === Test 8: Read File (~RANGE 2 3) ===
read_file file1.txt ~RANGE 2 3

# === Test 9: Copy File ===
copy_file file1.txt file1_copy.txt

# === Test 10: Rename File ===
rename_file file1_copy.txt file1_renamed.txt

# === Test 11: Get File Info ===
file_info file1.txt

# === Test 12: Compare Files ===
compare file1.txt file1_renamed.txt
write_file "different" file2.txt # Ghi đè file2.txt để khác
compare file1.txt file2.txt

# === Test 13: Get Absolute Path ===
abspath file1.txt

# === Test 14: Get Normalized Path ===
normalize file1.txt

# === Test 15: Clean up (Delete created files) ===
delete_file file1.txt
delete_file file2.txt
delete_file "file with spaces.txt"
delete_file file1_renamed.txt

# --- File Commands Test Finished ---