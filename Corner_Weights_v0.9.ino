// Modified 1/24/2019 for separate CLK pins for each HX711 module, added 12th HX711 to code
// NOTES: 1. If one HX711 module is bad, the code will run extremely slowly or not at all.
//        2. The bad module can be identified by commenting out one module at a time from the code starting with scale.begin statements.

#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Keypad setup
#define key1 10 //On keypad ribbon left-to-right: pin 2
#define key2 11 //On keypad ribbon left-to-right: pin 1
#define key3 12 //On keypad ribbon left-to-right: pin 4
#define key4 13 //On keypad ribbon left-to-right: pin 3
// Last connector on keypad to GND

// LCD display setup
LiquidCrystal_I2C lcd_master(0x27, 20, 4); 
LiquidCrystal_I2C lcd1(0x24, 16, 2);
LiquidCrystal_I2C lcd2(0x23, 16, 2);
LiquidCrystal_I2C lcd3(0x26, 16, 2);
LiquidCrystal_I2C lcd4(0x25, 16, 2);

//Scale setup
HX711 scale1;
HX711 scale2;
HX711 scale3;
HX711 scale4;

// Define Arduino pin numbers for the HX711 connections
#define scale1RX  5
#define scale1TX  4
#define scale1Calibration 12760 //YELLOW scale

#define scale2RX  7
#define scale2TX  6
#define scale2Calibration 12400 //WHITE scale

#define scale3RX  9
#define scale3TX  8
#define scale3Calibration 12300 //GREEN scale

#define scale4RX  3
#define scale4TX  2
#define scale4Calibration 12540 //RED scale

float val1, val2, val3, val4;


void setup() {
  Serial.begin(9600);

 //LCD init
  lcd_master.init();
  lcd_master.backlight();
  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();
  lcd3.init();
  lcd3.backlight();
  lcd4.init();
  lcd4.backlight();

 //Boot message
  lcd_master.print(" CORNER WEIGHTS"); //Insert any name here
  lcd_master.setCursor(3, 2);
  lcd_master.print("Clear scales!");
  lcd_master.setCursor(12, 3);
  lcd_master.print("ver 0.92");

  lcd1.print("YELLOW");
  lcd1.setCursor(0,1);
  lcd1.print("Clear scale!"); 
  lcd2.print("WHITE"); 
  lcd2.setCursor(0,1);
  lcd2.print("Clear scale!"); 
  lcd3.print("GREEN"); 
  lcd3.setCursor(0,1);
  lcd3.print("Clear scale!"); 
  lcd4.print("RED"); 
  lcd4.setCursor(0,1);
  lcd4.print("Clear scale!"); 
  
  //Keypad init
  pinMode(key1, INPUT_PULLUP);// set pin as input
  pinMode(key2, INPUT_PULLUP);// set pin as input
  pinMode(key3, INPUT_PULLUP);// set pin as input
  pinMode(key4, INPUT_PULLUP);// set pin as input  
 
 //Scales init 
  scale1.begin(scale1RX, scale1TX);  // First parameter is DT pin. Second parameter is SCK pin
  scale2.begin(scale2RX, scale2TX);
  scale3.begin(scale3RX, scale3TX);
  scale4.begin(scale4RX, scale4TX);
  
  while (scale1.is_ready() !=0 ) {   // Check connection. 0 means OK - 1 means not connected
    lcd_master.clear();
    lcd_master.setCursor(0, 2);
    lcd_master.print("Check YELLOW wiring");
   }

  while (scale2.is_ready() !=0 ) {   // Check connection. 0 means OK - 1 means not connected
    lcd_master.clear();
    lcd_master.setCursor(0, 2);
    lcd_master.print("Check WHITE wiring");
   }

  while (scale3.is_ready() !=0 ) {   // Check connection. 0 means OK - 1 means not connected
    lcd_master.clear();
    lcd_master.setCursor(0, 2);
    lcd_master.print("Check GREEN wiring");
   }

  while (scale4.is_ready() !=0 ) {   // Check connection. 0 means OK - 1 means not connected
    lcd_master.clear();
    lcd_master.setCursor(0, 2);
    lcd_master.print("Check RED wiring");
   }

//Set scale calibration and reset
  scale1.set_scale(scale1Calibration);  // set calibration value from list above
  scale2.set_scale(scale2Calibration);    
  scale3.set_scale(scale3Calibration);    
  scale4.set_scale(scale4Calibration);    
  scale1.tare();               // reset the scale to 0
  scale2.tare();     
  scale3.tare();     
  scale4.tare();

 delay(5000);
lcd_master.clear();  
lcd1.clear();
lcd2.clear();
lcd3.clear();
lcd4.clear(); 
}

void loop() {
  
  int key1S = digitalRead(key1);// read if key1 is pressed
  int key2S = digitalRead(key2);// read if key2 is pressed
  int key3S = digitalRead(key3);// read if key3 is pressed
  int key4S = digitalRead(key4);// read if key4 is pressed 
  
  
  val1 = scale1.get_units(); //YELLOW
  if (val1 < 1) val1 = 0;
  val2 = scale2.get_units(); //WHITE
  if (val2 < 1) val2 = 0;
  val3 = scale3.get_units(); //GREEN
   if (val3 < 1) val3 = 0;
  val4 = scale4.get_units(); //RED
  if (val4 < 1) val4 = 0;


// ---- MASTER display ----
   lcd_master.setCursor(0, 0);
   lcd_master.print("LEFT:");
    if (((val2+val3)*100/(val1+val2+val3+val4)) >0) {     //Display 0 if percentage is NAN
     lcd_master.print((val2+val3)*100/(val1+val2+val3+val4),0);
    } else {
     lcd_master.print("0");
    }
   lcd_master.print("% ");
   lcd_master.setCursor(10, 0);
   lcd_master.print("RGHT:");
    if (((val1+val4)*100/(val1+val2+val3+val4)) >0) {     //Display 0 if percentage is NAN
      lcd_master.print((val1+val4)*100/(val1+val2+val3+val4),0);
     } else {
      lcd_master.print("0");
     }
   lcd_master.print("% ");
   lcd_master.setCursor(0, 1);
   lcd_master.print("FRNT:");
     if (((val1+val2)*100/(val1+val2+val3+val4)) >0) {     //Display 0 if percentage is NAN
      lcd_master.print((val1+val2)*100/(val1+val2+val3+val4),0);
     } else {
      lcd_master.print("0");
     }
   lcd_master.print("% ");
   lcd_master.setCursor(10, 1);
   lcd_master.print("REAR:");
     if (((val3+val4)*100/(val1+val2+val3+val4)) >0) {     //Display 0 if percentage is NAN
      lcd_master.print((val3+val4)*100/(val1+val2+val3+val4),0); 
     } else {
      lcd_master.print("0");
     }
   
   
   lcd_master.print("% ");
   lcd_master.setCursor(3, 2);
   lcd_master.print("TOT: ");
   lcd_master.print(val1+val2+val3+val4);
   lcd_master.print("KG  ");
   lcd_master.setCursor(0, 3);
   lcd_master.print("1: Tare 2:Cross");
   
   delay(1000);
      
// Scale 1 - YELLOW FR
   lcd1.setCursor(0, 0);
   lcd1.print("FRONT RIGHT");
   lcd1.setCursor(0, 1);
   lcd1.print(val1, 2);
   lcd1.print(" KG        ");

// Scale 2 - WHITE FL
   lcd2.setCursor(0, 0);
   lcd2.print("FRONT LEFT");
   lcd2.setCursor(0, 1);
   lcd2.print(val2, 2);
   lcd2.print(" KG        ");

// Scale 3 - GREEN RL
   lcd3.setCursor(0, 0);
   lcd3.print("REAR LEFT");
   lcd3.setCursor(0, 1);
   lcd3.print(val3, 2);
   lcd3.print(" KG        ");

// Scale 4 - RED RR
   lcd4.setCursor(0, 0);
   lcd4.print("REAR RIGHT");
   lcd4.setCursor(0, 1);
   lcd4.print(val4, 2);
   lcd4.print(" KG        ");

// Keypad commands:
  if(!key1S){
     scale1.tare();               // reset the scales
     lcd1.clear();
     lcd1.print(" TARE!");
         
     scale2.tare(); 
     lcd2.clear();
     lcd2.print(" TARE!");

     scale3.tare(); 
     lcd3.clear();
     lcd3.print(" TARE!");
     
     scale4.tare(); 
     lcd4.clear();
     lcd4.print(" TARE!");
     
     delay(4000);                 // Wait for scales to stabilize
   }
   
   if(!key2S){
    lcd_master.clear();
    while (digitalRead(key4) == HIGH ){

     val1 = scale1.get_units(); //YELLOW
     if (val1 < 1) val1 = 0;
     val2 = scale2.get_units(); //WHITE
     if (val2 < 1) val2 = 0;
     val3 = scale3.get_units(); //GREEN
     if (val3 < 1) val3 = 0;
     val4 = scale4.get_units(); //RED
     if (val4 < 1) val4 = 0;
   
     
     lcd_master.setCursor(0, 0);
     lcd_master.print("CROSS WEIGHT");
     lcd_master.setCursor(0, 1);
     lcd_master.print("FR/RL:");
     lcd_master.print((val1+val3)*100/(val1+val2+val3+val4),0);
     lcd_master.print("%");
     lcd_master.setCursor(0, 2);
     lcd_master.print("FL/RR:");
     lcd_master.print((val2+val4)*100/(val1+val2+val3+val4),0);
      lcd_master.print("%");
     lcd_master.setCursor(0, 3);
     lcd_master.print("4: EXIT");
   
     }
   }
    
   if(!key3S){
   }
   
   if(!key4S){
   }
 
 }    // THE END
