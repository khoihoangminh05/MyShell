create test_dir_1
create nested_dir/sub_dir_1

list_tree .

copy test_dir_1 test_dir_1_copy

rename test_dir_1_copy test_dir_1_renamed

is_empty test_dir_1
is_empty nested_dir/sub_dir_1

get_size .


move test_dir_1 nested_dir/moved_dir


delete nested_dir/sub_dir_1
delete nested_dir/moved_dir
delete test_dir_1_renamed
delete nested_dir
delete test_dir_1

