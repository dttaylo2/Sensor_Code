//make sure the port the arduino is assigned to in the IDE matches correctly

const int analogIn0 = 0;   //used to select which pin to read from
const int analogIn2 = 2;   //used to select which pin to read from
const int analogIn5 = 5;   //used to select which pin to read from

int rawBuffer0[10];
float voltageBuffer0[10];

int rawBuffer2[10];
float voltageBuffer2[10];

int rawBuffer5[10];
float voltageBuffer5[10];

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
}

void loop() {
  
//this loop takes 10 raw data readings and stores them in buffer
  for (int i = 0; i<10 ;i ++){
    rawBuffer0[i] = analogRead(analogIn0);
    voltageBuffer0[i] = rawBuffer0[i] * (5.0 / 1023.0);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):

    rawBuffer2[i] = analogRead(analogIn2);
    voltageBuffer2[i] = rawBuffer2[i] * (5.0 / 1023.0);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):

    rawBuffer5[i] = analogRead(analogIn5);
    voltageBuffer5[i] = rawBuffer5[i] * (5.0 / 1023.0);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  }

//this loop prints the stored raw data readings
  for (int j = 0; j<10 ; j++){
    Serial.print(rawBuffer0[j]);
    Serial.print(", ");
    delay(100);
  }
  
  Serial.println("");   //new line for next print sequence
  delay(1000);

//this loop prints the stored converted data readings
  for (int k = 0; k<10 ; k++){
    Serial.print(voltageBuffer0[k]);
    Serial.print("V, ");
    delay(100);
  }
  
 Serial.println("");    //new line for next print sequence
  delay(2000);

  //this loop prints the stored raw data readings
  for (int j = 0; j<10 ; j++){
    Serial.print(rawBuffer2[j]);
    Serial.print(", ");
    delay(100);
  }
  
  Serial.println("");   //new line for next print sequence
  delay(1000);

//this loop prints the stored converted data readings
  for (int k = 0; k<10 ; k++){
    Serial.print(voltageBuffer2[k]);
    Serial.print("V, ");
    delay(100);
  }
  
 Serial.println("");    //new line for next print sequence
  delay(2000);

  //this loop prints the stored raw data readings
  for (int j = 0; j<10 ; j++){
    Serial.print(rawBuffer5[j]);
    Serial.print(", ");
    delay(100);
  }
  
  Serial.println("");   //new line for next print sequence
  delay(1000);

//this loop prints the stored converted data readings
  for (int k = 0; k<10 ; k++){
    Serial.print(voltageBuffer5[k]);
    Serial.print("V, ");
    delay(100);
  }
  
 Serial.println("");    //new line for next print sequence
  delay(2000);
}

