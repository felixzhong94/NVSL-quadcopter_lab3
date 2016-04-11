#include "serLCD.h"
#include "RadioFunctions.h"

#define CH1_pin 1
#define CH2_pin 2
#define CH3_pin 3
#define CH4_pin 4
#define BUT_pin 7

serLCD lcd;
int CH1_val, CH2_val, CH3_val, CH4_val, BUT_val;

#define CH1_min 390  //380
#define CH1_max 830  //840
#define CH2_min 325  //295
#define CH2_max 750  //760
#define CH3_min 200  //190
#define CH3_max 760  //770
#define CH4_min 270  //260
#define CH4_max 720  //730

const double CH1_conv = 1500.0/(CH1_max - CH1_min);
const double CH2_conv = 1500.0/(CH2_max - CH2_min);
const double CH3_conv = 1500.0/(CH3_max - CH3_min);
const double CH4_conv = 1500.0/(CH4_max - CH4_min);

void convertReading() {
  CH1_val = (CH1_val - CH1_min) * CH1_conv;
  CH1_val = (CH1_val > 1500) ? 1500: CH1_val;
  CH1_val = (CH1_val < 0) ? 0: CH1_val;
  
  CH2_val = (CH2_val - CH2_min) * CH2_conv;
  CH2_val = (CH2_val > 1500) ? 1500: CH2_val;
  CH2_val = (CH2_val < 0) ? 0: CH2_val;
  
  CH3_val = (CH3_val - CH3_min) * CH3_conv;
  CH3_val = (CH3_val > 1500) ? 1500: CH3_val;
  CH3_val = (CH3_val < 0) ? 0: CH3_val;
  CH3_val = 1500 - CH3_val;
  
  CH4_val = (CH4_val - CH4_min) * CH4_conv;
  CH4_val = (CH4_val > 1500) ? 1500: CH4_val;
  CH4_val = (CH4_val < 0) ? 0: CH4_val;
  CH4_val = 1500 - CH4_val;
}


void setup() {
  pinMode(BUT_pin, INPUT);
  Serial.begin(9600);  
  setupLcd();
  
  //Serial1.begin(115200);
  rfBegin(11); 
}

int i = 0;
void loop() {
  BUT_val = digitalRead(BUT_pin);
  readGimbals();
  updateLcd();
  if(++i > 10){
    sendSerialData();
    i = 0;
  }
  sendRfData();
  delay(10);
}

void readGimbals() {
  CH1_val = analogRead(CH1_pin);
  CH2_val = analogRead(CH2_pin);
  CH3_val = analogRead(CH3_pin);
  CH4_val = analogRead(CH4_pin);
  convertReading();
}

void setupLcd() {
  lcd = serLCD();
  lcd.clear();  
  lcd.selectLine(0);
  lcd.print("1-      2-      ");
  lcd.selectLine(1);
  lcd.print("3-      4-      ");
}


void printNUM (int val) {
  if(val < 1000)
      lcd.print("0");
  if(val < 100)
      lcd.print("0");    
  if(val < 10)
      lcd.print("0");      
  lcd.print(val);    
}

void updateLcd() {
  lcd.setCursor(0,2);
  printNUM(CH1_val); 
  lcd.setCursor(0,10);
  printNUM(CH2_val); 
  lcd.setCursor(1,2);
  printNUM(CH3_val); 
  lcd.setCursor(1,10);
  printNUM(CH4_val);
   
  lcd.setCursor(1,15);
  if(BUT_val)
    lcd.print("B");
  else
    lcd.print(" ");
}  

void sendSerialData() {
  Serial.print("CH1_val: "); Serial.print(CH1_val); Serial.print(" ");
  Serial.print("CH2_val: "); Serial.print(CH2_val); Serial.print(" ");
  Serial.print("CH3_val: "); Serial.print(CH3_val); Serial.print(" ");
  Serial.print("CH4_val: "); Serial.print(CH4_val); Serial.print(" ");
  Serial.print("BUTTON_val: "); Serial.print(BUT_val); Serial.print(" ");
  Serial.println(" ");
}

void sendRfData () {
  double result = CH2_val * (255.0/1500);
  
  result = (result > 255)? 255: result;
  result = (result < 0)? 0: result;
  
  uint8_t x = result;

  rfWrite(x);
}
