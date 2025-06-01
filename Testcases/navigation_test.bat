# --- Running Navigation Commands Test ---

# === Test 1: Print Working Directory (pwd) ===
pwd

# === Test 2: Change Directory (cd) ===
create temp_nav_dir # Tạo thư mục tạm để cd vào
cd temp_nav_dir
pwd

# === Test 3: List Directory Contents (ls) ===
ls .
touch file_in_nav_dir.txt # Tạo file để ls thấy
ls .

# === Test 4: Change Directory back and Clean up ===
cd ..
pwd
delete temp_nav_dir # Xóa thư mục tạm

# --- Navigation Commands Test Finished ---