CREATE DATABASE IF NOT EXISTS bank_db;
USE bank_db;

-- Create users table
CREATE TABLE IF NOT EXISTS users (
  id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(50) UNIQUE NOT NULL,
  password VARCHAR(255) NOT NULL
);

-- Create accounts table
CREATE TABLE IF NOT EXISTS accounts (
  id INT AUTO_INCREMENT PRIMARY KEY,
  user_id INT NOT NULL,
  account_type INT DEFAULT 0,
  balance DOUBLE DEFAULT 0.0,
  FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE USER IF NOT EXISTS 'your_db_user' IDENTIFIED BY 'your_db_password';
GRANT ALL PRIVILEGES ON bank_db.* TO 'your_db_user';
FLUSH PRIVILEGES;