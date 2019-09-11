// You can also wrap all exiting prints in #ifdef ENABLE_PRINT and #endif and then define or comment out #define ENABLE_PRINT at the top of ur file
//kapatmak icin ikinci parametreyi sil derleyici kullanilmadigi icin hepsini iptal edecek
#define debug(a)// (Serial.print(a))



/*
 * relay high oldugu zaman kapaniyor
 * kullandigim ucuz pro mini yandaki VCC ve GND pinlerinden beslenmek istiyor (RAW ve diger VCC problem cikartti)
 * firmware upload etmek icin elle resetlemek gerekiyor
 * relayi ledin oldugu bacaga baglamak iyi fikir degil kisa devre oldugunda led kapaniyor
 */

#include <TM1637Display.h>


//pins definitions for TM1637 and can be changed to other ports       
const int CLK = 10;
const int DIO = 11;
TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.



const int ledPin = 13;
const int rlyPin = 9;  //port 12 sanki port 13 e bagli gibi calisiyor, ayrica port 11 de calismadi
const int second = 1000;
const int buttonPin1 = 3;
const int buttonPin2 = 4;
const int buttonSwitch = 2; //kullanmadim bu dügmeyi


// parazit icin sayac
int ledState = LOW;  //ayni anda relay state
int buttonState;
int lastButtonState = LOW;

//
long int say = 0;

// Genel sayac
unsigned long previousMillis=0;


 int aState;
 int aLastState;  


void setup() {
    pinMode(buttonPin1, INPUT);
    pinMode(buttonPin2, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(rlyPin, OUTPUT);
    digitalWrite(ledPin, ledState);
    digitalWrite(rlyPin, !ledState);

    display.setBrightness(0);  //@BRIGHT_DARKEST = 0,BRIGHTEST = 7 / @true=Turn on display, false=Turn off display;
  
//    Serial.begin(9600);  
}

void loop() {
    //
    // Genel zaman sayaci, overflow protection
    //
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) >= second) {
        //digitalWrite(13, !digitalRead(13));
        previousMillis = currentMillis;
        //if (basbit) {
        if (say>0){
          say--;
          ledState = true;
        
          if (say == 0) {
            debug("zaman doldu\n");
            ledState = false;
            debug("------------\n");
          }
        }
    }




    
    
    aState = digitalRead(buttonPin1); // Reads the "current" state of the outputA

    // If the previous and the current state of the outputA are different, that means a Pulse has occured
    if (aState != aLastState){     
         
         // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
         if (digitalRead(buttonPin2) != aState) { 
           say += 10*60+1;
         } else {
           say -= 10*60;
         }
        
     
        if (say <= 0) {
            say = 6;
        } else if (say>43200) {  //max 12 saat
            say = 43200;
        }
    
    }
    
    aLastState = aState; // Updates the previous state of the outputA with the current state
    


    
    
    debug(zaman); 
    debug("!\n");

    debug(say);
    debug("\n");

    if (say>=60){      
        long int t = say;
        //int seconds = t % 60;
        t /= 60; // now it is minutes
        int minutes = t % 60;
        t /= 60; // now it is hours
        int hours = t % 24;    
        char sure[5];      
        sprintf(sure, "%02d%02d", hours, minutes);
        int r=atoi( sure );
        display.showNumberDec(r, true, 4); //Display the Variable value;
    }
    else
    {
        display.showNumberDec(say);
    }


    digitalWrite(ledPin, ledState);
    digitalWrite(rlyPin, !ledState);
    delayMicroseconds(800); 

}
/*
 int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};  
  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;
      tm1637.display(TimeDisp);
/
*/
