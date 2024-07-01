
int pinTemp = A1;   //This is where our Output data goes
int pinH = A2;

void setup() {
  Serial.begin(9600);     
}
void loop() {
  int temp_reading = analogRead(pinTemp); 
  int hum_reading = analogRead(pinH);   //Read the analog pin

  // Get the voltage by converting output into 0-5V scale
  float voltage_temp = (float)temp_reading * 5.0;
  voltage_temp /= 1024.0;

  // Convert voltage to degree Celsius including the 500mV offset adjustment
  float temperatureC = (voltage_temp - 0.5) * 100;  

  // Calculate humidity from voltage * multiplier
  float humidity = hum_reading * 0.0879765395894428;

  //print the temperature status
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println("C");  
  delay(1000); 

  Serial.print("Hum: ");
  Serial.print(humidity);
  Serial.println("%");  //print the temperature status
  delay(1000);  
}