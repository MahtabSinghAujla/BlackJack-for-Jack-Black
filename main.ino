#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd0(0x27,16,2);
LiquidCrystal_I2C lcd1(0x3F,16,2);
const int bzr = 3;
const int p1Y = 13;
const int p1N = 12;
char deck[52] = ['A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K']
bool isGameOn = false;

class player {
  public:
    unsigned int bal=50;
    char hand[14];
    int handVal=0;
    unsigned int bet=0;
    bool isStand=false;
    char content [2][16];
    int setBet () {
      unsigned long time=millis();
      while (millis()<time) {
        if (digitalRead(13)==HIGH) {
          ++bet;
        } else if (digitalRead(12)==HIGH) {
          --bet;
        }
        delay(50);
      }
      return bet;
    }
};

void setup() {
  pinMode(bzr, OUTPUT);
  pinMode(p1Y, INPUT);
  pinMode(p1N, INPUT);

  lcd0.init();
  lcd0.backlight();
  lcd1.init();
  lcd1.backlight();
  Serial.begin(9600);
  player p1;
}

void loop() {
  if (isGameOn==false) {
    p1.setBet()
  } else if (isGameOn==true) {
    
  }
}
