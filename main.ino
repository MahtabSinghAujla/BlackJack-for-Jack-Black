#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd0(0x27,16,2);
LiquidCrystal_I2C lcd1(0x3F,16,2);
const int ncP = 11;
const int nc1 = 13;
const int nc2 = 12;
char deck[52] = {'A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K'};
bool isGameOn = false;
int lastStateNC1;
unsigned long lastButtonPress=0;
int 

class player {
  public:
    bool isAce=false;
    int whereAce;
    unsigned int bal=500;
    int handIndex=0;
    char hand[12];
    int handVal=0;
    unsigned int bet=0;
    bool isStand=false;
    void setBet () {
      lastStateNC1=digitalRead(nc1);
      if (digitalRead(nc1)!=lastStateNC1 && digitalRead(nc1)==1) {
        if (digitalRead(nc2)!=digitalRead(nc1)) {
          ++bet;
          delay(50);
        } else {
          --bet;
          delay(50);
        }
      }
    }
    void hitStand () {
      lastStateNC1=digitalRead(nc1);
      if (digitalRead(nc1)!=lastStateNC1 && digitalRead(nc1)==1) {
        Serial.println("p1 hit");
        if (isStand==false) {
          char randIndex=random(52);
          while (deck[randIndex]=='0') {
            randIndex=random(52);
          }
          char drawn=deck[randIndex];
          hand[handIndex]=drawn;
          deck[randIndex]='0';
          if (isDigit(drawn)==true) {
            handVal+=drawn-'0';
          } else if (drawn=='K'||drawn=='Q'||drawn=='J'||drawn=='X') {
            handVal+=10;
          } else if (drawn=='A') {
            handVal+=11;
          }
          handIndex++;
            Serial.print(handVal);
          delay(100);
        }
      } else if (digitalRead(ncP)==HIGH) {
        if (millis()-lastButtonPress>50) {
          isStand=true;
          Serial.println("p1 stand");
        }
        lastButtonPress=millis();
      }
      int a=0;
      for (char i : hand) {
        if (i=='A') {
          isAce=true;
          whereAce=a;
        }
        a++;
      }
    }
    void dealerPlay () {
      if (handVal<17) {
        if (isStand==false) {
          char randIndex=random(52);
          while (deck[randIndex]=='0') {
            randIndex=random(52);
          }
          char drawn=deck[randIndex];
          hand[handIndex]=drawn;
          deck[randIndex]='0';
          if (isDigit(drawn)==true) {
            handVal+=drawn-'0';
          } else if (drawn=='K'||drawn=='Q'||drawn=='J'||drawn=='X') {
            handVal+=10;
          } else if (drawn=='A') {
            handVal+=11;
          }
          handIndex++;
        }
      } else if (handVal>=17) {
        isStand=true;
      }
      int a=0;
      for (char i : hand) {
        if (i=='A') {
          isAce=true;
          whereAce=a;
        }
        a++;
      }
    }
    void clear () {
      for (int i=0;i<12;i++) {
        hand[i]='0';
      }
      isAce=false;
      whereAce=0;
      handIndex=0;
      handVal=0;
      bet=0;
      isStand=false;
      char deck2[52] = {'A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K'};
      for (int i=0;i<52;i++) {
        deck[i]=deck2[i];
      }
    }
};
player p1;
player d;
void setup() {
  randomSeed(analogRead(0));
  pinMode(nc1, INPUT);
  pinMode(nc2, INPUT);
  pinMode(ncP, INPUT_PULLUP);

  lcd0.init();
  lcd0.backlight();
  lcd1.init();
  lcd1.backlight();
  Serial.begin(9600);
}

void loop() {
  Serial.println(isGameOn);

  if (isGameOn==false) {
    unsigned long time=millis();
      while (millis()<(time+5000)) {
        p1.setBet();
        lcd1.setCursor(0,0);
        lcd1.print("Set your bet:");
        lcd1.setCursor(14,0);
        lcd1.print(p1.bet);
        lcd1.setCursor(0,1);
        lcd1.print("Balance:");
        lcd1.setCursor(10,1);
        lcd1.print(p1.bal);
      }
    isGameOn=true;
  } else if (isGameOn==true) {
    lcd0.clear();
    lcd1.clear();
    for (int i=0;i<12;i++) {
      lcd0.setCursor(i,(i+1)%2);
      lcd0.print(d.hand[i]);
    }
    for (int i=0;i<12;i++) {
      lcd1.setCursor(i,(i+1)%2);
      lcd1.print(p1.hand[i]);
    }

    if (p1.isStand==false) {
      p1.hitStand();
      if (p1.handVal==21) {
        p1.bal+=p1.bet;
        p1.bet=0;
        p1.clear();
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("You win");
        delay(1000);
        isGameOn=false;
      } else if (p1.handVal>21) {
        if (p1.isAce==true) {
          p1.hand[p1.whereAce]='1';
          p1.handVal-=10;
        } else if (p1.isAce==false) {
          p1.bal-=p1.bet;
          p1.bet=0;
          p1.clear();
          lcd1.clear();
          lcd1.setCursor(0,0);
          lcd1.print("You lose");
          delay(1000);
          isGameOn=false;
        }
      }
    }//while p1 hit
    if (p1.isStand==true) {
      while (d.isStand==false) {
        d.dealerPlay();
        if (d.handVal>21) {
          if (d.isAce==true) {
            d.hand[d.whereAce]='1';
            d.handVal-=10;
          } else if (d.isAce==false) {
            p1.bal+=p1.bet;
            p1.bet=0;
            p1.clear();
            d.clear();
            lcd1.clear();
            lcd1.setCursor(0,0);
            lcd1.print("You win");
            delay(1000);
            isGameOn=false;
          }
        }
      }//while d hit
      if (d.isStand==true) {
        if (d.handVal>p1.handVal) {
          p1.bal-=p1.bet;
          p1.bet=0;
          p1.clear();
          d.clear();
          lcd1.clear();
          lcd1.setCursor(0,0);
          lcd1.print("You lose");
          delay(1000);
          isGameOn=false;
        } else if (d.handVal<p1.handVal) {
          p1.bal+=p1.bet;
          p1.bet=0;
          p1.clear();
          d.clear();
          lcd1.clear();
          lcd1.setCursor(0,0);
          lcd1.print("You win");
          delay(1000);
          isGameOn=false;
        } else if (d.handVal==p1.handVal) {
          p1.bet=0;
          p1.clear();
          d.clear();
          lcd1.clear();
          lcd1.setCursor(0,0);
          lcd1.print("A tie");
          delay(1000);
          isGameOn=false;
        }
      }
    }//if p1 stand
  }//isGameOn
}//void loop
