#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {6,5,4,3};
byte colPins[COLS] = {2,9,8,7};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LEDs
int led1 = 10;
int led2 = 11;
int led3 = 12;

// Buttons
int btn1 = A0;
int btn2 = A1;
int btn3 = A2;
int emergencyBtn = A3;

// Buzzer
int buzzer = 13;

// Passwords
String password = "1234";
String emergencyPass = "6666";
String input = "";

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(emergencyBtn, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);

  lcd.print("Enter Password");

  randomSeed(analogRead(0));
}

void loop() {

  char key = keypad.getKey();

  if (key) {
    lcd.setCursor(0,1);
    lcd.print(input + key);
    input += key;

    if (input.length() == 4) {

      // 🚨 Emergency
      if (input == emergencyPass && digitalRead(emergencyBtn) == LOW) {
        emergencyOpen();
      }

      // 🎮 Game
      else if (input == password) {
        gameMode();
      }

      // ❌ Wrong password
      else {
        accessDenied();
      }

      input = "";
      delay(500);
      lcd.clear();
      lcd.print("Enter Password");
    }
  }
}

// ❌ ACCESS DENIED (NO LOCK)
void accessDenied() {
  lcd.clear();
  lcd.print("ACCESS DENIED");

  tone(buzzer, 1000);
  delay(1500);
  noTone(buzzer);

  delay(1000); // wait then return
}

// 🎮 GAME MODE
void gameMode() {
  lcd.clear();
  lcd.print("GAME START");
  delay(1000);

  int correct = random(1,4);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  // LED ON for 1 sec
  if (correct == 1) digitalWrite(led1, HIGH);
  if (correct == 2) digitalWrite(led2, HIGH);
  if (correct == 3) digitalWrite(led3, HIGH);

  delay(1000);

  // LED OFF
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  lcd.setCursor(0,1);
  lcd.print("Press Button");

  while (true) {

    if (digitalRead(btn1) == LOW) {
      checkAnswer(1, correct);
      break;
    }
    if (digitalRead(btn2) == LOW) {
      checkAnswer(2, correct);
      break;
    }
    if (digitalRead(btn3) == LOW) {
      checkAnswer(3, correct);
      break;
    }
  }
}

// ✅ CHECK BUTTON
void checkAnswer(int pressed, int correct) {

  lcd.clear();

  if (pressed == correct) {
    lcd.print("DOOR OPEN");

    tone(buzzer, 1000);
    delay(500);
    noTone(buzzer);

    digitalWrite(led2, HIGH);
    delay(3000);
    digitalWrite(led2, LOW);
  } 
  else {
    lcd.print("WRONG BTN");

    tone(buzzer, 1000);
    delay(1500);
    noTone(buzzer);
  }

  delay(1000);
}

// 🚨 EMERGENCY
void emergencyOpen() {
  lcd.clear();
  lcd.print("EMERGENCY OPEN");

  tone(buzzer, 2000);
  delay(500);
  noTone(buzzer);

  digitalWrite(led2, HIGH);
  delay(5000);
  digitalWrite(led2, LOW);
}