#include <SoftwareSerial.h>
#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // Temp Sensor : SparkFunMLX90614 Arduino library
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define RF_CE 7
#define RF_CSN 8
#define SWITCH_PIN 2

// Radio pipe addresses for 2 nodes to communicate.
const uint64_t pipes[2] = {
  0xF0F0F0F0E1LL,
  0xF0F0F0F0D2LL
};

const int max_payload_size = 12;
int val = 0;

//SoftwareSerial serialComm(0, 1); // RX, TX

IRTherm therm; // Create an IRTherm object to interact with throughout

RF24 radio(RF_CE, RF_CSN);

int inPinI = 1;  //
int sampleI;
double filteredI;
double offsetI = 512; 
double sqI;
double sumI;
double ICAL = 11.3;
double Irms;

//value of port used by sensor
const int sensor2 = 2;
const int sensor3 = 3;


//raw data buffers
int rawBuffer0[10];
int rawBuffer1[10];
int rawBuffer2[10];
int rawBuffer3[10];

//real valued buffers
double realDataBuffer0[10];
double realDataBuffer1[10];
float realDataBuffer2[10];
float realDataBuffer3[10];

double avgVal0;
double avgVal1;
float avgVal2;
float avgVal3;

//char strConv0;
//char strConv1;
char out0[5];
char out1[5];
char radioTX[12];

String timeStamp;

void setup() {

    
  // Setup switch input
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);
  
//Clear the average values variables
  avgVal0 = 0;
  avgVal1 = 0;
  avgVal2 = 0;
  avgVal3 = 0;
  

  //start temp sensor
  therm.begin(); // Initialize thermal IR sensor
  therm.setUnit(TEMP_C); // Set the library's units to Celcius with TEMP_C


  // Start radio
  radio.begin();

  // Enable dynamic payloads
  radio.enableDynamicPayloads();

  // Write to pipe 0
  radio.openWritingPipe(pipes[0]);

  // Read on pipe 1.
  radio.openReadingPipe(1, pipes[1]);
  radio.stopListening();

  Serial.begin(9600);
  while (!Serial) {
   ; // wait for serial port to connect. Needed for native USB port only
  }

}

void loop() {

//Collect raw data readings 
 for (int i=0 ; i<10 ; i++){

    //Collect raw data readings 
      //Call therm.read() to read object and ambient temperatures from the sensor.
        therm.read();
      //rawBuffer2[i] = analogRead(sensor2);
      //rawBuffer3[i] = analogRead(sensor3);
     delay(50);
//Converted raw data
    realDataBuffer0[i] = calcIrms(1480);  // Calculate Irms
    realDataBuffer1[i] = therm.object();
//  realDataBuffer2[i] = rawBuffer2[i] * (5.0 / 1023.0);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V);
//  realDataBuffer3[i] = rawBuffer3[i] * (5.0 / 1023.0);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V);
delay(50);
}

avgVal0 = 0;
avgVal1 = 0;

//Get average values from buffers
for (int j=0 ; j<10 ; j++){
  avgVal0 = avgVal0 + realDataBuffer0[j];

}

  avgVal0 = avgVal0 / 10;

for (int j=0 ; j<10 ; j++){
  avgVal1 = avgVal1 + realDataBuffer1[j];
}
  avgVal1 = avgVal1 / 10;

/*
for (int j=0 ; j<10 ; j++){
  avgVal2 = avgVal2 + realDataBuffer2[j];
}

for (int j=0 ; j<10 ; j++){
  avgVal3 = avgVal3 + realDataBuffer3[j];
}*/





/*
dtostrf(avgVal0 , 6 , 2 ,out0);
dtostrf(avgVal1 , 6 , 2 ,out1);


for (int i = 0; i<6 ; i++){
  radioTX[i] = out0[i];
  Serial.println(out0[i]);
}

radioTX[6] = ',';

for (int j = 7; j<12 ; j++){
  radioTX[j] = out1[j];
}*/
dtostrf(avgVal0, 4, 2, &radioTX[0]);
radioTX[6] = ',';
dtostrf(avgVal1, 5, 2, &radioTX[7]);

   
//transmit data to RasPi
radio.write(&radioTX, max_payload_size);

  Serial.println(avgVal0);  //current
  Serial.println(avgVal1);  //temp
  
  for (int i =0; i<12 ; i++){
    
  Serial.print(radioTX[i]);
  
  }
    Serial.println("");
    
    delay(2000);
    
  //serialComm.write(avgVal2);  //vibration
  //serialComm.write(avgVal3);  //speed
}

//-----------------------------------------------------------------

double calcIrms(unsigned int Number_of_Samples)
{   
  int SupplyVoltage = readVcc();
   
  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    sampleI = analogRead(inPinI);

    // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset, 
  //  then subtract this - signal is now centered on 0 counts.
    offsetI = (offsetI + (sampleI-offsetI)/1024);
  filteredI = sampleI - offsetI;

    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum 
    sumI += sqI;
  }

  double I_RATIO = ICAL *((SupplyVoltage/1000.0) / (1024));
  Irms = I_RATIO * sqrt(sumI / Number_of_Samples); 

  //Reset accumulators
  sumI = 0;
  
  return Irms;
}

//------------------------------------------------------------------

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

//-----------------------------------------------------------------
