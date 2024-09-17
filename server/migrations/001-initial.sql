-- Up
CREATE TABLE katla_sensor_data (
    id INTEGER PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    temp_in REAL,
    temp_out REAL,
    humidity REAL
);
