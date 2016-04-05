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
