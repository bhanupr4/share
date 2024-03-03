
#include<LiquidCrystal.h>

const int X1=A0,Y1=A1,X2=A2,Y2=A3;

boolean touch();
int a=0;
const int p1=6,p2=7,p3=8,p4=9;
int x_cord=0,y_cord=0;
LiquidCrystal lcd(12,11,5,4,3,2);

void setup()
  {
  // Serial.begin(9600); 
   lcd.begin(16,2);
   pinMode(p1,OUTPUT);
   
   pinMode(p2,OUTPUT);
   pinMode(p3,OUTPUT);
   pinMode(p4,OUTPUT);
  // delay(1000);
  // Serial.println("X           Y");
  }
  
void loop()
  {
    if(touch()>0)
    {
  // Serial.print(x_cord);
  // Serial.print("           ");
  // Serial.println(y_cord);
    //delay(500);
   
    
    if(x_cord>0 && x_cord<333 && y_cord>0 && y_cord<250)
    {
      a=1;
      digitalWrite(p1,HIGH);
    //  lcd.print(a);
    }
    
      if(x_cord>333 && x_cord<666 && y_cord>0 && y_cord<250)
    {
      a=2;
      digitalWrite(p1,LOW);
      digitalWrite(p2,HIGH);
    //  lcd.print(a);
    }
    if(x_cord>666 && x_cord<1000 && y_cord>0 && y_cord<250)
    {
      a=3;
      digitalWrite(p2,LOW);
      digitalWrite(p3,HIGH);
      //lcd.print(a);
    }
    if(x_cord>0 && x_cord<333 && y_cord>250 && y_cord<500)
    {
      a=4;
      digitalWrite(p3,LOW);
      digitalWrite(p4,HIGH);
      //lcd.print(a);
    }
    if(x_cord>333 && x_cord<666 && y_cord>250 && y_cord<500)
    {
      a=5;
      digitalWrite(p4,LOW);
     // lcd.print(a);
    }
    if(x_cord>666 && x_cord<1000 && y_cord>250 && y_cord<500)
    {
      a=6;
      //lcd.print(a);
    }
    if(x_cord>0 && x_cord<333 && y_cord>500 && y_cord<750)
    {
      a=7;
      //lcd.print(a);
    }
    if(x_cord>333 && x_cord<666 && y_cord>500 && y_cord<750)
    {
      a=8;
      //lcd.print(a);
    }
    if(x_cord>666 && x_cord<1000 && y_cord>500 && y_cord<750)
    {
      a=9;
      //lcd.print(a);
    }
    if(x_cord>0 && x_cord<333 && y_cord>750 && y_cord<1000)
    {
      a=10;
      //lcd.print(a);
    }
    if(x_cord>333 && x_cord<666 && y_cord>750 && y_cord<1000)
    {
      a=0;
      //lcd.print(a);
    }
   /* if(x_cord>666 && x_cord<1000 && y_cord>750 && y_cord<1000)
    {
      lcd.print(1);
    }*/
  
    //if(a==1)
   // digitalWrite()
    lcd.setCursor(0,0);
    if(a==10)
    {
      lcd.print('*');
    }
    else
    {
   lcd.print(a);

    }
    delay(100);
//lcd.clear();
      
    }
  }
boolean touch()
       {
       boolean touch1=false; 
        
       pinMode(X1,OUTPUT);
       pinMode(X2,OUTPUT);
       digitalWrite(X1,LOW);
       digitalWrite(X2,HIGH);
       pinMode(Y1,INPUT);
       pinMode(Y2,INPUT);
       y_cord=analogRead(Y2);
      
       pinMode(Y1,OUTPUT);
       pinMode(Y2,OUTPUT);
       digitalWrite(Y1,LOW);
       digitalWrite(Y2,HIGH);
       pinMode(X1,INPUT);
       pinMode(X2,INPUT);
       
       x_cord=analogRead(X2);
               
       if(x_cord>0 && x_cord<1000 && y_cord>0 && y_cord<1000)
        touch1=true;
       
       return touch1;
       }

