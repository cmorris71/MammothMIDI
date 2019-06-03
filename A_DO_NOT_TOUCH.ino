//*****************************************************************************************************************
//************************** DO NOT TOUCH ANYTHING ON THIS TAB! ***************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************

unsigned int debounceTime = 10;
bool readyToSendMidi [10];

bool fsState [10];
unsigned long dbTime [10];
bool fsPush [10];
bool fsReady [10];

unsigned long blinkMillisArray [10];

unsigned long flashButtonPushedMillis [10];
unsigned long flashCurrentMillis [10];
bool flashLedReady [10];
bool flashLedState [10];
unsigned long flashLedTurnedOnAt [10];
unsigned long flashTiming[10];

int ttState = 0;
unsigned long lastTap[2];
unsigned long tapIntervalTime;
unsigned long tapTime1;
unsigned long tapTime2;

//TAP LED
unsigned long previousTapMillis = 0;
int tapLedState = LOW;

//*****************************************************************************
void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);
  Serial.begin(31250);
  const int ledPin =  13;      // the number of the LED 

  //setup serial port for monitoring
  //Serial.begin(9600);
  while (! Serial); // Wait untilSerial is ready - Leonardo
  u8g2.begin(); 

  for (int i = 0; i < NUM_FOOTSWITCHES; i++) {  pinMode(FOOTSWITCH[i] , INPUT_PULLUP);  }
  for (int j = 0; j < NUM_LEDS; j++) {  pinMode(LED[j], OUTPUT);  digitalWrite(LED[j], LOW); }
  
  for (int k = 0; k < 10; k++) {   dbTime[k] = 0;   }
  for (int l = 0; l < 10; l++) {   fsPush[l] = HIGH;   }
  for (int m = 0; m < 10; m++) {   fsState[m] = 0;   }  
  
  for (int n = 0; n < 10; n++) {   flashLedReady[n] = false;   }
  for (int o = 0; o < 10; o++) {   flashLedState[o] = false;   }
  for (int p = 0; p < 10; p++) {   fsReady[p] = false;   }
  for (int q = 0; q < 10; q++) {   readyToSendMidi[q] = false;   }    

  lastTap[0] = 0;
  lastTap[1] = 0;
  tapTime1 = 500;
  tapTime2 = 500;

  //testing
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}
//*****************************************************************************
void loop() {

   int tapRate;
   unsigned long currentTapMillis = millis();

// ======= DISPLAY ============
  //show(message);
// ======= FOOTSWITCH 1 ==============================
    if (digitalRead(FOOTSWITCH[0]) != fsPush[0]) {
    fsPush[0] = digitalRead(FOOTSWITCH[0]);
    if (fsPush[0] == LOW) {
        dbTime[0] = millis();
    } else {
        dbTime[0] = 0;
    }
}

if (dbTime[0] > 0 && (millis() - dbTime[0] > debounceTime)) {
    
    dbTime[0] = 0;


      flashButtonPushedMillis[0] = millis();

     if (tapTempo !=1) {
      fsReady[0] = true;
      readyToSendMidi[0] = true;
      fsState[0] = !fsState[0]; 
      }

     if (tapTempo == 1) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }

}

// ======= FOOTSWITCH 2 ==============================
    if (digitalRead(FOOTSWITCH[1]) != fsPush[1]) {
    fsPush[1] = digitalRead(FOOTSWITCH[1]);
    if (fsPush[1] == LOW) {
        dbTime[1] = millis();
    } else {
        dbTime[1] = 0;
    }
}

if (dbTime[1] > 0 && (millis() - dbTime[1] > debounceTime)) {
    
    dbTime[1] = 0;


      flashButtonPushedMillis[1] = millis();

     if (tapTempo !=2) {
      fsReady[1] = true;
      readyToSendMidi[1] = true;
      fsState[1] = !fsState[1]; 
      }

     if (tapTempo == 2) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }

}

// ======= FOOTSWITCH 3 ==============================
    if (digitalRead(FOOTSWITCH[2]) != fsPush[2]) {
    fsPush[2] = digitalRead(FOOTSWITCH[2]);
    if (fsPush[2] == LOW) {
        dbTime[2] = millis();
    } else {
        dbTime[2] = 0;
    }
}

if (dbTime[2] > 0 && (millis() - dbTime[2] > debounceTime)) {
    
    dbTime[2] = 0;


      flashButtonPushedMillis[2] = millis();

     if (tapTempo !=2) {
      fsReady[2] = true;
      readyToSendMidi[2] = true;
      fsState[2] = !fsState[2]; 
      }

     if (tapTempo == 3) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }

}

// ======= FOOTSWITCH 4 ==============================
    if (digitalRead(FOOTSWITCH[3]) != fsPush[3]) {
    fsPush[3] = digitalRead(FOOTSWITCH[3]);
    if (fsPush[3] == LOW) {
        dbTime[3] = millis();
    } else {
        dbTime[3] = 0;
    }
}

if (dbTime[3] > 0 && (millis() - dbTime[3] > debounceTime)) {
    
    dbTime[3] = 0;


      flashButtonPushedMillis[3] = millis();

     if (tapTempo !=4) {
      fsReady[3] = true;
      readyToSendMidi[3] = true;
      fsState[3] = !fsState[3]; 
      }

     if (tapTempo == 4) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }

}

// ======= FOOTSWITCH 5 ==============================
    if (digitalRead(FOOTSWITCH[4]) != fsPush[4]) {
    fsPush[4] = digitalRead(FOOTSWITCH[4]);
    if (fsPush[4] == LOW) {
        dbTime[4] = millis();
    } else {
        dbTime[4] = 0;
    }
}

if (dbTime[4] > 0 && (millis() - dbTime[4] > debounceTime)) {
    
    dbTime[4] = 0;


      flashButtonPushedMillis[4] = millis();

     if (tapTempo !=5) {
      fsReady[4] = true;
      readyToSendMidi[4] = true;
      fsState[4] = !fsState[4]; 
      }

     if (tapTempo == 5) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }

}

// ======= FOOTSWITCH 6 ==============================
    if (digitalRead(FOOTSWITCH[5]) != fsPush[5]) {
    fsPush[5] = digitalRead(FOOTSWITCH[5]);
    if (fsPush[5] == LOW) {
        dbTime[5] = millis();
    } else {
        dbTime[5] = 0;
    }
}

if (dbTime[5] > 0 && (millis() - dbTime[5] > debounceTime)) {
    
    dbTime[5] = 0;


      flashButtonPushedMillis[5] = millis();

     if (tapTempo !=6) {
      fsReady[5] = true;
      readyToSendMidi[5] = true;
      fsState[5] = !fsState[5]; 
      }

     if (tapTempo == 6) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }

}

// ======= FOOTSWITCH 7 ==============================
    if (digitalRead(FOOTSWITCH[6]) != fsPush[6]) {
    fsPush[6] = digitalRead(FOOTSWITCH[6]);
    if (fsPush[6] == LOW) {
        dbTime[6] = millis();
    } else {
        dbTime[6] = 0;
    }
}

if (dbTime[6] > 0 && (millis() - dbTime[6] > debounceTime)) {
    
    dbTime[6] = 0;


      flashButtonPushedMillis[6] = millis();

     if (tapTempo !=7) {
      fsReady[6] = true;
      readyToSendMidi[6] = true;
      fsState[6] = !fsState[6]; 
      }

     if (tapTempo == 7) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }

}


// ======= FOOTSWITCH 8 ==============================
    if (digitalRead(FOOTSWITCH[7]) != fsPush[7]) {
    fsPush[7] = digitalRead(FOOTSWITCH[7]);
    if (fsPush[7] == LOW) {
        dbTime[7] = millis();
    } else {
        dbTime[7] = 0;
    }
}

if (dbTime[7] > 0 && (millis() - dbTime[7] > debounceTime)) {
    
    dbTime[7] = 0;


      flashButtonPushedMillis[7] = millis();

     if (tapTempo !=8) {
      fsReady[7] = true;
      readyToSendMidi[7] = true;
      fsState[7] = !fsState[7]; 
      }

     if (tapTempo == 8) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }

}

// ======= FOOTSWITCH 9 ==============================
    if (digitalRead(FOOTSWITCH[8]) != fsPush[8]) {
    fsPush[8] = digitalRead(FOOTSWITCH[8]);
    if (fsPush[8] == LOW) {
        dbTime[8] = millis();
    } else {
        dbTime[8] = 0;
    }
}

if (dbTime[8] > 0 && (millis() - dbTime[8] > debounceTime)) {
    
    dbTime[8] = 0;


      flashButtonPushedMillis[8] = millis();

     if (tapTempo !=9) {
      fsReady[8] = true;
      readyToSendMidi[8] = true;
      fsState[8] = !fsState[8]; 
      }

     if (tapTempo == 9) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }

}

// ======= FOOTSWITCH 10  =============================
    if (digitalRead(FOOTSWITCH[9]) != fsPush[9]) {
    fsPush[9] = digitalRead(FOOTSWITCH[9]);
    if (fsPush[9] == LOW) {
        dbTime[9] = millis();
    } else {
        dbTime[9] = 0;
    }
}

if (dbTime[9] > 0 && (millis() - dbTime[9] > debounceTime)) {
    
    dbTime[9] = 0;


      flashButtonPushedMillis[9] = millis();

     if (tapTempo !=10) {
      fsReady[9] = true;
      readyToSendMidi[9] = true;
      fsState[9] = !fsState[9]; 
      }

     if (tapTempo == 10) {
      tTempo(tapTime1, tapTime2);
      MIDI.sendControlChange(64,100,1);  //(CC#, VALUE, CHANNEL)
      currentTapMillis = millis();
      }
}

//======= TAP TEMPO FLAG ==============================
  if (tapTempoUsed == true) {
    int ledNo = (tapTempo-1);
    int OnTime = 500;
   tapIntervalTime = ((tapTime1 + tapTime2) /4);

    
  if((tapLedState == HIGH) && (currentTapMillis - previousTapMillis >= tapIntervalTime))
  {
    tapLedState = LOW;  // Turn it off
    previousTapMillis = currentTapMillis;  // Remember the time
    digitalWrite(LED[ledNo], tapLedState);  // Update the actual LED
  }
  else if ((tapLedState == LOW) && (currentTapMillis - previousTapMillis >= tapIntervalTime))
  {
    tapLedState = HIGH;  // turn it on
    previousTapMillis = currentTapMillis;   // Remember the time
    digitalWrite(LED[ledNo], tapLedState);    // Update the actual LED
  }
}
