create table measurings (
	ID INT PRIMARY KEY NOT NULL,
	time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	temperature DECIMAL(5,1), 
	humidity DECIMAL(5,1)
);	
INSERT INTO measurings (`temperature`, `humidity`) VALUES(' 22.2',' 29.0');