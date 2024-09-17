import { parseISO } from 'date-fns';

import { KatlaSensorPayload } from '../types';

export const isValidSensorData = (
  data: ReturnType<typeof JSON.parse>
): data is KatlaSensorPayload => {
  try {
    parseISO(data.timestamp || '');
  } catch {
    return false;
  }
  return true;
};
