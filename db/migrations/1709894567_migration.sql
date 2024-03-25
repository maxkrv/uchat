PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS migration (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    name       TEXT NOT NULL UNIQUE ,
    was_run    INT DEFAULT 1,
    created_at INT DEFAULT(strftime('%s'))
);

