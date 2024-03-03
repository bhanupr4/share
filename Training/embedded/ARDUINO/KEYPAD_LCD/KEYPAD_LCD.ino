#include <LiquidCrystal.h>

unsigned char val=0;

const int row[] = {A3, A2, A1, A0};

boolean stat[]={HIGH,HIGH,HIGH,HIGH};

const int col[] = {A4, A5, 0, 1};

boolean state[]={HIGH,HIGH,HIGH,HIGH};

//byte rcPins[] = {R0, R1, R2, R3, C0, C1, C2, C3};

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
  lcd.print("hello, world!");
}

void loop() {
  
  for(int i=0;i<4;i++)
  {
   state[i]=digitalRead(rc[i]); 
  }
  
  val=!state[0]+2*!state[1]+4*!state[2]+8*!state[3];  

  lcd.setCursor(0, 1);
  lcd.print(val);
}



