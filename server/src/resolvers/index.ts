import { Handler } from 'express';
import { Database } from 'sqlite';

import { isValidSensorData } from '../lib/utils';

class InvalidSensorPayloadError extends Error {}

export const createInsertSensorDataHandler =
  (db: Database): Handler =>
  async (req, res) => {
    try {
      let data = req.body;

      if (!isValidSensorData(data)) {
        throw new InvalidSensorPayloadError(
          'sensor payload has incorrect format'
        );
      }

      await db.run(
        '\
        INSERT INTO katla_sensor_data (\
            temp_in, temp_out, humidity\
        ) VALUES (\
            ?1, ?2, ?3\
        );',
        data.temp_in,
        data.temp_out,
        data.humidity
      );
    } catch (e) {
      // Invalid payload => Bad request
      if (e instanceof InvalidSensorPayloadError) {
        res.status(400);
      }
      // Otherwise something went wrong in the server
      else {
        res.status(500);
      }
    }

    res.end();
  };
