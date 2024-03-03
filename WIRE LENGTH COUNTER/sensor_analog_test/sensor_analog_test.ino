#define S1 A6//LEFT SENSOR
#define S2 15//RIGHT SENSOR

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  pinMode(S1, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(S1));
  //  if (digitalRead(S1) == LOW) {
  //    delay(1);
  //    if (digitalRead(S1) == HIGH) {
  //      Serial.println(F("RESET"));
  //    }
  //  }
  //  delay(100);
}
