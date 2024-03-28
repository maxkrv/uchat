
CREATE TABLE IF NOT EXISTS user_sticker (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id         INT,
    file_id         INT,
    created_at      INT DEFAULT(strftime('%s', 'now')), 
    edited_at       INT DEFAULT(strftime('%s', 'now')), 
    UNIQUE (user_id, file_id),
    FOREIGN KEY(file_id) REFERENCES file(id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY(user_id) REFERENCES user(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TRIGGER user_sticker_update_edited_at_tr
AFTER UPDATE ON user_sticker
BEGIN
    UPDATE user_sticker SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;