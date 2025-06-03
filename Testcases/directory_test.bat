# --- Running Directory Commands Test ---

# === Test 1: Create Directory ===
create test_dir_1
create nested_dir/sub_dir_1

# === Test 2: List Directory Tree ===
list_tree .

# === Test 3: Copy Directory ===
copy test_dir_1 test_dir_1_copy

# === Test 4: Rename Directory ===
rename test_dir_1_copy test_dir_1_renamed

# === Test 5: Check if Directory is Empty ===
is_empty test_dir_1
is_empty nested_dir/sub_dir_1

# === Test 6: Get Directory Size ===
get_size .

# === Test 7: Move Directory ===
move test_dir_1 nested_dir/moved_dir

# === Test 8: Clean up (Delete created directories) ===
# Bạn sẽ cần xác nhận "yes" cho mỗi lệnh delete
delete nested_dir/sub_dir_1
delete nested_dir/moved_dir
delete test_dir_1_renamed
delete nested_dir
delete test_dir_1

# --- Directory Commands Test Finished ---