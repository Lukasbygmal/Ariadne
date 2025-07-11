CREATE TABLE Users (
    user_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    lvl INTEGER NOT NULL DEFAULT 1,
    xp INTEGER NOT NULL DEFAULT 0,
    gold INTEGER NOT NULL DEFAULT 0,
    max_hp INTEGER NOT NULL DEFAULT 100,
    strength_stat INTEGER NOT NULL DEFAULT 10,
    agility_stat INTEGER NOT NULL DEFAULT 10,
    armor_stat INTEGER NOT NULL DEFAULT 10,
   
    -- OAuth fields (GitHub only)
    github_id BIGINT UNIQUE,
    github_username VARCHAR(255),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);