CREATE TABLE IF NOT EXISTS migration (
    id         INTEGER PRIMARY KEY,
    name       TEXT NOT NULL UNIQUE ,
    was_run    INTEGER NOT NULL DEFAULT 1,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);