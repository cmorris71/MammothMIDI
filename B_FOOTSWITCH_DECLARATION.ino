/* ------------------------------------------- HOW TO SET A CC MESSAGE ------------------------------------------------- 
   THIS PAGE IS WHERE WE ARE GOING TO DECLARE WHAT OUR FOOTSWITCHES DO. FOR EACH FOOTSWITCH, ASSIGN 2 COMMAND AND 2 LED
   FUNCTIONS. SOME FEATURES NEED TWO COMMANDS TO WORK PROPERLY (FOR EXAMPLE STOP/PLAY ON THE LOOPER). MOST COMMANDS NEED
   THE SAME CC MESSAGE SENT FOR 'OFF' AND 'ON' HOWEVER WE STILL WANT TO BE ABLE TO CHANGE WHAT THE LED'S DO.
   
   FOR EXAMPLE, IF WE WANT OUR 'FOOTSWITCH 1' TO EMULATE STOMP FOOTSWITCH MODES FS1, WE'D CHANGE THE CC MESSAGE FOR BOTH
   COMMAND 1 & 2 TO - MIDI.sendControlChange(49,100,1); (CC#, VALUE, CHANNEL)

   THE NUMBERS IN BRACKETS ARE THE CC NUMBER (IN THIS CASE 49) THE VALUE (IN THIS CASE, ANY NUMBER BETWEEN 0-127) AND
   THE MIDI CHANNEL TO SEND THE MESSAGE ON (YOUR HELIX DEVICE BY DEFAULT IS SET TO LISTEN TO ALL MESSAGES. IF YOU HAVE
   CHANGED THIS AS YOU HAVE OTHER MIDI DEVICES PLUGGED IN, CHANGE THIS TO THE APPROPRIATE CHANNEL FOR YOUR SETUP)
   
   THIS WILL TOGGLE WHATEVER STOMP WE HAVE ASSIGNED TO FS1 ON AND OFF.

   ------------------------------------------- HOW TO SET AN LED FUNCTION ---------------------------------------------- 
   
   WE WOULD ALSO LIKE SOME LED INDICATION SO WE KNOW WHATS HAPPENING, COMMAND 1 IS THE DEFAULT 'OFF' STATE SO WE REMOVE 
   THE '//' FROM THE START OF THE //ledOff(1); COMMAND LIKE SO
     ledOff(1);
     
   THE NUMBER IN BRACKETS IS THE LED NUMBER, YOU SHOULDN'T HAVE TO CHANGE THIS UNLESS YOU ARE TRYING TO DO SOMETHING
   REALLY ODD.
    
   IN OUR CASE FOR STOMP FS1 EMULATION, WE WOULD LIKE COMMAND 2 TO BE LED ON, SO WE REMOVE THE '//' FROM //ledOn(1);
     ledOn(1);

   SIMPLE AS THAT!
   
   ------------------------------------------------- CHEAT SHEET ------------------------------------------------------
   HERE ARE MY RECOMMENDATIONS FOR SETTINGS;

   EMULATING STOMP FOOTSWITCHES - COMMAND 1 - (CC#, 100, 1) - ledOff, COMMAND 2 - (CC#, 100, 1) - ledOn
     FS1 = CC#49  FS2 = CC#50  FS3 = CC#51  FS4 = CC#52  FS5 = CC#53  FS7 = CC#54  
     FS8 = CC#55  FS9 = CC#56  FS10 = CC#57  FS11 = CC#58
   
   LOOPER RECORD/OVERDUB SWITCH - COMMAND 1 - (60,50,1) - ledOff,   COMMAND 2 - (60,100,1) - ledOn
   LOOPER PLAY/STOP SWITCH - COMMAND 1 - (61,50,1) - ledOff,   COMMAND 2 - (61,100,1) - ledOn
   LOOPER PLAY ONCE SWITCH - COMMAND 1 - (62,100,1) - flashLED,   COMMAND 2 - SAME
   LOOPER UNDO SWITCH - COMMAND 1 - (63,100,1) - flashLED,   COMMAND 2 - SAME
   LOOPER FORWARD/REVERSE SWITCH - COMMAND 1 - (65,50,1) - ledOff, COMMAND 2 - (65,100,1) - ledOn
   LOOPER FULL/HALF SPEED SWITCH - COMMAND 1 - (66,50,1) - ledOff, COMMAND 2 - (66,100,1) - ledOn
   LOOPER BLOCK ON/OFF - COMMAND 1 - (67,50,1) - ledOff, COMMAND 2 - (67,100,1) - ledOn

   SETLIST TOGGLE - USE THIS TO TOGGLE BETWEEN TWO SETLISTS. IF YOU ARE BUILDING THIS DEVICE JUST TO CHANGE SETLISTS,
   LET ME KNOW AND I'LL SEE WHAT I CAN DO, THE MAIN CODE WOULD NEED TO BE ALTERED.
   COMMAND 1 - (32,SETLIST#,1) - flashLED, COMMAND 2 - (32,SETLIST#,1) - flash led
   SETLIST # - SUBTRACT 1 FROM YOUR SELECTION (FOR EXAMPLE - SETLIST 3 = 2) - THIS IS STUPID BUT PROGRAMMING STARTS AT 0.
   
   TAP TEMPO - SEE TAB "MIDI_FOOTSWITCH_CONTROLLER". YOU DECLARE TAP TEMPO THERE THEN THIS SECTION FOR THAT FOOTSWITCH 
               IS IGNORED!
   
   TUNER SCREEN ON/OFF - COMMAND 1 - (68, 100, 1) - ledOff, COMMAND 2 - (68, 100, 1) - ledOn
   
   SNAPSHOT TOGGLE - YOU CAN TOGGLE BETWEEN TWO SNAPSHOTS HERE
   COMMAND 1 - (69, SNAPSHOT#, 1) - flashLED, COMMAND 2 - (69, SNAPSHOT#, 1) - flashLED
   SNAPSHOT# - SUBTRACT 1 FROM YOUR SELECTION (FOR EXAMPLE - SNAPSHOT 4 = 3) - THIS IS STUPID BUT PROGRAMMING STARTS AT 0.

   MULTIPLE SNAPSHOTS
   IF YOU ARE USING ONE FOOTSWITCH PER SNAPSHOT, MAKE BOTH COMMANDS THE SAME AS ABOVE. YOU MAY NEED TO ADD SOME ADDITIONAL
   ledOff COMMANDS TO TURN OFF OTHER SNAPSHOT LEDS. DO THIS AS FOLLOWS
       ledOn(#); - THE FOOTSWITCH LED YOU WANT ON
       ledOff(#); - ONE OF YOUR OTHER SNAPSHOTS
       ledOff(#); - ONE OF YOUR OTHER SNAPSHOTS ETC.

   YOU NEED TO COPY AND PASTE THE ledOff(#) FUNCTION INTO BOTH COMMANDS AND TELL IT TO TURN OFF YOUR OTHER SNAPSHOT LEDS.
   THIS SOUNDS TEDIOUS BUT IT IS THE QUICKEST WAY AND ONCE YOU'VE DONE IT ONCE YOU CAN COPY PASTE THE LED LIST TO OTHER
   FOOTSWITCHES.

   SIDE NOTE - THERE ARE A FEW POINTS IN THE CODE BELOW THAT LOOK LIKE TYPOS OR NONSENSE... DO NOT TOUCH ANYTHING!
   I CAN ASSURE YOU IT IS CORRECT AND NECESSARY FOR THE CODE TO FUNCTION CORRECTLY. 

   ONCE YOU HAVE FINISHED, RETURN BACK TO THE 'MIDI_FOOTSWITCH_CONTROLLER' TAB FOR THE UPLOAD PROCESS.

*///=====================================================================================================================
/// ======= FOOTSWITCH 1 ================================================================================================
/// =====================================================================================================================
  if (fsState[0] == 0 && readyToSendMidi[0] == true){

/// ========= COMMAND 1 ==========================================
      ///MIDI.sendControlChange(68,100,2);  //(CC#, VALUE, CHANNEL)
      songNumber++;
      MIDI.sendProgramChange(songNumber,HX_STOMP);
      MIDI.sendProgramChange(songNumber*2,TIMELINE);
      MIDI.sendProgramChange(songNumber*3,BIGSKY);
      readyToSendMidi[0] = false; // DO NOT TOUCH THIS
  }

  if (fsState[0] == 0 && fsReady[0] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(1);               //LED ON - Constructor is the (LED number)
      //ledOff(1);              //LED OFF - Constructor is the (LED number)
      flashLED(1, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(1, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[0] == 1 && readyToSendMidi[0] == true){  
/// ========= COMMAND 2 ==========================================
      ///MIDI.sendControlChange(68,100,2);  //(CC#, VALUE, CHANNEL)
      songNumber++;
      MIDI.sendProgramChange(songNumber,HX_STOMP);
      MIDI.sendProgramChange(songNumber*2,TIMELINE);
      MIDI.sendProgramChange(songNumber*3,BIGSKY);

      readyToSendMidi[0] = false; // DO NOT TOUCH THIS
      }

    if(fsState[0] == 1 && fsReady[0] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(1);               //LED ON - Constructor is the (LED number)
      //ledOff(1);              //LED OFF - Constructor is the (LED number)
      flashLED(1, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(1, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}// =====================================================================================================================
/// ======= FOOTSWITCH 2 ================================================================================================
/// =====================================================================================================================
  if (fsState[1] == 0 && readyToSendMidi[1] == true){

/// ========= COMMAND 1 ==========================================
      ///MIDI.sendControlChange(68,100,2);  //(CC#, VALUE, CHANNEL)
      songNumber--;
      MIDI.sendProgramChange(songNumber,HX_STOMP);
      MIDI.sendProgramChange(songNumber*2,TIMELINE);
      MIDI.sendProgramChange(songNumber*3,BIGSKY);
      readyToSendMidi[1] = false; // DO NOT TOUCH THIS
  }

  if (fsState[1] == 0 && fsReady[1] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(2);               //LED ON - Constructor is the (LED number)
      //ledOff(2);              //LED OFF - Constructor is the (LED number)
      flashLED(2, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(2, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[1] == 1 && readyToSendMidi[1] == true){  
/// ========= COMMAND 2 ==========================================
      ///MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      songNumber--;
      MIDI.sendProgramChange(songNumber,HX_STOMP);
      MIDI.sendProgramChange(songNumber*2,TIMELINE);
      MIDI.sendProgramChange(songNumber*3,BIGSKY);
      readyToSendMidi[1] = false; // DO NOT TOUCH THIS
      }

    if(fsState[1] == 1 && fsReady[1] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(2);               //LED ON - Constructor is the (LED number)
      //ledOff(2);              //LED OFF - Constructor is the (LED number)
      flashLED(2, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(2, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}// =====================================================================================================================
/// ======= FOOTSWITCH 3 ================================================================================================
/// =====================================================================================================================
  if (fsState[2] == 0 && readyToSendMidi[2] == true){

/// ========= COMMAND 1 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[2] = false; // DO NOT TOUCH THIS
  }

  if (fsState[2] == 0 && fsReady[2] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(3);               //LED ON - Constructor is the (LED number)
      //ledOff(3);              //LED OFF - Constructor is the (LED number)
      flashLED(3, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(3, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[2] == 1 && readyToSendMidi[2] == true){  
/// ========= COMMAND 2 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[2] = false; // DO NOT TOUCH THIS
      }

    if(fsState[2] == 1 && fsReady[2] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(3);               //LED ON - Constructor is the (LED number)
      //ledOff(3);              //LED OFF - Constructor is the (LED number)
      flashLED(3, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(3, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}// =====================================================================================================================
/// ======= FOOTSWITCH 4 ================================================================================================
/// =====================================================================================================================
  if (fsState[3] == 0 && readyToSendMidi[3] == true){

/// ========= COMMAND 1 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[3] = false; // DO NOT TOUCH THIS
  }

  if (fsState[3] == 0 && fsReady[3] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(4);               //LED ON - Constructor is the (LED number)
      //ledOff(4);              //LED OFF - Constructor is the (LED number)
      flashLED(4, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(4, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[3] == 1 && readyToSendMidi[3] == true){  
/// ========= COMMAND 2 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[3] = false; // DO NOT TOUCH THIS
      }

    if(fsState[3] == 1 && fsReady[3] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(4);               //LED ON - Constructor is the (LED number)
      //ledOff(4);              //LED OFF - Constructor is the (LED number)
      flashLED(4, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(4, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}// =====================================================================================================================
/// ======= FOOTSWITCH 5 ================================================================================================
/// =====================================================================================================================
  if (fsState[4] == 0 && readyToSendMidi[4] == true){

/// ========= COMMAND 1 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[4] = false; // DO NOT TOUCH THIS
  }

  if (fsState[4] == 0 && fsReady[4] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(5);               //LED ON - Constructor is the (LED number)
      //ledOff(5);              //LED OFF - Constructor is the (LED number)
      flashLED(5, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(5, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[4] == 1 && readyToSendMidi[4] == true){  
/// ========= COMMAND 2 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[4] = false; // DO NOT TOUCH THIS
      }

    if(fsState[4] == 1 && fsReady[4] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(5);               //LED ON - Constructor is the (LED number)
      //ledOff(5);              //LED OFF - Constructor is the (LED number)
      flashLED(5, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(5, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}// =====================================================================================================================
/// ======= FOOTSWITCH 6 ================================================================================================
/// =====================================================================================================================
  if (fsState[5] == 0 && readyToSendMidi[5] == true){

/// ========= COMMAND 1 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[5] = false; // DO NOT TOUCH THIS
  }

  if (fsState[5] == 0 && fsReady[5] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(6);               //LED ON - Constructor is the (LED number)
      //ledOff(6);              //LED OFF - Constructor is the (LED number)
      flashLED(6, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(6, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[5] == 1 && readyToSendMidi[5] == true){  
/// ========= COMMAND 2 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[5] = false; // DO NOT TOUCH THIS
      }

    if(fsState[5] == 1 && fsReady[5] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(6);               //LED ON - Constructor is the (LED number)
      //ledOff(6);              //LED OFF - Constructor is the (LED number)
      flashLED(6, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(6, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}// =====================================================================================================================
/// ======= FOOTSWITCH 7 ================================================================================================
/// =====================================================================================================================
  if (fsState[6] == 0 && readyToSendMidi[6] == true){

/// ========= COMMAND 1 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[6] = false; // DO NOT TOUCH THIS
  }

  if (fsState[6] == 0 && fsReady[6] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(7);               //LED ON - Constructor is the (LED number)
      //ledOff(7);              //LED OFF - Constructor is the (LED number)
      flashLED(7, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(7, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[6] == 1 && readyToSendMidi[6] == true){  
/// ========= COMMAND 2 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[6] = false; // DO NOT TOUCH THIS
      }

    if(fsState[6] == 1 && fsReady[6] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(7);               //LED ON - Constructor is the (LED number)
      //ledOff(7);              //LED OFF - Constructor is the (LED number)
      flashLED(7, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(7, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}// =====================================================================================================================
/// ======= FOOTSWITCH 8 ================================================================================================
/// =====================================================================================================================
  if (fsState[7] == 0 && readyToSendMidi[7] == true){

/// ========= COMMAND 1 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[7] = false; // DO NOT TOUCH THIS
  }

  if (fsState[7] == 0 && fsReady[7] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(8);               //LED ON - Constructor is the (LED number)
      //ledOff(8);              //LED OFF - Constructor is the (LED number)
      flashLED(8, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(8, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[7] == 1 && readyToSendMidi[7] == true){  
/// ========= COMMAND 2 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[7] = false; // DO NOT TOUCH THIS
      }

    if(fsState[7] == 1 && fsReady[7] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(8);               //LED ON - Constructor is the (LED number)
      //ledOff(8);              //LED OFF - Constructor is the (LED number)
      flashLED(8, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(8, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}// =====================================================================================================================
/// ======= FOOTSWITCH 9 ================================================================================================
/// =====================================================================================================================
  if (fsState[8] == 0 && readyToSendMidi[8] == true){

/// ========= COMMAND 1 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[8] = false; // DO NOT TOUCH THIS
  }

  if (fsState[8] == 0 && fsReady[8] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(9);               //LED ON - Constructor is the (LED number)
      //ledOff(9);              //LED OFF - Constructor is the (LED number)
      flashLED(9, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(9, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[8] == 1 && readyToSendMidi[8] == true){  
/// ========= COMMAND 2 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[8] = false; // DO NOT TOUCH THIS
      }

    if(fsState[8] == 1 && fsReady[8] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(9);               //LED ON - Constructor is the (LED number)
      //ledOff(9);              //LED OFF - Constructor is the (LED number)
      flashLED(9, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(9, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}// =====================================================================================================================
/// ======= FOOTSWITCH 10 ===============================================================================================
/// =====================================================================================================================
  if (fsState[9] == 0 && readyToSendMidi[9] == true){

/// ========= COMMAND 1 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[9] = false; // DO NOT TOUCH THIS
  }

  if (fsState[9] == 0 && fsReady[9] == true){      

      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(10);               //LED ON - Constructor is the (LED number)
      //ledOff(10);              //LED OFF - Constructor is the (LED number)
      flashLED(10, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(10, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

} 

  if (fsState[9] == 1 && readyToSendMidi[9] == true){  
/// ========= COMMAND 2 ==========================================
      MIDI.sendControlChange(68,100,1);  //(CC#, VALUE, CHANNEL)
      readyToSendMidi[9] = false; // DO NOT TOUCH THIS
      }

    if(fsState[9] == 1 && fsReady[9] == true){
      //LED FUNCTION   -  TO SELECT, DELETE THE TWO // AT THE START OF THE LINE
      //NOTE - ONLY SELECT 1!! ALSO NOTE, ALL TIMES ARE MILLISECONDS, 1000 = 1 SECOND ETC.
      
      //ledOn(10);               //LED ON - Constructor is the (LED number)
      //ledOff(10);              //LED OFF - Constructor is the (LED number)
      flashLED(10, 500);       //LED FLASH - Constructor is (LED number, flash time)
      //blinkLED(10, 200, 200);  //BLINK THE LED - Constructor is (LED number, time on, time off)

     
}}// ======================================================================================================================
//// =======================================================================================================================
////  =====================================================================================================================
