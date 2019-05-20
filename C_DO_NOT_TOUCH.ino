//*****************************************************************************************************************
//************************** DO NOT TOUCH ANYTHING ON THIS TAB! ***************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************


// ======= LED CLASSES ================================================================

// LED ON FUNCTION
void ledOn(int ledNo) {
  int ledNumber = (ledNo-1);
  digitalWrite (LED[ledNumber], HIGH);
}

// LED OFF FUNCTION
void ledOff(int ledNo) {
  int ledNumber = (ledNo-1);
  digitalWrite (LED[ledNumber], LOW);
}

// FLASHING LED FUNCTION
void flashLED(int ledNo, unsigned long flashTime) {
   int ledNumber = (ledNo-1);
   flashTiming[ledNumber] = (flashTime);
   
   flashCurrentMillis[ledNumber] = millis();
      
   if ((flashCurrentMillis[ledNumber] - flashButtonPushedMillis[ledNumber]) < flashTiming[ledNumber]) {
   digitalWrite(LED[ledNumber], HIGH);  //LED is turn on
   flashLedReady[ledNumber] = true; // LED IS ON
   flashLedTurnedOnAt[ledNumber] = millis(); //time LED was turned on
   flashLedState[ledNumber] = true; // LED IS ON
   flashLedReady[ledNumber] = false;
   }
   
   if(flashLedState[ledNumber] == true && (millis() - flashLedTurnedOnAt[ledNumber]) >= flashTiming[ledNumber]) {
      digitalWrite(LED[ledNumber], LOW);
      flashLedState[ledNumber] = false;
     } 

 }

// BLINK LED FUNCTION
void blinkLED (int ledNo, unsigned long timeOn, unsigned long timeOff) {
  int ledNumber = (ledNo-1);
  static int timer=timeOn;
  int blinkArray = ledNumber;

     if(( millis() - blinkMillisArray[blinkArray]) >= timer) {

      if (digitalRead(LED[ledNumber]) == HIGH) {
      timer = timeOff;    }
      else {
        timer = timeOn;   }

     digitalWrite(LED[ledNumber], !digitalRead(LED[ledNumber]));
     blinkMillisArray[blinkArray] = millis();
     }
}

//TAP TEMPO MATHS
unsigned long tTempo(unsigned long &ttTime1, unsigned long &ttTime2) { 
  unsigned long tapArray[2];

  if (ttState == 0) {
    tapArray[0] = millis();
    ttTime1 = (tapArray[0] - lastTap[1]);
    lastTap[0] = millis();
    ttState = 1;
  }

  else if (ttState == 1) {
    tapArray[1] = millis();
    ttTime2 = (tapArray[1] - lastTap[0]);
    lastTap[1] = millis();
    ttState = 0;
  }

}

//TAP TEMPO LED
void tapLED (int ledNo, unsigned long timeOn, unsigned long timeOff) {
  int ledNumber = (ledNo-1);
  int timer = timeOn;
  int currentTapMillis;

     if(( millis() - currentTapMillis) >= timer) {

      if (digitalRead(LED[ledNumber]) == HIGH) {
      timer = timeOff;    }
      else {
        timer = timeOn;   }

     digitalWrite(LED[ledNumber], !digitalRead(LED[ledNumber]));
     currentTapMillis = millis();
     }
}
