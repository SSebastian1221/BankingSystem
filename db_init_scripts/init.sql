CREATE DATABASE IF NOT EXISTS bank_system;
USE bank_system;

-- Create users table
CREATE TABLE IF NOT EXISTS users (
  id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(50) UNIQUE NOT NULL,
  password_hash VARCHAR(255) NOT NULL
);

-- Create accounts table
CREATE TABLE IF NOT EXISTS accounts (
  id INT AUTO_INCREMENT PRIMARY KEY,
  user_id INT NOT NULL,
  account_type INT DEFAULT 0,
  balance DOUBLE DEFAULT 0.0,
  FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE USER IF NOT EXISTS 'admin' IDENTIFIED BY 'password';
GRANT ALL PRIVILEGES ON bank_system.* TO 'admin';
FLUSH PRIVILEGES;
