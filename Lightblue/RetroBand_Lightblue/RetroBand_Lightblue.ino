/*
  ColorsExample
  For use with the "Handy BLE" iPhone app by Paul Shelley and 
  the LightBlue Bean by Punchthrough Design.
  This sketch receives Serial data with a simple packet format
  of '#' for a start character and ';' as an end character. Start
  and end characters can be changed easily. Simple error checking 
  is also included. 
  
  LightBlueBean - https://punchthrough.com
*/

/* Data buffer */
#define ACCEL_BUFFER_COUNT 125
byte aAccelBuffer[ACCEL_BUFFER_COUNT];
int iAccelIndex = 2;

void setup() {
  Serial.begin(57600);
}

void loop(){
  curSensoredTime = millis();
  
  // Read from sensor
  if(curSensoredTime - prevSensoredTime > SENSOR_READ_INTERVAL) {
    readFromSensor();  // Read from sensor
    prevSensoredTime = curSensoredTime;
    
    // Send buffer data to remote
    if(iAccelIndex >= ACCEL_BUFFER_COUNT - 3) {
      sendToRemote();
      Serial.println("------------- Send 20 accel data to remote");
    }
  }
  Bean.sleep(50);
}

void readFromSensor(){

	// Get the current acceleration with range of ±2g, 
	// and a conversion of 3.91×10-3 g/unit or 0.03834(m/s^2)/units. 
	AccelerationReading accel = Bean.getAcceleration();

	if(iAccelIndex < ACCEL_BUFFER_COUNT && iAccelIndex > 1) {
		uint16_t tempX = (abs(accel.xAxis));
		uint16_t tempY = (abs(accel.yAxis));
		uint16_t tempZ = (abs(accel.zAxis));

		char temp = (char)(tempX >> 8);
		if(temp == 0x00){
			temp = 0x7f;
		}
		aAccelBuffer[iAccelIndex] = temp;
		iAccelIndex++;
		temp = (char)(tempX);
		if(temp == 0x00){
			temp = 0x01;
		}
		aAccelBuffer[iAccelIndex] = temp;
		iAccelIndex++;

		temp = (char)(tempY >> 8);
		if(temp == 0x00){
			temp = 0x7f;
		}
		aAccelBuffer[iAccelIndex] = temp;
		iAccelIndex++;
		temp = (char)(tempY);
		if(temp == 0x00){
			temp = 0x01;
		}
		aAccelBuffer[iAccelIndex] = temp;
		iAccelIndex++;

		temp = (char)(tempZ >> 8);
		if(temp == 0x00){
			temp = 0x7f;
		}
		aAccelBuffer[iAccelIndex] = temp;
		iAccelIndex++;
		temp = (char)(tempZ);
		if(temp == 0x00){
			temp = 0x01;
		}

		aAccelBuffer[iAccelIndex] = temp;
		iAccelIndex++;
	}	
}

/**************************************************
 * BT Transaction
 **************************************************/
void sendToRemote() {
  // Write gabage bytes
  Serial.write( "accel" );
  // Write accel data
  Serial.write( (char*)aAccelBuffer );
  // Flush buffer
  //BTSerial.flush();
}