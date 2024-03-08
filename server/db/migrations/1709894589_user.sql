CREATE TABLE IF NOT EXISTS user (
    id              INTEGER PRIMARY KEY,
    name            VARCHAR(255) NOT NULL UNIQUE,
    tag             VARCHAR(255) NOT NULL DEFAULT (lower(hex(randomblob(16)))) UNIQUE,
    password_hash   VARCHAR(255) NOT NULL, 
    photo_id        VARCHAR(255) NULL, 
    status          VARCHAR(255), 
    description     VARCHAR(3000), 
    created_at      INT NOT NULL, 
    edited_at       INT NOT NULL, 
    FOREIGN KEY(photo_id) REFERENCES file(id)
);
