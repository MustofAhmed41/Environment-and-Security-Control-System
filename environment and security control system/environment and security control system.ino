#include <LiquidCrystal.h>
#include <Servo.h>
#include <Keypad.h>

LiquidCrystal lcd(47, 48, 50, 51, 52, 53);

int ledpin1 = 13;
int ledpin2 = 12;
int ledpin3 = 11;

int ldr = A0;
int ldrval = 0;

int tempval = 0;
int lm35 = A1;
int LMF = 23;
int LMR = 24;

const byte ROWS = 4;
const byte COLS = 3;

byte rowPins[ROWS] = {30, 31, 32, 33};
byte colPins[COLS] = {34, 35, 36};

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '='},
};
const int buzzer = 45;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Servo myservo;


int serpos = 0 ;

char ch[100];
int chcount = 0;

void setup() {

  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);
  pinMode(ledpin3, OUTPUT);
  myservo.attach(22);
  pinMode(LMF, OUTPUT);
  pinMode(LMR, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

int trycount = 0;
int a = 10;
bool sys = false;
bool intru = false;
void loop() {

  if (!false) {
    char key = keypad.getKey();
    if (key) {
      Serial.println(key);
      ch[chcount++] = key;
      if (chcount == 3) {
        if (ch[0] == '*' && ch[1] == '1' && ch[2] == '2') {
          sys = true;
          intru = false;
          tone(buzzer, 0);
          noTone(buzzer); 
          lcd.clear();
        } else {
          lcd.setCursor(0, 0);
          lcd.print("WRONG PASSOWRD!");
          delay(1000);
          lcd.clear();
          trycount++;
          chcount = 0;
        }

        if (trycount == 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Intruder Alert");
          intru = true;
        }
      }
    }
  }

  if (intru) {
    tone(buzzer, 1000);   
  } else {
    noTone(buzzer);    
  }

  if (sys) {
    ldrval = analogRead(ldr);
    if ( ldrval >= 10 && ldrval <= 300) {
      myservo.write(5);

    } else if (ldrval >= 301 && ldrval <= 600) {
      myservo.write(65);
    }
    else if (ldrval >= 601) {
      myservo.write(78);
    }

    tempval = analogRead(lm35);
    float millivolts = (tempval / 1024.0) * 5 * 1000;
    float cel = millivolts / 10;

    if (cel >= 15) {
      fanOn();
    } else if (cel < 15) {
      fanOff();
    }
  }


}

void fanOn() {
  digitalWrite(LMF, HIGH);
  digitalWrite(LMR, LOW);
}

void fanOff() {
  digitalWrite(LMR, LOW);
  digitalWrite(LMF, LOW);
}

//lcd.setCursor(0, 0);
//  lcd.print("degree Celcius");
//
//  Serial.println("Hi" + String(a));
//  digitalWrite(ledpin1, HIGH);
