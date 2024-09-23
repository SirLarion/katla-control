import { KatlaSensorPayload } from '../types';

const validateSharedSecret = (secret: string) => {
  if (process.env.CONTROLLER_SECRET !== secret) {
    throw Error('invalid secret');
  }
};

export const isValidSensorData = (
  data: ReturnType<typeof JSON.parse>
): data is KatlaSensorPayload => {
  try {
    validateSharedSecret(data.secret || '');
  } catch {
    return false;
  }
  return true;
};
