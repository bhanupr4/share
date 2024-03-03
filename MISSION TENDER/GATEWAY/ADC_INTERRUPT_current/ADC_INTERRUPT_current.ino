//take sample of 1 periods
//339.0604587/root(2)=240.136706
#define SHOW_SERIAL 0

#define mul 1.1*1000//306.07982096//301.47540749402106082228987829846//305.356756//240.136706//306.875407//422.378496//1.1

#define max_sample 100//300//340

#define ZERO 0//511

volatile int16_t last_sample = ZERO;//RECORED LAST SAMPLE
volatile int8_t isSIGNAL = -1;
//SYNC global variable

volatile uint16_t d[max_sample];
volatile uint16_t peak = 0;

volatile int16_t i = 0;

volatile uint32_t timer = 0;//, timer1 = 0;

//SLOWER
ISR(ADC_vect) {
  //sample taken and called
  //detect signal and count
  if (isSIGNAL == -1) {
    if ((last_sample == ZERO) && (ADC == ZERO))//detect zero or mid crossing
      isSIGNAL = 0;
  }
  if (isSIGNAL == 0) {
    //detect and trigger accumlation of singal
    if (last_sample == ZERO) {
      if (ADC > ZERO) {
        isSIGNAL = 1;
        last_sample = ADC;
        i = 0;
        //        timer = micros();
      }
    }
  }
  if (isSIGNAL == 1) {
    //check for tail of signal
    if (last_sample == ZERO) {
      if (ADC > ZERO) {
        //        timer1 = micros();
        //        timer -= micros();
        //stop recording and disable interrupt
        ADCSRA &= ~(1 << ADIE);
        isSIGNAL = 2;
        return;
      }
    }
    //recored the singal
    last_sample = ADC;
    d[i] = ADC;
    i++;
    peak = max(peak, ADC);
    //    if (ADC == 0)
    //      peak = 0;
  }
  //reenable adc to take sample
  ADCSRA |= (1 << ADSC);
}

//FASTER
//ISR(ADC_vect) {
//  //sample taken and called
//  //detect signal and count
//  if (isSIGNAL < 0) {
//    if (last_sample <= 0 && ADC == 0)
//      isSIGNAL = 0;
//  }
//  else if (isSIGNAL == 0) {
//    //detect and trigger accumlation of singal
//    if (last_sample <= 0) {
//      if (ADC > 0) {
//        isSIGNAL = 1;
//        last_sample = ADC;
//        i = 0;
//      }
//    }
//  }
//  else if (isSIGNAL == 1) {
//    //check for tail of signal
//    if (last_sample == 0) {
//      if (ADC > 0) {
//        //stop recording and disable interrupt
//        isSIGNAL = 2;
//        ADCSRA &= ~(1 << ADIE);//disable interrupt
//        return;
//      }
//    }
//    //recored the singal
//    last_sample = ADC;
//    d[i] = ADC;
//    i++;
//    peak = max(peak, ADC);
//    //    if (ADC == 0)
//    //      peak = 0;
//  }
//  //reenable adc to take sample
//  ADCSRA |= (1 << ADSC);
//}

void setup() {
  // put your setup code here, to run once:
  if (SHOW_SERIAL) {
    Serial.begin(2000000);
    while (!Serial);
  }

  //  pinMode(A0, INPUT);
  //ALDR-0//A0
  ADMUX = (1 << REFS1) | (1 << REFS0);//11-1.1V//01-AVCC//00-AREF
  //ADSC TO START CONVERSION 0-IF CONVERSION COMPLETE
  //ADIE- ADC INTERRUPT VECTOR IF CONVERSION COMPLETE "ADC_vrct"
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);//16000/8=200kHz//110-332SAMPLES//111-180SAMPLES

  //distroy 100 old startup sample
  //  for (uint8_t s = 0; s < 100; s++) {
  //    while ((ADCSRA & (1 << ADSC)));
  //    ADCSRA |= (1 << ADSC);
  //  }
  //  ADCSRA |= (1 << ADIE) | (1 << ADSC); //ENABLE ADC INTERRUPT
  //  cli();
  //  Serial.println(F("Setup done"));
  //  delay(100);
  //  sei();
  pinMode(9, OUTPUT);
  //  analogWrite(9, 100);
}

//uint8_t j = 0;

uint16_t adc() {
  ADCSRA |= (1 << ADSC);
  while ((ADCSRA & (1 << ADSC)));
  return ADC;
}

void triggerADC() {
  isSIGNAL = -1;
  last_sample = ZERO;
  peak = 0;
  ADCSRA |= (1 << ADIE) | (1 << ADSC); //ENABLE ADC INTERRUPT
}

void loop() {
  // put your main code here, to run repeatedly:
  //  cli();
  //  for (uint8_t i = 0; i < 255; i++) {
  //    analogWrite(9, i);
  //  while (1) {
  //    //    Serial.println(analogRead(A0));
  //    Serial.println(adc());
  //    delay(10);
  //  }
  if (isSIGNAL == 2) {
    if (SHOW_SERIAL) {
      //    Serial.println();
      for (int32_t k = 0; k < i; k++) {
        //      Serial.print(d[k]);
        //      Serial.print(F("\t"));
        //      Serial.println(peak);

        Serial.print((d[k] * mul) / 1024.0, 6);
        Serial.print(F("\t"));
        Serial.print(((peak * mul) / 1024.0), 6);

        //      Serial.print(F("\t"));
        //      Serial.print(((peak * 1.1) / 1024.0), 6);
        //      Serial.print(F("\t"));
        //      Serial.print(1000000.0 / (timer), 2);
        //      Serial.print(F("\t"));
        //      Serial.print(i);
        Serial.println();
        //    delay(20);
      }
      //    {
      //      Serial.print(((peak * mul) / 1023.0), 1);
      //      Serial.print(F("\t"));
      //      Serial.print(((peak * 1.1) / 1023.0), 5);
      //      //      Serial.print(F("\t"));
      //      //      Serial.print(((timer - timer1)));
      //      Serial.print(F("\t"));
      //      Serial.print((1000000.00 / (timer)), 2);
      //      Serial.print(F("\t"));
      //      Serial.print(i);
      //      Serial.println();
      //    }
    }
    delay(1000);
    triggerADC();
    goto out;
  }

out:
  delay(100);//1000
  //  front = -1;
  //  back = -1;
  //  if (j < 245)
  //    j += 10;
  //  else
  //    j = 0;
  //  analogWrite(9, j);
  //}
  //  Serial.println((d * 1.1) / 1023, 3);
  //  Serial.println(ADCSRA, BIN);
  //  delay(10);
  //  sei();
  //  cli();
  //  sei();
  //  Serial.println(analogRead(A0));
}
