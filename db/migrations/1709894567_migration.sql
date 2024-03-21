-- Active: 1711027646487@@127.0.0.1@3306
CREATE TABLE IF NOT EXISTS migration (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    name       TEXT NOT NULL UNIQUE ,
    was_run    INT NOT NULL DEFAULT 1,
    created_at INT DEFAULT(strftime('%s'))
);
