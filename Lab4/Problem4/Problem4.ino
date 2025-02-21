#define ONOFF 2 
#define SPEED 3 // lock button
#define LED 9 
#define TEMP 
#define TIMER_2_COMP_VAL 78 // for 5ms
int bitSize = 14;
int sampleCount = 0;
int fanSpeed = 0;
bool fanState = 0;

// temp sensor test
float temp = 3.2;

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  noInterrupts();

  // timer 1 for fan PWM
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  TCCR1A |= (1<<WGM11);
  TCCR1B |= (1<<WGM12) | (1<<WGM13);
  TCCR1A |= (1<<COM1A1) | (1<<COM1B1);
  TCCR1B |= (1<<CS10);
  
  // timer 2 for samples
  // speed is 16 MHz / 1024
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = TIMER_2_COMP_VAL;
  TCCR2A |= (1<<WGM21);
  TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
  TIMSK2 |= (1<<OCIE2A);

  interrupts();
  // interrupts
  attachInterrupt(digitalPinToInterrupt(ONOFF), isrONOFF, RISING);
  attachInterrupt(digitalPinToInterrupt(SPEED), isrSPEED, RISING);

}

void loop() {
}

// ISR of onoff
void isrONOFF(){
  fanState = !fanState;
}

// ISR to change speed 
void isrSPEED(){
  if(fanState){ // check if on or off
    fanSpeed = (fanSpeed + 1) % 3;  // cycle through 0, 1, 2
    switch(fanSpeed){
      case 0:
        fanSpeed = 0;
        // analogWrite to fan here 
        // analogWrite(fan, .3*Temperature) PWM is 1/3 of temp
        Serial.println("Fan Speed 0");
        break;
      case 1:
        fanSpeed = 1;
        // analogWrite to fan here 
        // analogWrite(fan, .3*Temperature) PWM is 2/3 of temp        
        Serial.println("Fan Speed 1");
        break;
      case 2:
        fanSpeed = 2;
        // analogWrite to fan here 
        // analogWrite(fan, .3*Temperature) PWM is 100% of temp
        Serial.println("Fan Speed 2");
        break;
    }
  }
}

// ADC function
float SAR14(float Vin, float Vref){
  // init variables
  float thresh;
  int bitValue;
  int result[bitSize];
  float floatResult = 0;
  // half Vref and set thresh to Vref
  Vref /= 2;
  float initialVref = Vref;
  thresh = Vref;

  // start with LSB and count through each bit
  for(int count=0; count<bitSize; count++){
    Vref /= 2;
    // if Vin is greater than thresh, bit is 1 and increase thresh
    if (Vin >= thresh){
      bitValue = 1;
      thresh += Vref;
    } else { // if Vin is less than thresh bit is 0 and decrease thresh
      bitValue = 0;
      thresh -= Vref;
    }
    result[count] = bitValue; // set bit in result to bit finding
  }
  // find float result from array
  for(int count=0; count<bitSize; count++){
    // if result bit 1 is 1
    if(result[count] == 1){
      // Vref / 2^n 
      floatResult = floatResult + (initialVref / (pow(2,count))); 
    }
  }
  return floatResult;
}

// sample every 5 ms
ISR(TIMER2_COMPA_vect){
  sampleCount++;
  // do if real life
  // temp = SAR14(TEMP);
  // for testing
  temp = SAR14(2.75,3);
  // find average temp 
  float averageTemp = averageTemp + temp;
  if (sampleCount == 1000){
    averageTemp = averageTemp/1000;
    sampleCount = 0;
    Serial.println(averageTemp);
    averageTemp = 0;
  }
}