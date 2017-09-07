/*
* Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS/SDA: Pin 10
* RST: Pin 9
*/


//------INITIALISE FOR RFID--------

#include <SPI.h>
#include <RFID.h>


RFID rfid(8,9);


int led = 7;
int power = 8; 
int serNum[5];
int cards[][5] = {
  {101,118,4,171,118}
  
};

bool access = false;

//-------INITIALISE FOR SD CARD---------------
#include <SD.h>
#include "RTClib.h"

File myFile;
RTC_DS1307 rtc;

//-------INITIALISE FOR 5V STEPPER MOTOR------

#include <Stepper.h>

const int stepsPerRevolution = 512;  

Stepper myStepper(stepsPerRevolution, 2, 5, 6, 7);


void setup(){


//------SET UP FOR RFID------
    Serial.begin(9600);
    SPI.begin();
    rfid.init();

    pinMode(led, OUTPUT);

    digitalWrite(led, LOW);
   
//-----SET UP FOR SD CARD-------

while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

   myFile = SD.open("PROJECT.csv", FILE_WRITE);
  Serial.print("DateTime,User,Access\n");
  if (myFile){
    myFile.println("DateTime,User,Access");
    myFile.close();
  }

rtc.begin();
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

//-----SET UP FOR 5V STEPPER MOTOR-----
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
}


void loop(){
  

   
    if(rfid.isCard()){
    
        if(rfid.readCardSerial()){

                  if(rfid.serNum[2] != 4) {
                      access = false;

                  } else {
                      access = true;
                  }           
        }
        
       if(access){
          myStepper.step(stepsPerRevolution);
          delay(100);
          myStepper.step(stepsPerRevolution);
          delay(5000);

          myStepper.step(-stepsPerRevolution);
          delay(100);
          myStepper.step(-stepsPerRevolution); 

   
    DateTime now = rtc.now();
        Serial.print(now.year(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print(" ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
          Serial.print(",");
          Serial.print("Lachlan");
          Serial.print(",");
          Serial.println("GRANTED");

      myFile = SD.open("PROJECT.csv", FILE_WRITE);
      if(myFile){
        myFile.print(now.year(), DEC);
        myFile.print('/');
        myFile.print(now.month(), DEC);
        myFile.print('/');
        myFile.print(now.day(), DEC);
        myFile.print(" ");
        myFile.print(now.hour(), DEC);
        myFile.print(':');
        myFile.print(now.minute(), DEC);
        myFile.print(':');
        myFile.print(now.second(), DEC);
        myFile.print(",");
        myFile.print("Lachlan");
        myFile.print(",");
        myFile.println("GRANTED");
        myFile.close();
      }

          
      } else { 
       
     DateTime now = rtc.now();
          Serial.print(now.year(), DEC);
          Serial.print('/');
          Serial.print(now.month(), DEC);
          Serial.print('/');
          Serial.print(now.day(), DEC);
          Serial.print(" ");
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.print(':');
          Serial.print(now.second(), DEC);
          Serial.print(",");
          Serial.print("Unidentified");
          Serial.print(",");
          Serial.println("DENIED"); 

        myFile = SD.open("PROJECT.csv", FILE_WRITE);
        if(myFile){
          myFile.print(now.year(), DEC);
          myFile.print('/');
          myFile.print(now.month(), DEC);
          myFile.print('/');
          myFile.print(now.day(), DEC);
          myFile.print(" ");
          myFile.print(now.hour(), DEC);
          myFile.print(':');
          myFile.print(now.minute(), DEC);
          myFile.print(':');
          myFile.print(now.second(), DEC);
          myFile.print(",");
          myFile.print("Unidentified");
          myFile.print(",");
          myFile.println("DENIED"); 
          myFile.close();
        }
          
          delay(1000);
                           
      }         
    }    
    rfid.halt();

}


