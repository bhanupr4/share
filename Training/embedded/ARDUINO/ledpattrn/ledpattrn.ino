const int arr[]={9,10,11};


void setup() {
  int i;
  for(i=0;i<3;i++)
  {
    pinMode(arr[i],OUTPUT);
  }

}

void loop() {
  int j;
  for(j=0;j<256;j++)
  {
  analogWrite(arr[2],);    // red
  delay(100);
  }
}
