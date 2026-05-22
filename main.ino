#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd0(0x27,16,2);
LiquidCrystal_I2C lcd1(0x3F,16,2);
const int bzr = 3;
const int p1Y = 13;
const int p1N = 12;
char deck[52] = {'A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K','A','2','3','4','5','6','7','8','9','X','J','Q','K'};
bool isGameOn = false;

class player {
  public:
    bool isAce=false;
    int whereAce;
    unsigned int bal=50;
    int handIndex=0;
    char hand[14];
    int handVal=0;
    unsigned int bet=0;
    bool isStand=false;
    char content [2][16];
    void setBet () {
      if (digitalRead(13)==HIGH) {
          ++bet;
          delay(50);
      } else if (digitalRead(12)==HIGH) {
          --bet;
          delay(50);
      }
    }
    void hitStand () {
      if (digitalRead(13)==HIGH) {
        Serial.println('p1 hit');
        if (isStand==false) {
          char randIndex=random(52);
          if (deck[randIndex]!='0') {
            char drawn=deck[randIndex];
            hand[handIndex]=drawn;
            Serial.println(drawn);
            deck[randIndex]='0';
            if (isDigit(drawn)==true) {
              handVal+=drawn-'0';
            } else if (drawn=='K'||drawn=='Q'||drawn=='J') {
              handVal+=10;
            } else if (drawn=='A') {
              handVal+=11;
            }
            handIndex++;
            Serial.println(handVal)
          }
          delay(100);
        }
      } else if (digitalRead(12)==HIGH) {
        isStand==true;
        Serial.println('p1 stand');
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
          if (deck[randIndex]!='0') {
            char drawn=deck[randIndex];
            hand[handIndex]=drawn;
            deck[randIndex]='0';
            if (isDigit(drawn)==true) {
              handVal+=drawn-'0';
            } else if (drawn=='K'||drawn=='Q'||drawn=='J') {
              handVal+=10;
            } else if (drawn=='A') {
              handVal+=11;
            }
            handIndex++;
          }
          delay(1000);
        }
      } else if (handVal>=17) {
        isStand==true;
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
    void contentUpdate () {
      for (int a=0;a<16;a++) {
        content[(a+1)%2][a]=hand[a];
      }
    }
};
player p1;
player d;
void setup() {
  randomSeed(21);
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
  d.contentUpdate();
  p1.contentUpdate();
  
  for (int i=0;i<16;i++) {
    lcd0.setCursor(0,0);
    lcd0.print(d.content[0][i]);
  }
  for (int i=0;i<16;i++) {
    lcd0.setCursor(0,1);
    lcd0.print(d.content[1][i]);
  }

  for (int i=0;i<16;i++) {
    lcd1.setCursor(0,0);
    lcd1.print(p1.content[0][i]);
  }
  for (int i=0;i<16;i++) {
    lcd1.setCursor(0,1);
    lcd1.print(p1.content[1][i]);
  }
  
  if (isGameOn==false) {
    unsigned long time=millis();
      while (millis()<(time+1)) {
        p1.setBet();
        isGameOn=true;
      }
  } else if (isGameOn==true) {
    while (p1.isStand==false) {
      p1.hitStand();
      if (p1.handVal==21) {
        p1.bal+=p1.bet;
        p1.bet=0;
        isGameOn=false;
      } else if (p1.handVal>21) {
        if (p1.isAce==true) {
          p1.hand[p1.whereAce]=1;
          p1.handVal-=10;
        } else if (p1.isAce==false) {
          p1.bal-=p1.bet;
          p1.bet=0;
          isGameOn=false;
        }
      }
    }//while p1 hit
    if (p1.isStand==true) {
      if (d.isStand==false) {
        d.dealerPlay();
        if (d.handVal>21) {
          if (d.isAce==true) {
            d.hand[d.whereAce]=1;
            d.handVal-=10;
          } else if (d.isAce==false) {
            p1.bal+=p1.bet;
            p1.bet=0;
            isGameOn=false;
          }
        }
      }//while d hit
      if (d.isStand==true) {
        if (d.handVal>p1.handVal) {
          p1.bal-=p1.bet;
          p1.bet=0;
          isGameOn=false;
        } else if (d.handVal<p1.handVal) {
          p1.bal+=p1.bet;
          p1.bet=0;
          isGameOn=false;
        } else if (d.handVal==p1.handVal) {
          p1.bet=0;
          isGameOn=false;
        }
      }
    }//if p1 stand
  }//isGameOn
}//void loop
