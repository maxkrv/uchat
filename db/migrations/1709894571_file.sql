-- Active: 1711027646487@@127.0.0.1@3306
CREATE TABLE IF NOT EXISTS file (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    name            VARCHAR(255) NOT NULL,
    url             VARCHAR(255) NULL,
    created_at      INT DEFAULT(strftime('%s', 'now')),
    edited_at       INT DEFAULT(strftime('%s', 'now'))
);


CREATE TRIGGER file_update_edited_at_tr
AFTER UPDATE ON file
BEGIN
    UPDATE file SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;

