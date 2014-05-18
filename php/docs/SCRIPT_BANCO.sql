DROP DATABASE monitora_servidor;

CREATE DATABASE monitora_servidor;

use monitora_servidor;

CREATE TABLE leituras
(
	leit_id int NOT NULL auto_increment  PRIMARY KEY,
	leit_data timestamp ,
	leit_valor int,
	leit_tipo char(1)
);

CREATE index idx_leituras_data on LEITURAS(leit_data);
CREATE index idx_leituras_data_tipo on LEITURAS(leit_data,leit_tipo);