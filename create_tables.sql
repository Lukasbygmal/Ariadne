-- to start server: sudo service mysql start
--mysql -u root -p
--USE ariadne;
--SELECT * FROM Users;
-- If problem with root not being accessed
-- sudo mysql
--ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '';
--FLUSH PRIVILEGES;

CREATE TABLE Users (user_id INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(255) NOT NULL, lvl INT NOT NULL, xp INT NOT NULL, gold INT NOT NULL, max_hp INT NOT NULL, strength_stat INT NOT NULL, agility_stat INT NOT NULL, armor_stat INT NOT NULL);
INSERT INTO Users (name, lvl, xp, gold, max_hp, strength_stat, agility_stat, armor_stat) VALUES
('Aragorn', 1, 50, 100, 100, 75, 65, 60),
('Legolas', 1, 75, 85, 90, 60, 80, 50),
('Gimli', 1, 25, 120, 110, 85, 55, 75),
('Frodo', 1, 30, 75, 80, 50, 70, 40),
('Samwise', 1, 40, 90, 95, 60, 60, 55);
--CREATE TABLE Items (item_id INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(255) NOT NULL, type VARCHAR(50) NOT NULL, attributes JSON);
--CREATE TABLE Equipment (equipment_id INT PRIMARY KEY AUTO_INCREMENT, user_id INT, item_id INT, slot VARCHAR(50), FOREIGN KEY (user_id) REFERENCES Users(user_id), FOREIGN KEY (item_id) REFERENCES Items(item_id), UNIQUE (user_id, slot));
--CREATE TABLE Inventory (inventory_id INT PRIMARY KEY AUTO_INCREMENT, user_id INT, item_id INT, slot INT, FOREIGN KEY (user_id) REFERENCES Users(user_id), FOREIGN KEY (item_id) REFERENCES Items(item_id), UNIQUE (user_id, slot));
