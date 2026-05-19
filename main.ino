#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd0(0x27,16,2);
const int bzr = 3;
const int p1Y = 13;
const int p1N = 12;

class player {
  public:
    int playerNum;
    int bal=50;
    char hand[14];
    int handVal=0;
    int bet=0;
    bool isStand=false;
    string address;
    player (string a,string b,) {
      playerNum=a;
      address=b
    }
    LiquidCrystal_I2C lcd(address,16,2);
    int setBet () {
      
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
}

void loop() {
  lcd0.setCursor(0,0);
  lcd0.print("This is");
  lcd0.setCursor(0, 1);
  lcd0.print("Dealer");

  lcd1.setCursor(0,0);
  lcd1.print("This is");
  lcd1.setCursor(0, 1);
  lcd1.print("Player 1");

  if (digitalRead(13)==HIGH) {
    Serial.println("Hit");
  }
  if (digitalRead(12)==HIGH) {
    Serial.println("Stand");
  }
}
