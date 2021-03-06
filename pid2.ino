#include<avr/interrupt.h>
const int phaseA=2;
const int phaseB=3;
const int LPWM=5,RPWM=6;
volatile int xung=0;
double tocdodat;
volatile double tocdora;
volatile double out=0,lastout=0;
volatile double e=0,e1=0,e2=0;
float sampletime=0.01;
double KI,KP,KD;
double alpha,beta,gama;
int forward=0;



void demxung()
{
 
  if(digitalRead(phaseB)==HIGH){
    xung++;
  }
  else
  {
    xung--;
  }
 
}
ISR (TIMER1_OVF_vect)
{
  TCNT1=63035;
  tocdora=abs(xung)*60/(374*sampletime);Serial.println(tocdora);
  xung=0;
  e=tocdodat-tocdora;
  out=(alpha*e+beta*e1+gama*e2+2*sampletime*lastout)/(2*sampletime);
  lastout=out;
  e2=e1;
  e1=e;
  if(out>255)
    out=255;
  else if(out<-255)
  {
    out=-255;
  }
  if(out<0)
    out=255+out;
  //digitalWrite(L_EN,HIGH);
 // digitalWrite(R_EN,LOW);
 if(forward){
  digitalWrite(RPWM,LOW);
  analogWrite(LPWM,out);}
  else
  {
   digitalWrite(LPWM,LOW);
   analogWrite(RPWM,out);
  }
  
  //Serial.println(out);
}
  
void setup() {
  // put your setup code here, to run once:
pinMode(phaseA,INPUT_PULLUP);
pinMode(phaseB,INPUT_PULLUP);
pinMode(LPWM,OUTPUT);
pinMode(RPWM,OUTPUT);
attachInterrupt(0,demxung,FALLING);
Serial.begin(9600);
cli();
  TCCR1A=0x00;
  TCCR1B=0x00;
  TIMSK1=0x00;
  TCCR1B|=(1<<CS10)|(1<<CS11);
  TCNT1=63035;
  TIMSK1=(1<<TOIE1);
  sei();
  KP=0.005;KI=0.01;KD=0;
  alpha=2*sampletime*KP+KI*sampletime*sampletime+2*KD;
  beta=sampletime*sampletime*KI+4*KD+2*sampletime*KP;
  gama=2*KD;
  tocdodat=200;
}

void loop() {
  // put your main code here, to run repeatedly:

}
