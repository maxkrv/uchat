-- Active: 1711025793902@@127.0.0.1@3306
CREATE TABLE IF NOT EXISTS room (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    name            VARCHAR(255) NOT NULL,
    photo_id        INT NULL,
    type            VARCHAR(30) DEFAULT 'direct',
    description     VARCHAR(255), 
    created_at      INT DEFAULT(strftime('%s', 'now')),
    edited_at       INT DEFAULT(strftime('%s', 'now')),
    FOREIGN KEY(photo_id) REFERENCES file(id) ON DELETE SET NULL ON UPDATE SET NULL
);

CREATE TRIGGER room_update_edited_at_tr
AFTER UPDATE ON room
BEGIN
    UPDATE room SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS room_member (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id         INT NOT NULL,
    room_id         INT NOT NULL,
    is_admin        INT DEFAULT 0,
    created_at      INT DEFAULT(strftime('%s', 'now')),
    edited_at       INT DEFAULT(strftime('%s', 'now')),
    UNIQUE (user_id, room_id),
    FOREIGN KEY(user_id) REFERENCES user(id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY(room_id) REFERENCES room(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TRIGGER room_member_update_edited_at_tr
AFTER UPDATE ON room_member
BEGIN
    UPDATE room_member SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS favourite_room (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    room_id         INT NOT NULL,
    user_id         INT NOT NULL,
    created_at      INT DEFAULT(strftime('%s', 'now')), 
    edited_at       INT DEFAULT(strftime('%s', 'now')), 
    UNIQUE (room_id, user_id),
    FOREIGN KEY(user_id) REFERENCES user(id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY(room_id) REFERENCES room(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TRIGGER favourite_room_update_edited_at_tr
AFTER UPDATE ON favourite_room
BEGIN
    UPDATE favourite_room SET edited_at = strftime('%s', 'now') WHERE id = NEW.id;
END;

