#include "RadioFunctions.h"
void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
  rfBegin(11); 
  analogWrite(3,0);
}

void loop() {
  if (rfAvailable())  // If data receievd on radio...
  {
    uint8_t result = rfRead();
    analogWrite(3,result);
    Serial.println((int)result);
  }
}
