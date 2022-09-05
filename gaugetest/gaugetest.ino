
int gaugePositivePin=10;
int gaugeNegativePin=11;


void setup() {

pinMode(gaugePositivePin,OUTPUT);
pinMode(gaugeNegativePin,OUTPUT);

  
}
int pos;
void loop() {
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
    gauge(pos);
    delay(250);                       // waits 15 ms for the servo to reach the position
  }  
}

void gauge(int percent){
  int pwm = map(percent,0,100,-128,128);
  
  if(percent<50){
    digitalWrite(gaugePositivePin,LOW);
    analogWrite(gaugeNegativePin,-pwm);
  }
  else{
    digitalWrite(gaugeNegativePin,LOW);
    analogWrite(gaugePositivePin,pwm);
  }
}
