SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='';


-- -----------------------------------------------------
-- Table `birch`.`Study`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `birch`.`Study` ;

CREATE  TABLE IF NOT EXISTS `birch`.`Study` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `update_timestamp` TIMESTAMP NOT NULL ,
  `create_timestamp` TIMESTAMP NOT NULL ,
  `uid` VARCHAR(45) NOT NULL ,
  `site` VARCHAR(45) NOT NULL ,
  `interviewer` VARCHAR(45) NOT NULL ,
  `datetime_acquired` DATETIME NOT NULL ,
  `note` TEXT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `dk_uid` (`uid` ASC) ,
  INDEX `dk_site` (`site` ASC) ,
  INDEX `dk_datetime_acquired` (`datetime_acquired` ASC) ,
  INDEX `dk_interviewer` (`interviewer` ASC) ,
  UNIQUE INDEX `uq_uid` (`uid` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `birch`.`Image`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `birch`.`Image` ;

CREATE  TABLE IF NOT EXISTS `birch`.`Image` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `update_timestamp` TIMESTAMP NOT NULL ,
  `create_timestamp` TIMESTAMP NOT NULL ,
  `study_id` INT UNSIGNED NOT NULL ,
  `laterality` ENUM('left','right') NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_study_id` (`study_id` ASC) ,
  CONSTRAINT `fk_image_study_id`
    FOREIGN KEY (`study_id` )
    REFERENCES `birch`.`Study` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `birch`.`User`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `birch`.`User` ;

CREATE  TABLE IF NOT EXISTS `birch`.`User` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `update_timestamp` TIMESTAMP NOT NULL ,
  `create_timestamp` TIMESTAMP NOT NULL ,
  `name` VARCHAR(255) NOT NULL ,
  `password` VARCHAR(255) NOT NULL ,
  `last_login` DATETIME NULL ,
  `study_id` INT UNSIGNED NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `uq_name` (`name` ASC) ,
  INDEX `dk_last_login` (`last_login` ASC) ,
  INDEX `fk_study_id` (`study_id` ASC) ,
  CONSTRAINT `fk_user_study_id`
    FOREIGN KEY (`study_id` )
    REFERENCES `birch`.`Study` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `birch`.`Rating`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `birch`.`Rating` ;

CREATE  TABLE IF NOT EXISTS `birch`.`Rating` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `update_timestamp` TIMESTAMP NOT NULL ,
  `create_timestamp` TIMESTAMP NOT NULL ,
  `image_id` INT UNSIGNED NOT NULL ,
  `user_id` INT UNSIGNED NOT NULL ,
  `rating` TINYINT(1) NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_rating_image_id` (`image_id` ASC) ,
  INDEX `fk_rating_user_id` (`user_id` ASC) ,
  INDEX `dk_rating` (`rating` ASC) ,
  CONSTRAINT `fk_rating_image_id`
    FOREIGN KEY (`image_id` )
    REFERENCES `birch`.`Image` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_rating_user_id`
    FOREIGN KEY (`user_id` )
    REFERENCES `birch`.`User` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
