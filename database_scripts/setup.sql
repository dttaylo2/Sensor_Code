DROP DATABASE IF EXISTS sensorData;
CREATE DATABASE seniordesign;
USE seniordesign;

CREATE TABLE sensorData(
	id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	temperatureVal DOUBLE NOT NULL,
	currentVal DOUBLE NOT NULL,
	vibrationVal DOUBLE NOT NULL,
	rpmVal BIGINT NOT NULL,
	receivedTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE classifications(
	id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    description VARCHAR(50) UNIQUE);
    
CREATE TABLE sensorClassifications(
	sensorId INT NOT NULL,
    classificationId INT NOT NULL,
    accuracyRate DOUBLE NOT NULL,
    FOREIGN KEY (sensorId) REFERENCES sensorData(id),
    FOREIGN KEY (classificationId) REFERENCES classifications(id));

DELIMITER $$

CREATE PROCEDURE `sp_getAllData`(
	IN p_rowCount INTEGER
)
BEGIN
	SELECT tmp.*
	FROM(
		SELECT *
		FROM sensorData
		ORDER BY receivedTime DESC
		LIMIT p_rowCount
	) AS tmp
	ORDER BY tmp.receivedTime;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE `sp_getDates`(
)
BEGIN
	SELECT DISTINCT DATE(receivedTime)
	FROM sensorData
	ORDER BY DATE(receivedTime) DESC;
END $$

DELIMITER ;