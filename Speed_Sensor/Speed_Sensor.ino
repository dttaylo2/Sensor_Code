#include <TimerOne.h>

const byte M_TICK = 2;
const int DEBOUNCE_DELAY = 10;
volatile const long TIMER_PER_uS = 1000000;
const unsigned int PULSE_REV = 8;
volatile int tickCounter = 0;
volatile int rpmArray[5] = {0};
volatile int index = 0;
volatile unsigned long time = 100;
volatile unsigned long rpm = 0;

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

void setup ()
{
  attachInterrupt(0, motorTick, RISING);
  Serial.begin(9600);

  Timer1.initialize(TIMER_PER_uS);
  Timer1.attachInterrupt(computeRPM);
}

void loop ()
{
  String sRpm(rpm);
  Serial.println("RPM: " + sRpm);
  delay(1000);
}
