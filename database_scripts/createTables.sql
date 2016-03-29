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