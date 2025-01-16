#define BUTTON_1 2 
#define RED_LED 5
#define GREEN_LED 4

#define TIME 500

void setup() {
  // Begin Communication
  Serial.begin(9600);

  // Init I/Os
  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);

  // Create ISR vector table
  //attachInterrupt(digitalPinToInterrupt(BUTTON_1),isr_button,CHANGE);
  // Disable Interrupts 
  noInterrupts();
  // Enable external interrupt on INT0
  EIMSK |= (1 << INT0);
  // Trigger interrupt on any logical change
  EICRA |= (1 << ISC00);
  // Enable interrupts
  interrupts();
}

void loop() {
  digitalWrite(RED_LED,HIGH);
  delay(TIME);
  digitalWrite(RED_LED,LOW);
  delay(TIME);
}
ISR(INT0_vect)
{
  if(digitalRead(BUTTON_1))
  {
    digitalWrite(GREEN_LED,HIGH);
  }
  else
  {
    digitalWrite(GREEN_LED,LOW);
  }
}