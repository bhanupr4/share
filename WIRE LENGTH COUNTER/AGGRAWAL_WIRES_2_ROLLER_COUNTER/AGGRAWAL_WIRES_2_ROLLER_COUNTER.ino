#define S1 14//LEFT SENSOR
#define S2 15//RIGHT SENSOR
//#define TX 2//DISPLAY RX

#define RESET 16//reset button

#define len 46.50f//assume 1 meter// per revolution

#define yes 1//touched
#define no 0//not touched
#define invalid -1//touched not out

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
volatile int32_t count = 0;
volatile int8_t state1 = invalid;//-1 -invalid
volatile int8_t state2 = invalid;//-1 -invalid
volatile uint16_t check = 0;

void update() {
  lcd.setCursor(7, 0);
  lcd.print((count * len) / 100.00);
  lcd.setCursor(6, 1);
  lcd.print(count);

  //  Serial.print(digitalRead(S1));
  //  Serial.print(F("\t\t"));
  //  Serial.print(digitalRead(S2));
  //  Serial.print(F("\t\t"));
  //  Serial.println(count);
}

void COUNT_old() {
  if (digitalRead(S1) == TOUCH && state1 == invalid) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state1 = no;
  }
  if (digitalRead(S1) == nTOUCH && state1 == yes) { //METAL DETECTED
    //COUNT
    state1 = yes;
    count++;
    update();
  }
}

void COUNT_oldnew() {
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

void _COUNT() {

  if (state1 == yes) {
    //S1 CROSSED
    if (digitalRead(S1) == TOUCH && state1 == yes) { //METAL DETECTED
      //WAIT FOR TO BECOME HIGH
      state1 = invalid;
    }
    if (digitalRead(S1) == nTOUCH && state1 == invalid) { //METAL DETECTED
      //COUNT
      state1 = no;
    }

    if (digitalRead(S2) == TOUCH && state2 == no) { //METAL DETECTED
      //WAIT FOR TO BECOME HIGH
      state2 = invalid;
    }
    if (digitalRead(S2) == nTOUCH && state2 == invalid) { //METAL DETECTED
      //COUNT
      state2 = no;
      count++;
      //
      Timer1.attachInterrupt(COUNT);
    }
  }
  else if (state2 == yes) {
    //S2 CROSSED
    if (digitalRead(S2) == TOUCH && state2 == yes) { //METAL DETECTED
      //WAIT FOR TO BECOME HIGH
      state2 = invalid;
    }
    if (digitalRead(S2) == nTOUCH && state2 == invalid) { //METAL DETECTED
      //COUNT
      state2 = no;
      //check for stage 2 or out
      Timer1.attachInterrupt(COUNT);
    }

    if (digitalRead(S1) == TOUCH && state1 == no) { //METAL DETECTED
      //WAIT FOR TO BECOME HIGH
      state1 = invalid;
    }
    if (digitalRead(S1) == nTOUCH && state1 == invalid) { //METAL DETECTED
      //COUNT
      state1 = no;
      if (count > 0)
        count--;
      //
      Timer1.attachInterrupt(COUNT);
    }
  }
  else {
    if (digitalRead(S1) == TOUCH && state1 == no) { //METAL DETECTED
      //WAIT FOR TO BECOME HIGH
      state1 = invalid;
    }
    if (digitalRead(S1) == nTOUCH && state1 == invalid) { //METAL DETECTED
      //COUNT
      state1 = yes;
      //check for stage 2 or out
      Timer1.attachInterrupt(COUNT_2);
    }

    if (digitalRead(S2) == TOUCH && state2 == no) { //METAL DETECTED
      //WAIT FOR TO BECOME HIGH
      state2 = invalid;
    }
    if (digitalRead(S2) == nTOUCH && state2 == invalid) { //METAL DETECTED
      //COUNT
      state2 = yes;
      //check for stage 2 or out
      Timer1.attachInterrupt(COUNT_1);
    }
  }
}

void COUNT() {
  Serial.println(F("COUNT"));
  if (digitalRead(S1) == TOUCH && state1 == no) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state1 = invalid;
    Serial.print(F("A"));
  }
  if (digitalRead(S1) == nTOUCH && state1 == invalid) { //METAL DETECTED
    //COUNT
    state1 = yes;
    //check for stage 2 or out
    Timer1.attachInterrupt(COUNT_2);
    Serial.print(F("B"));
  }

  if (digitalRead(S2) == TOUCH && state2 == no) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state2 = invalid;
    Serial.print(F("C"));
  }
  if (digitalRead(S2) == nTOUCH && state2 == invalid) { //METAL DETECTED
    //COUNT
    state2 = yes;
    //check for stage 2 or out
    Timer1.attachInterrupt(COUNT_1);
    Serial.print(F("D"));
  }
}

void COUNT_1() {
  //CROSSED S2
  //COUNT--
  Serial.println(F("COUNT1"));
  if (digitalRead(S1) == TOUCH && state1 == no) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state1 = invalid;
    Serial.print(F("A1"));
  }
  if (digitalRead(S1) == nTOUCH && state1 == invalid) { //METAL DETECTED
    //COUNT
    state1 = no;
    if (count > 0)
      count--;
    //
    update();
    Timer1.attachInterrupt(COUNT);
    Serial.print(F("A2"));
  }

  if (digitalRead(S2) == TOUCH && state2 == yes) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state2 = invalid;
    Serial.print(F("A3"));
  }
  if (digitalRead(S2) == nTOUCH && state2 == invalid) { //METAL DETECTED
    //COUNT
    state2 = no;
    //check for stage 2 or out
    Timer1.attachInterrupt(COUNT);
    Serial.print(F("A4"));
  }
}

void COUNT_2() {
  //CROSSED S1
  //COUNT++
  Serial.println(F("COUNT2"));
  if (digitalRead(S1) == TOUCH && state1 == yes) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state1 = invalid;
    Serial.print(F("B1"));
  }
  if (digitalRead(S1) == nTOUCH && state1 == invalid) { //METAL DETECTED
    //COUNT
    state1 = no;
    //check for stage 2 or out
    Timer1.attachInterrupt(COUNT);
    Serial.print(F("B2"));
  }

  if (digitalRead(S2) == TOUCH && state2 == no) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state2 = invalid;
    Serial.print(F("B3"));
  }
  if (digitalRead(S2) == nTOUCH && state2 == invalid) { //METAL DETECTED
    //COUNT
    state2 = no;
    count++;
    update();
    //
    Timer1.attachInterrupt(COUNT);
    Serial.print(F("B4"));
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
  Serial.println(state1);
  Serial.println(state2);
  //  timer = millis();
  Timer1.initialize(500000);//(10000);
  Timer1.attachInterrupt(COUNT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  delay(1000);
  //  if (digitalRead(RESET) == LOW) {
  //    //RESET COUNTER
  //    count = 0;
  //    //RESET STATE
  //    state1 = invalid;
  //  }
}
