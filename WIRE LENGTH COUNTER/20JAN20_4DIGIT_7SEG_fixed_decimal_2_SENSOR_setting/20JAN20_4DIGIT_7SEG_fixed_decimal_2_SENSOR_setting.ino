#define S1 14//LEFT SENSOR
#define S2 15//RIGHT SENSOR
//#define TX 2//DISPLAY RX

#define OK 19//reset button
#define SET 18//reset button
#define RESET 16//reset button//seek
#define RECALL 17//RECALL//up

#define MOVE 16//reset button//seek
#define UPNUM 17//RECALL//up

//#define len 50.00//46.50f//assume 1 meter// per revolution

#define yes 1//touched
#define no 0//not touched
#define invalid -1//touched not out

#define Ttoggle 400

#define LN 12

#define TOUCH LOW
#define nTOUCH HIGH

//#include<SoftwareSerial.h>
#include<TimerOne.h>

uint32_t timer, toggle;
volatile int32_t count = 0;
volatile int8_t state1 = no;//-1 -invalid
volatile int8_t state2 = no;//-1 -invalid
volatile uint16_t check = 0;
volatile float len = 50.00;

int32_t old_count = 0;
//The code shows the analog value of a sensor connected to the arduino board.

//The value displayed can range anywhere from 0 to 9999 on a four-digit display.

const int sensorPin = 0; //The analog sensor is connected to analog pin 0 of the arduino

//ABCDEFG,dp
const int numeral[11] = {
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
};

//pins for decimal point and each segment
//dp, G, F, E, D, C, B, A
const int segmentPins[] = {4, 7, 8, 6, 5, 3, 2, 9};//{2, 3, 4, 5, 6, 7, 8}; //

const int numberofDigits = 4;

const int digitPins[numberofDigits] = {10, 11, 12, 13}; //digits 1, 2, 3, 4

void COUNT() {
  //  Serial.println(F("COUNT"))
  //lcd.setCursor(15, 0);
  //lcd.print(' ');
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
  Timer1.initialize(1000);//(10000);
  Timer1.attachInterrupt(COUNT);

  for (int i = 0; i < 8; i++)
    pinMode(segmentPins[i], OUTPUT); //set segment and DP pins to output

  //sets the digit pins as outputs
  for (int i = 0; i < numberofDigits; i++)
    pinMode(digitPins[i], OUTPUT);
  //read length from eeprom
  //show length then out
  //  Serial.begin(19200);
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
  if (digitalRead(RESET) == LOW) {
    while (digitalRead(RESET) == LOW);
    if (digitalRead(RESET) == HIGH) {
      Timer1.detachInterrupt();
      //RESET COUNTER
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
  if (digitalRead(RECALL) == LOW) {
    while (digitalRead(RECALL) == LOW);
    if (digitalRead(RECALL) == HIGH) {
      Timer1.detachInterrupt();
      //RESET COUNTER
      count = old_count;
      //RESET STATE
      //      state1 = old_state1;
      //      state2 = old_state2;
      Timer1.attachInterrupt(COUNT);
    }
  }

  if (digitalRead(SET) == LOW) {
    while (digitalRead(SET) == LOW);
    if (digitalRead(SET) == HIGH) {
      Timer1.detachInterrupt();
      uint32_t tout = millis() / 100, OLDlen = len * 100, offset = 1000;
      toggle = millis();
      uint8_t pos = 1;
      while ((millis() / 100 - tout) < (10000 / 100)) { // && digitalRead(OK) == HIGH) {
        //////////////
        if (digitalRead(MOVE) == LOW) {
          while (digitalRead(MOVE) == LOW) {
            showNumber(OLDlen, 1, pos);//pos-position of digit to be set from left to right
          }
          if (digitalRead(MOVE) == HIGH) {
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
        if (digitalRead(UPNUM) == LOW) {
          while (digitalRead(UPNUM) == LOW) {
            showNumber(OLDlen, 1, pos);//pos-position of digit to be set from left to right
          }
          if (digitalRead(UPNUM) == HIGH) {
            uint32_t temp = OLDlen / pow(10, (4 - pos));
            //            Serial.println(temp);
            temp %= 10;//pow(10, (4 - pos));
            //            uint8_t temp1 = temp;
            //            Serial.println(temp);
            //            Serial.println(OLDlen);
            if (temp < 9) {
              //              Serial.println((uint32_t)pow(10, (4 - pos)) + 1);
              //              OLDlen = OLDlen + (uint32_t)pow(10, (4 - pos)) + 1;
              OLDlen = OLDlen + offset;
              //              Serial.print("add");
            }
            else if (temp == 9) {
              //              OLDlen = OLDlen - (9 * ((uint32_t)pow(10, (4 - pos)) + 1));
              OLDlen = OLDlen - (9 * offset);
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
        if (digitalRead(OK) == LOW) {
          len = (float)(OLDlen / 100.00);
          break;
        }
        showNumber(OLDlen, 1, pos);//pos-position of digit to be set from left to right
        //        len = (float)(OLDlen / 100.00);
      }
    }
    Timer1.attachInterrupt(COUNT);
  }
  showNumber((count * len) / 10.00, 2, 0);
}

void showNumber (int number, int dec, int skip)
{
  if (number == 0)
    showDigit (0, numberofDigits - 1, dec); //display 0 in the rightmost digit
  else
  {
    if (number > 0 && number <= 9)
    {
      showDigit(0, 2, 2); //digit + 1);
    }
    for (int digit = numberofDigits - 1; digit >= 0; digit--)
    {
      if (number > 0)
      {
        if ((skip != (digit + 1))) {
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
        number = number / 10.00;
      }
    }
  }
}

//Displays given number on a 7-segment display at the given digit position
void showDigit (int number, int digit, int dec)
{
  digitalWrite(digitPins[digit], HIGH);
  for (int segment = 0; segment < 8; segment++)
  {
    boolean isBitSet = bitRead(numeral[number], segment);

    isBitSet = ! isBitSet; //remove this line if common cathode display
    digitalWrite(segmentPins[segment], isBitSet);
    if (digit == dec)
      digitalWrite(segmentPins[0], 0);
  }
  delay(5);
  digitalWrite(digitPins[digit], LOW);
}
