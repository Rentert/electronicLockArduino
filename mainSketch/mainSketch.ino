#include <TimerOne.h>
#include <SoftwareSerial.h>

SoftwareSerial RFID(3, 2); // RX and TX

bool flag = true;

int numb = 0;

int sr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

long getMillisecond(float quantityOfSecond) {
  return 1000000 * quantityOfSecond;
}

void playNote(int pin, int note,long period, int tempo, void (*isr)()) {
  flag = true;
  Timer1.setPeriod(period);
  
  Timer1.attachInterrupt(isr);
  
  int i = 0;
  while (flag) {
    if( i == 1000)
    {
      delay(tempo / 2); 
      i = 0;
    } else {
       tone(pin, note);
       noTone(pin); 
    }
    i++;
  }
  
  Timer1.detachInterrupt(); 
  
}

void offPlay()
{
  flag = false;
}


void setup()
{
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(0, INPUT);
  
  Timer1.initialize();
  
  digitalWrite(6, HIGH);
  
  playNote(8,600, getMillisecond(0.5), 100, offPlay);

  digitalWrite(7, HIGH);

}

void loop()
{
  if (digitalRead(0) == HIGH) {
    digitalWrite(6, LOW);
  }
  else {
    digitalWrite(6, HIGH);
  }
  if (RFID.available() > 0) {
    if(numb == 14) {
      numb = 0;
      
      digitalWrite(6, LOW);
      playNote(8,1000, getMillisecond(1), 100, offPlay);
      digitalWrite(6, HIGH);
      
     }
     
     sr[numb] = RFID.read();
     numb++;
  }
}
