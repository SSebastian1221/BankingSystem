cmake_minimum_required(VERSION 3.10)
project(BankingSystem)

set(CMAKE_CXX_STANDARD 17)

# Set MySQL include directory
set(MYSQL_INCLUDE_DIR "/usr/include/mysql")

# Add executable
add_executable(bank_app bank_app.cpp Bank.cpp)

# Add include path
target_include_directories(bank_app PRIVATE ${MYSQL_INCLUDE_DIR})

# Let the linker find mysqlclient automatically
target_link_libraries(bank_app mysqlclient)


