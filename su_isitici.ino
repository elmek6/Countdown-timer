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


const int buttonPin2 = 2;
const int buttonPin1 = 3;
const int buttonPin4 = 4;
const int buttonPin3 = 5;
const int ledPin = 13;
const int rlyPin = 12;
const int second = 1000;


// parazit icin sayac
int ledState = LOW;  //ayni anda relay state
int buttonState;
int lastButtonState = LOW;

//
long int say = 0;
long int zaman = 0;
//bool basbit = true;  //pullup kullanildigi icin ilk deger true

// Genel sayac
unsigned long previousMillis=0;

//
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 15;


void setup() {
//  lcd.begin(2, 16);          // set up the LCD's number of columns and rows:
  //lcd.print("LCD Display");  // Print a message to the LCD.
  
    pinMode(buttonPin1, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLUP);
    pinMode(buttonPin3, INPUT_PULLUP);
    pinMode(buttonPin4, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    pinMode(rlyPin, OUTPUT);
    digitalWrite(ledPin, ledState);
    digitalWrite(rlyPin, !ledState);

    display.setBrightness(0);  //@BRIGHT_DARKEST = 0,BRIGHTEST = 7 / @true=Turn on display, false=Turn off display;
  
    Serial.begin(9600);  
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

  
    int deger = 0;
    //
    // Parazit icin tus vurusu algilama döngüsü
    //
    if (!digitalRead(buttonPin1)) deger = 60*20+1;       // 20 dakika
    if (!digitalRead(buttonPin2)) deger = 60*60+1;       // 1 saat
    if (!digitalRead(buttonPin3)) deger = 60*60*4+1;     // 4 saat
    if (!digitalRead(buttonPin4)) deger = 1;

    int reading=1;

    if (deger>0) {
        reading = 0;
        zaman = deger;

        if (deger == 1) {
          say = 0;
          zaman = 0;
          ledState = LOW;
        }

    }
    
    debug(zaman); 
    debug("!\n");

    if (reading != lastButtonState) {
      lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) > debounceDelay) {

        if (reading != buttonState) {   // if the button state has changed:
            buttonState = reading;
            debug("bounce\n");

            if (buttonState == HIGH) {
                say += zaman;
                if (say>43200)  //max 12 saat
                {
                    say = 43200+1;
                }
               
            }
        }
    }


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
    lastButtonState = reading;
    //delay(100);

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
