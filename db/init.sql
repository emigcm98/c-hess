CREATE USER 'admin'@'%' IDENTIFIED BY 'admin';
GRANT ALL PRIVILEGES ON chessdb.* TO 'admin'@'%';
CREATE DATABASE chessdb;
USE chessdb;

CREATE TABLE users (
  id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(255) NOT NULL UNIQUE,
  token VARCHAR(255) NOT NULL,
  password VARCHAR(255) NOT NULL,
  elo DECIMAL(5,1) NOT NULL DEFAULT 1200.0,
  name VARCHAR(255) NOT NULL,
  birthdate DATE NOT NULL,
  email VARCHAR(255) NOT NULL UNIQUE,
  last_login TIMESTAMP,
  status ENUM('ACTIVE', 'INACTIVE') NOT NULL DEFAULT 'ACTIVE',
  avatar VARCHAR(255),
  gender ENUM('M', 'F', 'Other') NOT NULL,
  country VARCHAR(255),
  creationdate TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE games (
  id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  whiteplayer INT NOT NULL,
  blackplayer INT NOT NULL,
  whiteelo INT NOT NULL,
  blackelo INT NOT NULL,
  pgn TEXT NOT NULL,
  creationdate TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  result ENUM('1-0', '0-1', '1/2-1/2') NOT NULL,
  -- elo_diff INT GENERATED ALWAYS AS (whiteelo - blackelo) STORED,
  elo_diff INT NOT NULL,
  time_control VARCHAR(255) NOT NULL,
  opening VARCHAR(255) NOT NULL,
  FOREIGN KEY (whiteplayer) REFERENCES users(id),
  FOREIGN KEY (blackplayer) REFERENCES users(id)
);