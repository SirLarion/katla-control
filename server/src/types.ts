// Sensor data collected by Katla. The values need to be nullable so as not to
// invalidate the entire payload if one value is null
export type KatlaSensorPayload = {
  secret: string;
  temp_in?: number;
  temp_out?: number;
  humidity?: number;
};
