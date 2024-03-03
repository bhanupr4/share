int val=0;
int a;
float b=0.0;
void setup() {
Serial.begin(9600);
}

void loop() {
val=analogRead(A0);
a=val*4.887;
b=a/10.0;

Serial.println(b);

delay(1000);
}
