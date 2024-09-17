import { Handler } from 'express';
import { Database } from 'sqlite';

import { decryptKatlaData } from '../lib/decrypt';
import { isValidSensorData } from '../lib/utils';

class InvalidSensorPayloadError extends Error {}

export const createInsertSensorDataHandler =
  (db: Database): Handler =>
  async (req, res) => {
    console.log(req.body);
    const decryptedData = decryptKatlaData(req.body);
    console.log('decrypted', decryptedData);

    try {
      let data = JSON.parse(decryptedData);

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
      // If the data is malformed Syntax, the likely issue is that it was
      // encrypted with the wrong key
      if (e instanceof SyntaxError) {
        res.status(401);
      }
      // Invalid payload => Bad request
      else if (e instanceof InvalidSensorPayloadError) {
        res.status(400);
      }
      // Otherwise something went wrong in the server
      else {
        res.status(500);
      }
    }

    res.end();
  };
