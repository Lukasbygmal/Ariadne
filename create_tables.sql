-- to start server: sudo service mysql start
--mysql -u root -p
--USE ariadne;
--SELECT * FROM Users;
-- If problem with root not being accessed
-- sudo mysql
--ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '';
--FLUSH PRIVILEGES;

CREATE TABLE Users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255) NOT NULL,
    lvl INT NOT NULL DEFAULT 1,
    xp INT NOT NULL DEFAULT 0,
    gold INT NOT NULL DEFAULT 0,
    max_hp INT NOT NULL DEFAULT 100,
    strength_stat INT NOT NULL DEFAULT 10,
    agility_stat INT NOT NULL DEFAULT 10,
    armor_stat INT NOT NULL DEFAULT 10,
    
    -- OAuth fields
    github_id BIGINT UNIQUE,
    github_username VARCHAR(255),
    email VARCHAR(255),
    avatar_url TEXT,
    provider ENUM('github', 'discord', 'local') DEFAULT 'local',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_login TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
INSERT INTO Users (name, lvl, xp, gold, max_hp, strength_stat, agility_stat, armor_stat, provider) 
VALUES ('Aragorn', 1, 50, 100, 100, 75, 65, 60, 'local');