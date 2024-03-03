float val=0;

void setup() {
Serial.begin(9600);
}

void loop() {
val=analogRead(A0);
val=val*4.887;  //convert count into milliVolt

val=5000.0-val;

val=val/220.0;

Serial.println(val);
delay(400);
}
