#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd0(0x27,16,2);
LiquidCrystal_I2C lcd1(0x3F,16,2);
const int bzr = 3;
const int p1Y = 13;
const int p1N = 12;

void setup() {
  pinMode(bzr, OUTPUT);
  pinMode(p1Y, INPUT);
  pinMode(p1N, INPUT);

  lcd0.init();
  lcd0.backlight();
  lcd1.init();
  lcd1.backlight();
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

  tone(bzr, 1000);
}
