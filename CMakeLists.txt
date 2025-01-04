cmake_minimum_required(VERSION 3.10)
project(BankingSystem)

# Set the MySQL configuration manually since find_package(MySQL) is unreliable
set(MYSQL_INCLUDE_DIR "/usr/include/mysql")
set(MYSQL_LIBRARY "/usr/lib/x86_64-linux-gnu/libmysqlclient.so")

# Add the executable
add_executable(bank_app bank_app.cpp)

# Include MySQL headers
target_include_directories(bank_app PRIVATE ${MYSQL_INCLUDE_DIR})

# Link against MySQL library
target_link_libraries(bank_app PRIVATE ${MYSQL_LIBRARY})