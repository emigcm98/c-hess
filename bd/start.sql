CREATE TABLE IF NOT EXISTS `User` (
	`username` VARCHAR(16) NOT NULL COMMENT 'Username',
	`password` VARCHAR(32) NOT NULL,
	`elo` INT(4) unsigned NOT NULL DEFAULT '1200',
	`name` VARCHAR(32),
	`birthdate` DATE NOT NULL,
	`creation_date` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (`username`)
);

