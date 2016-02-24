#include <TimerOne.h>
#include <SoftwareSerial.h>

SoftwareSerial RFID(3, 2); // RX and TX

bool flag = true;

int speaker = 8;
int diodeA = 7;
int diodeB = 6;
int button = 0;

int keyLen = 14;

int numb = 0;

int sr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

class Lock {
  public:
    void lock() {
      flag = true;
    }
     void unLock() {
      flag = false;
    }
    bool isLock() {
      return flag;
    }
  private:
    bool flag;
};

Lock lock;

long getMillisecond(float quantityOfSecond) {
  return 1000000 * quantityOfSecond;
}

void playNote(int pin, int note,long period, int tempo, void (*isr)()) {
  flag = true;
  lock.lock();
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
  lock.unLock();
  
}

void offPlay()
{
  flag = false;
}


void setup()
{
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
  pinMode(speaker, OUTPUT);
  pinMode(diodeA, OUTPUT);
  pinMode(diodeB, OUTPUT);
  pinMode(button, INPUT);
  
  Timer1.initialize();
  
  digitalWrite(diodeB, HIGH);
  
  playNote(speaker, 600, getMillisecond(0.5), 100, offPlay);

  digitalWrite(diodeA, HIGH);

}

void loop()
{
  if(lock.isLock()) {
    digitalWrite(diodeB, !digitalRead(button));
    
    if (RFID.available() > 0) {
      if(numb == keyLen) {
        numb = 0;
        
        digitalWrite(diodeB, LOW);
        playNote(speaker,1000, getMillisecond(1), 100, offPlay);
        digitalWrite(diodeB, HIGH);
        
       }
       
       sr[numb] = RFID.read();
       numb++;
    }
  }
}
