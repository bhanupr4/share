#define PIR A0
#define BUZZ 11

void setup() {
  // put your setup code here, to run once:
  pinMode(PIR, INPUT);//_PULLUP);
  pinMode(BUZZ, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogRead(PIR) > 400 ? analogWrite(BUZZ, 249) : analogWrite(BUZZ, 0);
  Serial.println(analogRead(PIR));
}
