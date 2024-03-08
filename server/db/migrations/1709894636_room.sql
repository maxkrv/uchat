CREATE TABLE IF NOT EXISTS room (
    id              INTEGER PRIMARY KEY,
    name            VARCHAR(255) NOT NULL UNIQUE,
    photo_id        VARCHAR(255)  NULL,
    category        VARCHAR(30) DEFAULT 'DIRECT',
    description     VARCHAR(255), 
    created_at      INT NULL, 
    edited_at       INT NULL
);
CREATE TABLE IF NOT EXISTS room_member (
    id              INTEGER PRIMARY KEY,
    user_id         INTEGER NOT NULL,
    room_id         INTEGER NOT NULL,
    is_admin        INTEGER DEFAULT 0,
    created_at      INT NOT NULL, 
    edited_at       INT NOT NULL, 
    FOREIGN KEY(user_id) REFERENCES user(id),
    FOREIGN KEY(room_id) REFERENCES room(id)
);
CREATE TABLE IF NOT EXISTS favourite_room (
    id              INTEGER PRIMARY KEY,
    room_id         INTEGER NOT NULL,
    user_id         INTEGER NOT NULL,
    created_at      INT NOT NULL, 
    edited_at       INT NOT NULL, 
    FOREIGN KEY(room_id) REFERENCES room(id),
    FOREIGN KEY(user_id) REFERENCES user(id)
);
