#include <TimerOne.h>
#include <SoftwareSerial.h>
#include <EEPROM2.h>
// считыватель карт
SoftwareSerial RFID(3, 2); // RX and TX

bool flag = true;

int speaker = 8;
int diodeA = 7;
int diodeB = 6;
int button = 0;

int keyLen = 14;

int sr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t memory [200];
uint8_t len = 0;
/**
 * Функция возвращает число секунд в милисекундах
*/
long getMillisecond(float quantityOfSecond) {
  return 1000000 * quantityOfSecond;
}
/**
 * Функция воспроизведения заданой частоты, заданной продолжительностью,
 * последний аргумен, это функци которая останавливает воспроизведение
*/
void playNote(int pin, int note,long period, int tempo, void (*isr)()) {
  flag = true;
  Timer1.setPeriod(period);
  
  Timer1.attachInterrupt(isr);
  
  int i = 0;
  // генерируем звук, пока flag не будет false по таймеру
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
/**
 * Функция останавливает воспроизведение звука
 */
void offPlay()
{
  flag = false;
}

/**
 * Функция выполняеться при старте устройства 
 */
void setup()
{  
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
  // инициализируем порты
  pinMode(speaker, OUTPUT);
  pinMode(diodeA, OUTPUT);
  pinMode(diodeB, OUTPUT);
  pinMode(button, INPUT);
  
  Timer1.initialize();
  
  digitalWrite(diodeB, HIGH);
  
  // воспроизводим звук приветствия
  playNote(speaker, 600, getMillisecond(0.5), 100, offPlay);

  digitalWrite(diodeA, HIGH);

}
/**
 * Функция, выполняетьс в бесконечном цикле 
 */
void loop()
{
  // проверяем содержимое буфера, считывателя,
  // если в нем больше 13 символов начинаем считывать
    if (RFID.available() >= 14) {
      int summ = 0;
      // просматриваем 14 первым символов пришедших на устройстов
      for (int i = 0; i < 14; ++i) {
        summ += RFID.read();
        Serial.print(RFID.read(), DEC);
        Serial.print(' ');
      }
      Serial.print('\n');
      // если сумма меньше 100 подаем low на диод A иначе на диод b
      digitalWrite(summ < 100? diodeB : diodeA , LOW);
      playNote(speaker, 600, getMillisecond(0.5), 100, offPlay);
      // ждем пока пользователь не уберет карту
      while(RFID.available() > 0) {
        RFID.read();
        delay(60);
      }
      
      // возващаем диоды в исходное состоние
      digitalWrite(diodeB, HIGH);
      digitalWrite(diodeA, HIGH);
    }
}
