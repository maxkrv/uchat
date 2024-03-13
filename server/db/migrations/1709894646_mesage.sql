
CREATE TABLE IF NOT EXISTS message (
    id              INTEGER PRIMARY KEY,
    author_id       INTEGER NOT NULL,
    room_id         INTEGER NOT NULL,
    reply_id        INTEGER ,
    word            VARCHAR(255) NOT NULL,
    created_at      INT NOT NULL, 
    edited_at       INT NOT NULL, 
    FOREIGN KEY(author_id) REFERENCES user(id),
    FOREIGN KEY(room_id) REFERENCES room(id)
);

CREATE TABLE IF NOT EXISTS message_reader (
    id              INTEGER PRIMARY KEY,
    reader_id       INTEGER NOT NULL,
    message_id      INTEGER NOT NULL,
    created_at      INT NOT NULL, 
    edited_at       INT NOT NULL, 
    FOREIGN KEY(reader_id) REFERENCES user(id),
    FOREIGN KEY(message_id) REFERENCES message(id)
);

CREATE TABLE IF NOT EXISTS message_file (
    id              INTEGER PRIMARY KEY,
    file_id         INTEGER NOT NULL,
    message_id      INTEGER NOT NULL,
    created_at      INT NOT NULL, 
    edited_at       INT NOT NULL, 
    FOREIGN KEY(file_id) REFERENCES file(id),
    FOREIGN KEY(message_id) REFERENCES message(id)
);

CREATE TABLE IF NOT EXISTS room_pinned_message (
    id              INTEGER PRIMARY KEY,
    room_id         INTEGER NOT NULL,
    message_id      INTEGER NOT NULL,
    created_at      INT NOT NULL, 
    edited_at       INT NOT NULL, 
    FOREIGN KEY(room_id) REFERENCES room(id),
    FOREIGN KEY(message_id) REFERENCES message(id)
);
