#define S1 14//LEFT SENSOR
#define S2 15//RIGHT SENSOR
//#define TX 2//DISPLAY RX

#define OK A6//reset button
#define SET 17//reset button
#define RESET 18//reset button//seek
#define RECALL 16//RECALL//up
#define BUZZ 19//RECALL//up

#define MOVE 18//reset button//seek
#define UPNUM 16//RECALL//up

//#define len 50.00//46.50f//assume 1 meter// per revolution

#define yes 1//touched
#define no 0//not touched
#define invalid -1//touched not out

#define Ttoggle 400

#define th 499

#define LN 12

#define TOUCH LOW
#define nTOUCH HIGH

#define PRESS LOW
#define nPRESS HIGH

#define ON HIGH
#define OFF LOW

#define ADD 1

#define S 11
#define E 12
#define P 13
#define A 14
#define V 15
#define L 17

//#include<SoftwareSerial.h>
#include<TimerOne.h>
#include<EEPROM.h>

uint32_t timer, toggle;
volatile int32_t count = 0;
volatile int8_t state1 = no;//-1 -invalid
volatile int8_t state2 = no;//-1 -invalid
volatile uint16_t check = 0;
volatile float len = 50.00f;
volatile float BUZZth = 100.00;

int32_t old_count = 0;
//The code shows the analog value of a sensor connected to the arduino board.

//The value displayed can range anywhere from 0 to 9999 on a four-digit display.

const int sensorPin = 0; //The analog sensor is connected to analog pin 0 of the arduino

//ABCDEFG,dp
const int numeral[18] = {
  B11111100, //0
  B01100000, //1
  B11011010, //2
  B11110010, //3
  B01100110, //4
  B10110110, //5
  B00111110, //6
  B11100000, //7
  B11111110, //8
  B11100110, //9
  B00010000, //_
  B10110110, //S
  B10011110, //E
  B11001110, //P
  B11101110, //A
  B01111100, //V
  B00000010, //-
  B00011100, //L
};

//pins for decimal point and each segment
//dp, G, F, E, D, C, B, A
const int segmentPins[] = {5, 7, 10, 3, 4, 6, 13, 9};//{2, 3, 4, 5, 6, 7, 8}; //

const int numberofDigits = 4;

const int digitPins[numberofDigits] = {8, 11, 12, 2}; //digits 1, 2, 3, 4

void buzzON() {
  //  pinMode(BUZZ, OUTPUT);
  digitalWrite(BUZZ, HIGH);
}

void buzzOFF() {
  //  pinMode(BUZZ, INPUT_PULLUP);
  digitalWrite(BUZZ, LOW);
}

void COUNT() {
  //  Serial.println(F("COUNT"))
  //lcd.setCursor(15, 0);
  //lcd.print(' ');
  ((count * len) >= (BUZZth * 100)) ? buzzON() : buzzOFF();

  if (digitalRead(S1) == TOUCH && state1 == no) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state1 = yes;
    //    Serial.print(F("A"));
  }
  if (digitalRead(S1) == nTOUCH && state1 == yes) { //METAL DETECTED
    //COUNT
    state1 = invalid;
    //s1 crossed
    //check for stage 2 or out
    Timer1.attachInterrupt(COUNT_2);
    //    Serial.print(F("B"));
  }

  if (digitalRead(S2) == TOUCH && state2 == no) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state2 = yes;
    //    Serial.print(F("C"));
  }
  if (digitalRead(S2) == nTOUCH && state2 == yes) { //METAL DETECTED
    //COUNT
    state2 = invalid;
    //check for stage 2 or out
    Timer1.attachInterrupt(COUNT_1);
    //    Serial.print(F("D"));
  }
}

void COUNT_1() {
  //CROSSED S2
  //COUNT--
  //  Serial.println(F("COUNT1"));
  //lcd.setCursor(15, 0);
  //lcd.print('-');

  if (digitalRead(S1) == TOUCH && state1 == no) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state1 = yes;
    //    Serial.print(F("A1"));
  }
  if (digitalRead(S1) == nTOUCH && state1 == yes) { //METAL DETECTED
    //COUNT
    state1 = no;
    state2 = no;
    if (count > 0)
      count--;
    //
    //    update('-');
    Timer1.attachInterrupt(COUNT);
    //    Serial.print(F("A2"));
  }

  if (digitalRead(S2) == TOUCH && state2 == invalid) { //METAL DETECTED
    //WAIT FOR TO BECOME HIGH
    state2 = yes;
    //    Serial.print(F("A3"));
  }
  if (digitalRead(S2) == nTOUCH && state2 == yes) { //METAL DETECTED
    //COUNT
    state2 = no;
    //check for stage 2 or out
    Timer1.attachInterrupt(COUNT);
    //    Serial.print(F("A4"));
  }
}

void COUNT_2() {//increament count
  //CROSSED S1
  //COUNT++
  //state1 is invalid
  //if s1 crossed then ignore count
  //  Serial.println(F("COUNT2"));
  //lcd.setCursor(15, 0);
  //lcd.print('+');
  if (digitalRead(S1) == TOUCH && state1 == invalid) { //  s1 touched again
    //WAIT FOR TO BECOME HIGH
    state1 = yes;
    //    Serial.print(F("B1"));
  }
  if (digitalRead(S1) == nTOUCH && state1 == yes) { //METAL DETECTED
    //COUNT
    state1 = no;
    //check for stage 2 or out
    Timer1.attachInterrupt(COUNT);
    //    Serial.print(F("B2"));
  }

  //if s2 crossed then count---- tested
  if (digitalRead(S2) == TOUCH && state2 == no) { //s1 touched already then waiting for s2
    //WAIT FOR TO BECOME HIGH
    state2 = yes;
    //    Serial.print(F("B3"));
  }
  if (digitalRead(S2) == nTOUCH && state2 == yes) { //METAL DETECTED
    //COUNT
    state2 = no;
    state1 = no;
    count++;
    //    update('+');

    //
    Timer1.attachInterrupt(COUNT);
    //    Serial.print(F("B4"));
  }
}


void setup()
{ pinMode(RESET, INPUT_PULLUP);
  pinMode(RECALL, INPUT_PULLUP);
  pinMode(SET, INPUT_PULLUP);
  pinMode(OK, INPUT_PULLUP);
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);

  pinMode(BUZZ, OUTPUT);
  digitalWrite(BUZZ, OFF);//OFF

  //sets the digit pins as outputs
  for (int i = 0; i < numberofDigits; i++)
    pinMode(digitPins[i], OUTPUT);
  //  delay(500);
  digitalWrite(BUZZ, ON);//ON
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT); //set segment and DP pins to output
    digitalWrite(segmentPins[i], LOW);
    delay(100);
    //    digitalWrite(segmentPins[i], LOW);
    //    delay(100);
  }

  digitalWrite(BUZZ, OFF);//OFF
  //RESET DIGITS TO HIGH AND SEGEMENTS TO LOW
  for (int i = 0; i < numberofDigits; i++) {
    digitalWrite(digitPins[i], HIGH);
    delay(100);
  }
  //read length from eeprom
  EEPROM.get(ADD + 4, BUZZth);
  EEPROM.get(ADD, len);

  Timer1.initialize(1000);//(10000);
  Timer1.attachInterrupt(COUNT);
  timer = millis() / 100;
  while ((millis() / 100) - timer < 1500 / 100) {
    showDigit(A, 0, 7);//S
    showDigit(E, 1, 7);//E
    showDigit(P, 2, 2);//P
    showDigit(L, 3, 3);//S
  }
  timer = millis() / 100;
  while ((millis() / 100) - timer < 1500 / 100) {
    showDigit(S, 0, 0);//S
    showDigit(E, 1, 1);//E
    showDigit(P, 2, 2);//P
    showDigit(S, 3, 3);//S
  }

  timer = millis() / 100;
  while ((millis() / 100) - timer < 1200 / 100) {
    showNumber(len * 100, 1, 0);
  }

  timer = millis() / 100;
  while ((millis() / 100) - timer < 1200 / 100) {
    showNumber(BUZZth * 10, 2, 0);
  }

  timer = millis() / 100;
  while (((millis() / 100) - timer) < 700 / 100) {
    showDigit(16, 0, 5);//-
    showDigit(16, 1, 0);//
    showDigit(16, 2, 0);//
    showDigit(16, 3, 0);//
  }

  //show length then out
  //  Serial.begin(19200);
  //  while (1)
  //    showNumber(1234, 2, 0);
}

void loop()
{
  //  int value = analogRead(sensorPin);
  //  showNumber(value);
  //  showNumber(1234);
  //  uint32_t i = 0, timer = millis();
  //  while (i < 9999) {
  //    if ((millis() - timer) >= 10) {
  //      i++;
  //      timer = millis();
  //    }
  //    showNumber(i);
  //  }
  if ((digitalRead(RESET) == PRESS)) {
    while (digitalRead(RESET) == PRESS);
    if (digitalRead(RESET) == nPRESS) {
      Timer1.detachInterrupt();
      //RESET
      if (count != 0)
        old_count = count;
      count = 0;
      //RESET STATE
      //      old_state1 = state1;
      //      old_state2 = state2;
      state1 = no;
      state2 = no;
      Timer1.attachInterrupt(COUNT);
    }
  }
  //recall
  if (digitalRead(RECALL) == PRESS) {
    while (digitalRead(RECALL) == PRESS);
    if (digitalRead(RECALL) == nPRESS) {
      Timer1.detachInterrupt();
      //RESET COUNTER
      count = old_count;
      //RESET STATE
      //      state1 = old_state1;
      //      state2 = old_state2;
      Timer1.attachInterrupt(COUNT);
    }
  }

  if (digitalRead(SET) == PRESS) {
    while (digitalRead(SET) == PRESS);
    if (digitalRead(SET) == nPRESS) {
      Timer1.detachInterrupt();
      uint32_t tout = millis() / 100, OLDlen = len * 100, offset = 1, OLDBUZZth = BUZZth * 10;
      toggle = millis();
      uint8_t pos = 4, page = 1;
      while ((millis() / 100 - tout) < (10000 / 100)) { // && digitalRead(OK) == HIGH) {
        //////////////
        if (digitalRead(SET) == PRESS) {
          while (digitalRead(SET) == PRESS) {
            if (page == 1)
              showNumber(OLDlen, 1, pos);//pos-position of digit to be set from left to right
            else if (page == 2)
              showNumber(OLDBUZZth, 2, pos);//pos-position of digit to be set from left to right
          }
          if (digitalRead(SET) == nPRESS) {
            page == 1 ? page = 2 : page = 1;
            tout = millis() / 100;
          }
        }
        if (digitalRead(MOVE) == PRESS) {
          while (digitalRead(MOVE) == PRESS) {
            if (page == 1)
              showNumber(OLDlen, 1, pos);//pos-position of digit to be set from left to right
            else if (page == 2)
              showNumber(OLDBUZZth, 2, pos);//pos-position of digit to be set from left to right
          }
          if (digitalRead(MOVE) == nPRESS) {
            if (pos < 5)
              pos++;
            if (pos > 4)
              pos = 1;
            tout = millis() / 100;
            offset = 1;
            for (uint8_t i = pos; i < 4; i++) {
              offset *= 10;
            }
          }
          //          showNumber(OLDlen, 1, pos);//pos-position of digit to be set from left to right
        }

        //UPNUM
        if (digitalRead(UPNUM) == PRESS) {
          while (digitalRead(UPNUM) == PRESS) {
            if (page == 1)
              showNumber(OLDlen, 1, pos);//pos-position of digit to be set from left to right
            else if (page == 2)
              showNumber(OLDBUZZth, 2, pos);//pos-position of digit to be set from left to right
          }
          if (digitalRead(UPNUM) == nPRESS) {
            uint32_t temp;
            page == 1 ? (temp = OLDlen / offset) : (temp = OLDBUZZth / offset); //pow(10, (4 - pos));
            //            Serial.println(temp);
            temp %= 10;//pow(10, (4 - pos));
            //            uint8_t temp1 = temp;
            //            Serial.println(temp);
            //            Serial.println(OLDlen);
            if (temp < 9) {
              //              Serial.println((uint32_t)pow(10, (4 - pos)) + 1);
              //              OLDlen = OLDlen + (uint32_t)pow(10, (4 - pos)) + 1;
              page == 1 ? (OLDlen += offset) : (OLDBUZZth += offset);
              //              Serial.print("add");
            }
            else if (temp == 9) {
              //              OLDlen = OLDlen - (9 * ((uint32_t)pow(10, (4 - pos)) + 1));
              page == 1 ? (OLDlen -= (9 * offset)) : (OLDBUZZth -= (9 * offset));
              //              Serial.print("sub");
            }
            //              temp = 0;
            //            OLDlen = temp;
            //            Serial.println(OLDlen);
            tout = millis() / 100;
          }
          //          showNumber(OLDlen * 100, 1, pos);//pos-position of digit to be set from left to right
        }
        //if set low
        if (analogRead(OK) <= th) {
          if (((len * 100 != OLDlen) || (BUZZth * 10 != OLDBUZZth)) && (OLDlen > 0) && (OLDBUZZth > 0)) {
            timer = millis() / 100;
            while (((millis() / 100) - timer) < 1000 / 100) {
              showDigit(S, 0, 5);//S
              showDigit(A, 1, 0);//A
              showDigit(V, 2, 0);//V
              showDigit(E, 3, 0);//E
            }
            if (len * 100 != OLDlen) {
              len = (float)(OLDlen / 100.00);
              EEPROM.put(ADD, len);
              timer = millis() / 100;
              while (((millis() / 100) - timer) < 1000 / 100) {
                showNumber(len * 100, 1, 0);
              }
            }
            if (BUZZth * 10 != OLDBUZZth) {
              BUZZth = (float)(OLDBUZZth / 10.00);
              EEPROM.put(ADD + 4, BUZZth);
              timer = millis() / 100;
              while (((millis() / 100) - timer) < 1000 / 100) {
                showNumber(BUZZth * 10, 2, 0);
              }
            }
            timer = millis() / 100;
            while (((millis() / 100) - timer) < 700 / 100) {
              showDigit(16, 0, 5);//-
              showDigit(16, 1, 0);//
              showDigit(16, 2, 0);//
              showDigit(16, 3, 0);//
            }
          }
          break;
        }
        if (page == 1)
          showNumber(OLDlen, 1, pos);//pos-position of digit to be set from left to right
        else if (page == 2)
          showNumber(OLDBUZZth, 2, pos);//pos-position of digit to be set from left to right
        //        len = (float)(OLDlen / 100.00);
      }
    }
    Timer1.attachInterrupt(COUNT);
  }
  showNumber((count * len) / 10.00, 2, 0);
}

void showNumber (int number, int dec, int skip)
{
  if (number == 0 && skip == 0) {
    showDigit (0, numberofDigits - 1, dec); //display 0 in the rightmost digit
  }
  if (number > 0 && number <= 9 && skip == 0)
  {
    showDigit(0, 2, 2); //digit + 1);
  }
  for (int digit = numberofDigits - 1; digit >= 0; digit--)
  {
    if (number > 0)
    {
      if (skip != (digit + 1)) {
        showDigit(number % 10, digit, dec);
      }
      else {
        if ((millis() - toggle) > 2 * Ttoggle) {
          //            showDigit(10, digit, dec);
          toggle = millis();
        }
        else if ((millis() - toggle) < Ttoggle) {
          showDigit(number % 10, digit, dec);
          //            toggle = millis();
        }
      }
      number /= 10.00;
      //        number = number / 10.00;
    }
    else if (number == 0) {
      if (skip == (digit + 1)) {
        if ((millis() - toggle) > 2 * Ttoggle) {
          //            showDigit(10, digit, dec);
          toggle = millis();
        }
        else if ((millis() - toggle) < Ttoggle) {
          //            showDigit(number % 10, digit, dec);
          //            toggle = millis();
          showDigit(10, digit, dec);
        }
      }
      //      else
      //        showDigit (0, numberofDigits - 1, dec); //display 0 in the rightmost digit
    }
  }
}
//Displays given number on a 7-segment display at the given digit position
void showDigit (int number, int digit, int dec)
{
  for (int segment = 0; segment < 8; segment++)
  {
    boolean isBitSet = bitRead(numeral[number], segment);

    isBitSet = ! isBitSet; //remove this line if common cathode display
    digitalWrite(segmentPins[segment], isBitSet);
    if (digit == dec)
      digitalWrite(segmentPins[0], 0);
  }
  digitalWrite(digitPins[digit], 0);
  delayMicroseconds(5000);
  digitalWrite(digitPins[digit], 1);
}
