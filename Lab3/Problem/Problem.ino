// define shield pins 
#define BUTTON 2 
#define GREEN_LED 4
#define RED_LED 5

#define LED_TIME 200 // 100 ms on 
#define TIMER1_COMPARE_VALUE 31250 //1s w/ 256 prescale 
#define TIMER2_COMPARE_VALUE 255 // 1024 prescale, needs to be 1562.5 to be 10 Hz, so another "prescale" is in the ISR


// init counters/flags for each timer
volatile unsigned int gCounter = 0;
volatile unsigned char gISRFlagTimer1 = 0;
volatile unsigned int gISRFlagTimer2 = 0;
volatile bool buttonPressed = false;

void setup() {
  // init comms
  Serial.begin(9600);

  // init I/Os off
  pinMode(GREEN_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  digitalWrite(RED_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
  
  // Speed of Timer1 = 16MHz/256 = 62.5kHz 
  noInterrupts();
  // reset timer regs 
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR2A = 0;
  TCCR2B = 0;
  // compare match 
  OCR1A = TIMER1_COMPARE_VALUE;
  OCR2A = TIMER2_COMPARE_VALUE;
  // CTC mode 
  TCCR1B |= (1<<WGM12);
  TCCR2B |= (1<<WGM22);
  // set prescales 
  TCCR1B |= (1<<CS12);
  TCCR2B |= (1<<CS22);
  TCCR2B |= (1<<CS21);
  TCCR2B |= (1<<CS20);
  // enable timer compare interrupt
  TIMSK1 |= (1<<OCIE1A);
  TIMSK2 |= (1<<OCIE2A);
  // button interrupt
  // Enable external interrupt on INT0
  EIMSK |= (1 << INT0);
  // Trigger interrupt on any logical change
  EICRA |= (1 << ISC00);
  interrupts();
}

void loop() {
  // foreground task
  if (gCounter == 3) { // print every 3 seconds 
    Serial.println("Foreground ");
    gCounter = 0; 
  }
}

ISR(TIMER1_COMPA_vect){
  // toggle first 
  gISRFlagTimer1 = ~gISRFlagTimer1;
  // every 1 sec
  if(gISRFlagTimer1){
    gCounter++; // main task timer 
    digitalWrite(GREEN_LED, HIGH);
    delay(LED_TIME);
    digitalWrite(GREEN_LED, LOW);
  }
}

ISR(TIMER2_COMPA_vect){
  // count 6 times, 6 times will take .1 sec 
  // this works as another prescaler
  gISRFlagTimer2++;
  if (buttonPressed){
    // 255 * 6 = 1530 Hz, ideally should be 1562.5 but this is closest 
    if (gISRFlagTimer2 == 6){
      gISRFlagTimer2 = 0;
      digitalWrite(RED_LED,HIGH);
    } 
  } else if (gISRFlagTimer2 == 6){
      gISRFlagTimer2 = 0;
  } else {
    digitalWrite(RED_LED,LOW);
  }
}

// ISR for button press
ISR(INT0_vect) {
  buttonPressed = digitalRead(BUTTON);
}

