#include<SoftwareSerial.h>
SoftwareSerial wifi(7, 8);//Rx,Tx
int x = 0, y = 0;
int x2, y2;// x and y of device 2

void setup() {
  // put your setup code here, to run once:
  wifi.begin(9600);
  Serial.begin(9600);
  wifi.println("AT+RST");
  Serial.println(wifi.readString());
  wifi.println("AT");
  Serial.println(wifi.readString());
  wifi.println("AT+CWQAP");
  Serial.println(wifi.readString());
  wifi.println("AT+CWMODE=2");
  Serial.println(wifi.readString());
  wifi.println("AT+CWSAP=\"DEVICE1\",\"12345678\",4,0");
  Serial.println(wifi.readString());
  //RESET
  wifi.println("AT+CIPMUX=1");
  Serial.println(wifi.readString());
  wifi.println("AT+CIPSERVER=1,53");
  Serial.println(wifi.readString());
}

void loop() {
  // put your main code here, to run repeatedly:
  while (wifi.available())
  {
    Serial.println("reading ......");
    String st = "";
    st = wifi.readString();
    if (st[2] == '+' && st[3] == 'I' && st[4] == 'P' && st[5] == 'D')
    {
      // extracting x2 and y2 of device 2
      Serial.println("Received extracting x2 and y2");
      for (int i = 0; st[i] != NULL; i++) {
        {
          if (st[i] == 'X')
          {
            String instring = "";
            x2 = 0;
            y2 = 0;
            i++;
            while (st[i] != 'Y')
            {
              instring += st[i];
              i++;
            }
            x2 = x2 + instring.toInt();
            Serial.println(x2);

            instring = "";
            i++;
            while (st[i] != 'E')
            {
              //Serial.print(st[i]);
              instring += st[i];
              i++;
            }
            y2 = y2 + instring.toInt();
            Serial.println(y2);
          }
        }
      }
    }
    /*for (int i = 0; st[i] != NULL; i++) {
      Serial.print("st[");
      Serial.print(i);
      Serial.print("]= ");
      Serial.println(st[i]);
      }*/

    Serial.print(st);
    Serial.println();
    st = "";
  }
  while (Serial.available()) {
    Serial.println("writing...");
    String st1 = "";
    st1 = Serial.readString();
    Serial.println(st1);
    wifi.println(st1);
    st1 = "";
  }
}
