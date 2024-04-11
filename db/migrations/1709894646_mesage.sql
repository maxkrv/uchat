
CREATE TABLE IF NOT EXISTS message (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    author_id       INT NOT NULL,
    room_id         INT NOT NULL,
    reply_id        INT NULL,
    data            VARCHAR(5000) NULL,
    created_at      INT DEFAULT(strftime('%s', 'now')), 
    edited_at       INT DEFAULT(strftime('%s', 'now')), 
    FOREIGN KEY(author_id) REFERENCES user(id) ON DELETE SET NULL ON UPDATE CASCADE,
    FOREIGN KEY(room_id) REFERENCES room(id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY(reply_id) REFERENCES message(id) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TRIGGER message_update_edited_at_tr
AFTER UPDATE ON message
BEGIN
    UPDATE message SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS message_reader (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    reader_id       INT NOT NULL,
    message_id      INT NOT NULL,
    created_at      INT DEFAULT(strftime('%s', 'now')), 
    edited_at       INT DEFAULT(strftime('%s', 'now')), 
    UNIQUE (reader_id, message_id),
    FOREIGN KEY(reader_id) REFERENCES user(id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY(message_id) REFERENCES message(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TRIGGER message_reader_update_edited_at_tr
AFTER UPDATE ON message_reader
BEGIN
    UPDATE message_reader SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS message_file (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    file_id         INT,
    message_id      INT,
    created_at      INT DEFAULT(strftime('%s', 'now')), 
    edited_at       INT DEFAULT(strftime('%s', 'now')), 
    UNIQUE (file_id, message_id),
    FOREIGN KEY(file_id) REFERENCES file(id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY(message_id) REFERENCES message(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TRIGGER message_file_update_edited_at_tr
AFTER UPDATE ON message_file
BEGIN
    UPDATE message_file SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS room_pinned_message (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    room_id         INT NOT NULL,
    message_id      INT NOT NULL UNIQUE,
    created_at      INT DEFAULT(strftime('%s', 'now')), 
    edited_at       INT DEFAULT(strftime('%s', 'now')), 
    UNIQUE (message_id, room_id),
    FOREIGN KEY(room_id) REFERENCES room(id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY(message_id) REFERENCES message(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TRIGGER room_pinned_message_update_edited_at_tr
AFTER UPDATE ON room_pinned_message
BEGIN
    UPDATE room_pinned_message SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;

