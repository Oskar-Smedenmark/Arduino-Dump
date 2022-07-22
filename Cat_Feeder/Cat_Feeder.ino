#include <CapacitiveSensor.h>
const int AMOUNTOFSENSORS = 4;
CapacitiveSensor sensors[AMOUNTOFSENSORS];//, CapacitiveSensor(3,6), CapacitiveSensor(3,7)}; //pin 4,5,6,7
int leds[AMOUNTOFSENSORS];
boolean dimmedLeds[AMOUNTOFSENSORS];
const int STARTLED = 9;
boolean switchAble[AMOUNTOFSENSORS];
int lastTouchedSensor[AMOUNTOFSENSORS];
unsigned long lastTouchedTime[AMOUNTOFSENSORS];
unsigned long lastTouchedTimer[AMOUNTOFSENSORS];
int touchDelay = 150; //250 ms between touchregistration
long senseValue = 250;
int currentLed = STARTLED;

//Interrupt
unsigned long button_time = 0;
unsigned long last_button_time = 0;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  for (int i = 0; i < AMOUNTOFSENSORS; i++)
  {
    switchAble[i] = true;
    lastTouchedSensor[i] = -1;
    lastTouchedTimer[i] = 0;
    lastTouchedTime[i] = 0;
    dimmedLeds[i] = false;
    if (i < 2)
      sensors[i] = CapacitiveSensor(3, 5 + i);
    else
      sensors[i] = CapacitiveSensor(4, 5 + i);
    leds[i] = i + 9;
    pinMode(leds[i], OUTPUT);
    digitalWrite(STARTLED+i, HIGH);
  }
  attachInterrupt(digitalPinToInterrupt(2), ISR_resetButton, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  capacitiveTouched(currentLed, sensors[currentLed - STARTLED]);
  currentLed++;
  if (currentLed >= STARTLED + AMOUNTOFSENSORS)
  {
    currentLed = STARTLED;
  }
}

void capacitiveTouched(int ledNum, CapacitiveSensor& sensor) {
  int index = ledNum - STARTLED;
  
  long val = sensor.capacitiveSensor(10);
  /*if(index > 1)
  {
    //Serial.println(val);
    //val *= 10;
  }else
    //Serial.println(val);*/10
  if (val <= senseValue && !switchAble[index] && lastTouchedSensor[index] == ledNum)
  {
    switchAble[index] = true;
    lastTouchedSensor[index] = -1;
  }
  else if (switchAble[index] && val > senseValue)
  {
    lastTouchedTime[index] = millis();
    if (lastTouchedTime[index] - lastTouchedTimer[index] > touchDelay)
    {
      switchAble[index] = false;
      if (digitalRead(ledNum) == LOW)
      {
        digitalWrite(ledNum, HIGH);
      }
      else
      {
        if (!dimmedLeds[index] && index != 1 && index != 3)
        {
          analogWrite(ledNum, 32);
          dimmedLeds[index] = true;
        }
        else
        {
          digitalWrite(ledNum, LOW);
          dimmedLeds[index] = false;
        }
      }
      lastTouchedSensor[index] = ledNum;
      lastTouchedTimer[index] = lastTouchedTime[index];
    }
  }
}

void ISR_resetButton()
{
  button_time = millis();
  //check to see if increment() was called in the last 250 milliseconds
  if (button_time - last_button_time > 250) {
    for (int i = 0; i < AMOUNTOFSENSORS; i++)
    {
      digitalWrite(STARTLED + i, HIGH);
      dimmedLeds[i] = false;
    }
    last_button_time = button_time;
  }
}
