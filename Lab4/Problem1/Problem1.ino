// init from lab assignment 
#define bitSize 14
float Vref = 3.6;
float Vin = 3.3;
float thresh;
int count;
int bitValue;
int results[bitSize];

void setup() { 
  // init comm
  Serial.begin(9600);
}

void loop() {
  // test vals
  float Vref = 3.0;
  float Vin = .83;
  SAR14(Vin,Vref);
  delay(1000);
}

// ADC function
void SAR14(float Vin, float Vref){
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
  // print float result
  Serial.println(floatResult);
}
