#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd0(0x27,16,2);
LiquidCrystal_I2C lcd1(0x3F,16,2);

void setup() {
  lcd0.init();
  lcd0.backlight();
  lcd1.init();
  lcd1.backlight();
}

void loop() {
  lcd0.setCursor(0,0);
  lcd0.print("This is");
  lcd0.setCursor(0,1);
  lcd0.print("The Dealer");

  lcd1.setCursor(0,0);
  lcd1.print("This is");
  lcd1.setCursor(0,1);
  lcd1.print("The Player");
