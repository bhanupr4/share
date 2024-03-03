#define S1 14//LEFT SENSOR
#define S2 15//RIGHT SENSOR
//#define TX 2//DISPLAY RX

#define RESET 16//reset button

#define len 46.50f//assume 1 meter// per revolution

#define yes 1
#define no 0
#define invalid -1

#define LN 12

#define TOUCH LOW
#define nTOUCH HIGH

//#include<SoftwareSerial.h>
#include<TimerOne.h>
#include <LiquidCrystal.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;


//SoftwareSerial disp(0, TX);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//volatile uint32_t timer = 0;
volatile uint32_t count = 0;
volatile int8_t state1 = invalid;//-1 -invalid

void update() {
  lcd.setCursor(7, 0);
  lcd.print((count * len) / 100);
  lcd.setCursor(6, 1);
  lcd.print(count);

  Serial.print(digitalRead(S1));
  Serial.print(F("\t\t"));
  Serial.print(digitalRead(S2));
  Serial.print(F("\t\t"));
  Serial.println(count);
}

void COUNT() {
  if (digitalRead(RESET) == LOW) {
    //RESET COUNTER
    count = 0;
    //RESET STATE
    state1 = invalid;
  }
  if (digitalRead(S1) == TOUCH && state1 == invalid) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state1 = yes;
  }
  if (digitalRead(S1) == nTOUCH && state1 == yes) { //METAL DETECTED
    //COUNT
    state1 = invalid;
    count++;
    update();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  pinMode(RESET, INPUT_PULLUP);
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  //  lcd_begin();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Meters="));
  lcd.setCursor(0, 1);
  lcd.print(F("Count="));

  //  timer = millis();
  Timer1.initialize(10000);
  Timer1.attachInterrupt(COUNT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  delay(1000);
  if (digitalRead(RESET) == LOW) {
    //RESET COUNTER
    count = 0;
    //RESET STATE
    state1 = invalid;
  }
}
