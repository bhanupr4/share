const int adc=A0;
int val=0;
float val1=0;

void setup() {
Serial.begin(9600);
}

void loop() {
val=analogRead(A0);
val1=val*4.887;

Serial.println(val1);

delay(1000);
}
