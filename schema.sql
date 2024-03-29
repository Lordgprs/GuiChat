BEGIN;
DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS users;
CREATE TABLE users (
	id SERIAL NOT NULL PRIMARY KEY,
	name VARCHAR(32) NOT NULL UNIQUE,
	password_hash VARCHAR(256) NOT NULL,
	active BOOLEAN NOT NULL DEFAULT TRUE
);

CREATE TABLE messages (
	id SERIAL NOT NULL PRIMARY KEY,
	sender_id INTEGER NOT NULL REFERENCES users(id)
		ON DELETE CASCADE ON UPDATE CASCADE,
	receiver_id INTEGER REFERENCES users(id)
		ON DELETE CASCADE ON UPDATE CASCADE,
	message_text TEXT,
	sent TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP
);
COMMIT;


