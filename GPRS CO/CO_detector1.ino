//#include <Math.h>
int mq7 = A5;
int rl = 1000;
float vrl;
int adcdata;
int r0 = 2000;
float vcc = 4.48;
float ppmco;
float rs;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  GSM.begin(9600);
  pinMode(mq7, INPUT);
  sendGSM("AT+SAPBR=3,1,\"APN\",\"aircelwap\"");
  sendGSM("AT+SAPBR=1,1", 3000);
}

void sendcodata()
{
  sendGSM("AT+HTTPINIT");
  sendGSM("AT+HTTPPARA=\"CID\",1");
  /////////////////
  String sendata = "AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/update.json?api_key=BBVU429XTA00VLSA";
  sendata += "&field1=";
  sendata += String(ppmco);
  sendata += "\"\r";
  Serial.print("sendata=");
  Serial.println(sendata);
  ////////////////
  GSM.println(sendata);
  delay(500);
  while (GSM.available()) {
    parseATText(GSM.read());
  }
  sendGSM("AT+HTTPACTION=0");
}

void calcppm()
{
  adcdata = analogRead(mq7);
  vrl = vcc * adcdata / 1023;
  rs = rl * (( vcc / vrl) - 1);
  //ppmco = pow(100.00 * (rl / r0 * (vcc / vrl - 1)), -1.6);
  ppmco = 100 * pow((rs / r0), -1.6);
  return (String)ppmco;
}

void loop() {
  // put your main code here, to run repeatedly:
  adcdata = analogRead(A5);
  Serial.print("data=");
  Serial.print(adcdata);
  calcppm();
  Serial.print("\tVRL=");
  Serial.print(vrl);
  Serial.print("\tRs=");
  Serial.print(rs);
  Serial.print("\tppm(CO)=");
  Serial.println(ppmco);
  sendcodata();
  delay(1000);
}
