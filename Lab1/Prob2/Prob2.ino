#include <math.h>

double array[360] = {0};

void setup() {
  Serial.begin(9600);
    for (int i = 0; i < 360; i++) {
    array[i] = (cos(i*DEG_TO_RAD));
    Serial.println(array[i]);
  }
}

void loop() {

}
