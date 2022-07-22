bool bouncing;
int relayPin = 7;
int bounce = 2; //Temperature will try to stay within bounce degrees
 
void setup() {
  // put your setup code here, to run once:
  pinMode(relayPin, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
  bouncing = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  int tmpAV, potAV;
  float tmpVolt, potVolt, degC, desC;
  
  tmpAV = analogRead(0);
  potAV = analogRead(2);

  tmpVolt = tmpAV*5.0 / 1023.0;
  potVolt = (potAV*5.0 / 1023.0);
  
  degC = (tmpVolt-0.59)*100.0;
  desC = (potVolt)*100.0/4,76 + 20;
  
  if(!bouncing)
  {
    if(degC >= desC + bounce)
      bouncing = !bouncing;
  }
  else
  {
    if(degC <= desC - bounce)
      bouncing = !bouncing;
  }
  if(!bouncing)
    digitalWrite(relayPin, LOW);
  else
    digitalWrite(relayPin, HIGH);
}
