// PI metal detector for arduino version_18_min (C) alex — 1967 201
/* Pulsed induction: Charge a coil, then discharge and mesaure the duration of the flyback pulse...
 *  If a pulse takes longer than usual to calm down, there's metal neaby.
 */

int gaugePositivePin=10;
int gaugeNegativePin=11;

int piezoPin=9; //beeeeep
int FETpin=12;
 
//Measured values with calibration offset:
int ss0 = 0;
int ss1 = 0;
int ss2 = 0;

//Calibration measurements:
long c0 = 0;
long c1 = 0;
long c2 = 0;
byte i = 0;

//Booleans? - set to 1 if SSx is lower than Cx ... but never reset before all three have triggered??? TODO: is this a bug?
int sss0 = 0;
int sss1 = 0;
int sss2 = 0;

//Raw ADC measurements:
int s0 = 0;
int s1 = 0;
int s2 = 0;

 

void setup ()

{
  Serial.begin(115200);
  
pinMode(gaugePositivePin,OUTPUT);
pinMode(gaugeNegativePin,OUTPUT);
pinMode(FETpin,OUTPUT);

gauge(0);
delay(500);
gauge(100);
delay(500);
gauge(0);
delay(500);
gauge(100);
delay(500);
gauge(0);


//DDRB = 0xFF; // port B – all out
//DDRD = 0xFF; // port D – all out
delay (1000); //pause for Volts) to stabilise before calibration

 
for (i = 0; i <255; i ++) {// calibration

  //PORTB = B11111111; // Sets port B pins to 1 and turns ON transistor. 
  digitalWrite(FETpin,HIGH);
  delayMicroseconds (200); // wait 200 microseconds
  //PORTB = 0; // Turns OFF transistor
  digitalWrite(FETpin,LOW);
  delayMicroseconds (20);
  s0 = analogRead (A0);
  s1 = analogRead (A0);
  s2 = analogRead (A0);
  c0 = c0 + s0;
  c1 = c1 + s1;
  c2 = c2 + s2;
  delay (3);
  }

c0 = c0 / 255;
c0 = c0 - 5;
c1 = c1 / 255;
c1 = c1 - 5;
c2 = c2 / 255;
c2 = c2 - 5;

Serial.println("Calibration complete: ");
Serial.print("C0: "); Serial.println(c0);
Serial.print("C1: "); Serial.println(c1);
Serial.print("C2: "); Serial.println(c2);
Serial.println();

delay(500);
}

 

void loop ()
{
//PORTB = B11111111; // Sets port B pins to 1 and turns ON transistor.  
digitalWrite(FETpin,HIGH);
delayMicroseconds (200); // wait 200 microseconds
//PORTB = 0; // Turns OFF transistor.
digitalWrite(FETpin,LOW);
delayMicroseconds (20);

s0 = analogRead (A0);
s1 = analogRead (A0);
s2 = analogRead (A0);

//LED readout is done three times... is this somehow relying on POV?

ss0 = s0 - c0;
if (ss0 <0) sss0 = 1; 
ss0 = ss0 / 16; //256/16=16
//PORTD = ss0; // send to the indicator (send to LEDs)
gauge(25); //This might work
delay (1);

ss1 = s1 - c1;
if (ss1 <0)sss1 = 1;
ss1 = ss1 / 16;
//PORTD = ss1; // send to the indicator (send to LEDs)
gauge(50);
delay (1);


ss2 = s2 - c2;
if (ss2 <0) sss2 = 1;
ss2 = ss2 / 16;
//PORTD = ss2; // send to the indicator (send to LEDs)
gauge(75);
delay (1);

Serial.print("SS0: "); Serial.println(ss0);
Serial.print("SS1: "); Serial.println(ss1);
Serial.print("SS2: "); Serial.println(ss2);
Serial.println();



if (sss0 + sss1 + sss2> 2) 
  {
    //WHAT?!
    beeperIsSupposedToBeOn=True;

    gauge(100);
  /*digitalWrite (7, HIGH);
  digitalWrite (6, HIGH);
  digitalWrite (5, HIGH);
  digitalWrite (4, HIGH);
  digitalWrite (3, HIGH);
  digitalWrite (2, HIGH);
  digitalWrite (1, HIGH);
  digitalWrite (0, HIGH);*/
  delay (1);
  sss0 = 0;
  sss1 = 0;
  sss2 = 0;
  }
  else beeperIsSupposedToBeOn = false;
}

unsigned long nextBeepTime = 0;
bool beeperIsSupposedToBeOn = false;
unsigned long piezoFrq = 4000;
uint8_t beepDuration = 25;


void beepMaintenance(){ //Beeper maintenance.
if (beeperIsSupposedToBeOn && millis()>nextBeepTime) {
  tone(piezoPin, piezoFrq, beepDuration); //every 100 ms
  nextBeepTime = millis()+100;
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
