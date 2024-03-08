CREATE TABLE IF NOT EXISTS file (
    id              INTEGER PRIMARY KEY,
    name            VARCHAR(255) NOT NULL,
    url             VARCHAR(255) NULL,
    created_at      INT NOT NULL,
    edited_at       INT NOT NULL
);
