#include<SoftwareSerial.h>
SoftwareSerial Bt(10,11);
int led1=9;
int led2=6;
void setup() {
  Serial.begin(9600);
  Bt.begin(38400);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
}

void loop() {
  if(Bt.available())
  {
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    String message=Bt.readString();
    Serial.println(message);
    if(message.indexOf("Switch on LED1")>=0)
    {
      digitalWrite(led1,HIGH);
      delay(1000);
    }
    else if(message.indexOf("Switch on LED2")>=0)
    {
      digitalWrite(led2,HIGH);
      delay(1000);
    }
    else if(message.indexOf("Fade LED1")>=0)
    {
      for(int i=0;i<256;i++)
      {
        analogWrite(led1,i);
        delay(30);
      }
      for(int i=255;i>=0;i--)
      {
        analogWrite(led1,i);
        delay(30);
      }
      delay(100);
    }
    else if(message.indexOf("Fade LED2")>=0)
    {
      for(int i=0;i<256;i++)
      {
        analogWrite(led2,i);
        delay(30);
      }
      for(int i=255;i>=0;i--)
      {
        analogWrite(led2,i);
        delay(30);
      }
      delay(100);
    }
  }
  delay(1000); 
}
