// Libraries
#include <SoftwareSerial.h>
#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // Temp sensor library
#include <SPI.h>
#include <TimerOne.h>
#include "nRF24L01.h"
#include "RF24.h"

// Timer
const byte M_TICK = 2;
const int DEBOUNCE_DELAY = 10;
volatile const long TIMER_PER_uS = 1000000;
const unsigned int PULSE_REV = 4;
volatile int tickCounter = 0;
volatile int tickCounter1 = 0;
volatile int tickCounter2 = 0;
volatile int rpmArray[5] = {0};
volatile int index = 0;
volatile unsigned long time = 100;
volatile unsigned long rpm = 0;
// Define radio pins
#define RF_CE 7
#define RF_CSN 8

const int sensorPin = A0;
const float baselineVibe = 0;

// Radio pipe addresses for the 2 nodes to communicate
const uint64_t pipes[2] = {
  0xF0F0F0F0E1LL,
  0xF0F0F0F0D2LL
};

// Data size we are sending
const int dataSize = 30;

// Create an IRTherm object to interact with
IRTherm therm;

// Start radio
RF24 radio(RF_CE, RF_CSN);

// Define values to be read
double current;
double temperatureSum;
double temperatureAverage;
double vibrationSum;
double vibrationAverage;
double acclrSum;
double acclrAvg;

// Read 16 times for average.
int numDataReads = 8;
int loopCounter;

// Char array for data to be sent out
char radioTX[dataSize];

// Values used for reading Irms
int inPinI = 1;
int sampleI;
double filteredI;
double offsetI = 512; 
double sqI;
double sumI;
double ICAL = 10;
double Irms;

double Hz;


//accelerometer
int inPinAcclr = 2;
double acclrRead;



unsigned long time1;
unsigned long readTime;

void setup() {
  
  // Start temperature sensor
  therm.begin();
  therm.setUnit(TEMP_C);

  // Start radio with dnamic payloads.
  // Write to pipe 0, read on pipe 1.
  radio.begin();
  radio.enableDynamicPayloads();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.stopListening();

  time1 = 0;
  readTime = 0;

  // Begin serial
  Serial.begin(9600);
  
  attachInterrupt(0, motorTick, RISING);
  Timer1.initialize(TIMER_PER_uS);
  Timer1.attachInterrupt(computeRPM);
}

void loop() {


  
  // Reset values
  current = 0.0;
  temperatureSum = 0.0;
  vibrationSum = 0.0;
  acclrSum = 0.0;

  time1 = millis();  //take time reading before a sensor read cycle

  current = calcIrms(1480);
  
  //readTime = millis() - time1;  //compare sensor read cycle start time with current time
  //Serial.println(readTime);  //print the sensor read cycle time duration
  //Serial.println();

  //time1 = millis();  //take time reading before a sensor read cycle

  for(loopCounter = 0; loopCounter < numDataReads; loopCounter++) {
      
    // Call therm.read() to read object and ambient temperatures from the sensor.
    therm.read();
    temperatureSum += therm.object();
  }

  //readTime = millis() - time1;  //compare sensor read cycle start time with current time
  //Serial.println(readTime);  //print the sensor read cycle time duration
  //Serial.println();
  
  //time1 = millis();  //take time reading before a sensor read cycle
  
  for(loopCounter = 0; loopCounter < numDataReads; loopCounter++) {
    vibrationSum += (analogRead(sensorPin)) * 0.175;
  }

  //readTime = millis() - time1;  //compare sensor read cycle start time with current time  
  //Serial.println(readTime);  //print the sensor read cycle time duration
  //Serial.println();
  
  
  for (loopCounter = 0; loopCounter < numDataReads; loopCounter++) {
    acclrSum += (analogRead(inPinAcclr));
  }
  
  // Get Average values from temp and vibration
  temperatureAverage = temperatureSum / numDataReads;
  vibrationAverage = vibrationSum / numDataReads;
  acclrAvg = acclrSum / numDataReads;

  // Populate them.
  // Use four bytes to display current to a 2 decimal place precision.
  dtostrf(current, 5, 2, &radioTX[0]);
  // Put a comma after current
  radioTX[6] = ',';
  // Use five bytes with 2 decimal places for temperature
  dtostrf(temperatureAverage, 5, 2, &radioTX[7]);
  
  radioTX[12] = ',';
  
  dtostrf(vibrationAverage, 5, 2 , &radioTX[13]);

  radioTX[18] = ',';

  dtostrf(rpm, 5, 0 , &radioTX[19]);

  radioTX[24] = ',';

  dtostrf(acclrAvg, 5, 0 , &radioTX[25]);

  //readTime = millis() - time1;  //compare sensor read cycle start time with current time  
  //Serial.println(readTime);  //print the sensor read cycle time duration
  //Serial.println();
  
  //Transmit data
  radio.write(&radioTX, dataSize);
  
  /*Serial.write(radioTX, dataSize);
  Serial.println(readTime);
  Serial.println();*/

  // Delay
  delay(100);
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
void motorTick()
{ 
  if ((micros() - time) < DEBOUNCE_DELAY)
    return;

  for (int k = 0; k < 3; ++k)
    if (digitalRead(M_TICK) != HIGH)
      return;
  
  ++tickCounter;

  time = micros();
}

void computeRPM()
{
  switch (index)
  {
    case 0:
      rpmArray[0] = tickCounter * (TIMER_PER_uS / 1000000.) * (60. / PULSE_REV);
      ++index;
    break;
    
    case 1:
      rpmArray[1] = tickCounter * (TIMER_PER_uS / 1000000.) * (60. / PULSE_REV);
      ++index;
      break;
    
    case 2:
      rpmArray[2] = tickCounter * (TIMER_PER_uS / 1000000.) * (60. / PULSE_REV);
      ++index;
    break;

    case 3:
      rpmArray[3] = tickCounter * (TIMER_PER_uS / 1000000.) * (60. / PULSE_REV);
      ++index;
    break;
    
    case 4:
      rpmArray[4] = tickCounter * (TIMER_PER_uS / 1000000.) * (60. / PULSE_REV);
      index = 0;
    break;

  }

  tickCounter = 0;

  rpm = (rpmArray[0] + rpmArray[1] + rpmArray[2] + rpmArray[3] + rpmArray[4]) / 5;
}

//-----------------------------------------------------------------------------------
