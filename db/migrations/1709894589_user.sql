CREATE TABLE IF NOT EXISTS user (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    name            VARCHAR(255) NOT NULL UNIQUE,
    tag             VARCHAR(255)  UNIQUE DEFAULT(lower(hex(randomblob(16)))),
    password_hash   VARCHAR(255) NOT NULL, 
    photo_id        INT NULL, 
    status          VARCHAR(255), 
    description     VARCHAR(3000), 
    created_at      INT DEFAULT(strftime('%s', 'now')),
    edited_at       INT DEFAULT(strftime('%s', 'now')),
    FOREIGN KEY(photo_id) REFERENCES file(id)  ON DELETE SET NULL ON UPDATE SET NULL
);

CREATE TRIGGER user_update_edited_at_tr
AFTER UPDATE ON user
BEGIN
    UPDATE user SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;

