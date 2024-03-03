//The code shows the analog value of a sensor connected to the arduino board.

//The value displayed can range anywhere from 0 to 9999 on a four-digit display.

const int sensorPin = 0; //The analog sensor is connected to analog pin 0 of the arduino

//ABCDEFG,dp
const int numeral[10] = {
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
};

//pins for decimal point and each segment
//dp, G, F, E, D, C, B, A
const int segmentPins[] = {4, 7, 8, 6, 5, 3, 2, 9};//{2, 3, 4, 5, 6, 7, 8}; //

const int numberofDigits = 4;

const int digitPins[numberofDigits] = {10, 11, 12, 13}; //digits 1, 2, 3, 4

void setup()
{
  for (int i = 0; i < 8; i++)
    pinMode(segmentPins[i], OUTPUT); //set segment and DP pins to output

  //sets the digit pins as outputs
  for (int i = 0; i < numberofDigits; i++)
    pinMode(digitPins[i], OUTPUT);
}

void loop()
{
  //  int value = analogRead(sensorPin);
  //  showNumber(value);
  //  showNumber(1234);
  uint32_t i = 0, timer = millis();
  while (i < 9999) {
    if ((millis() - timer) >= 10) {
      i++;
      timer = millis();
    }
    showNumber(i);
  }
}

void showNumber (int number)
{
  if (number == 0)
    showDigit (0, numberofDigits - 1); //display 0 in the rightmost digit
  else
  {
    for (int digit = numberofDigits - 1; digit >= 0; digit--)
    {
      if (number > 0)
      {
        showDigit(number % 10, digit);
        number = number / 10;
      }
    }
  }
}

//Displays given number on a 7-segment display at the given digit position
void showDigit (int number, int digit)
{
  digitalWrite(digitPins[digit], HIGH);
  for (int segment = 0; segment < 8; segment++)
  {
    boolean isBitSet = bitRead(numeral[number], segment);

    isBitSet = ! isBitSet; //remove this line if common cathode display
    digitalWrite(segmentPins[segment], isBitSet);
    if (digit == 2)
      digitalWrite(segmentPins[0], 0);
  }
  delay(5);
  digitalWrite(digitPins[digit], LOW);
}
