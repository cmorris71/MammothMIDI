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
  if (fsReleased[0])
  {
      fsReleased[0] =0;
      songNumber++;
      Serial.println("FS1 Released");
      showSong(String(songNumber));
//      MIDI.sendProgramChange(songNumber,HX_STOMP);
//      MIDI.sendProgramChange(songNumber*2,TIMELINE);
//      MIDI.sendProgramChange(songNumber*3,BIGSKY);
      readyToSendMidi[0] = false; 
  }

// =====================================================================================================================
/// ======= FOOTSWITCH 2 ================================================================================================
/// =====================================================================================================================
  if (fsReleased[1])
  {
      fsReleased[1] =0;
      songNumber--;
      Serial.println("FS2 Released");
      showSong(String(songNumber));
//      MIDI.sendProgramChange(songNumber,HX_STOMP);
//      MIDI.sendProgramChange(songNumber*2,TIMELINE);
//      MIDI.sendProgramChange(songNumber*3,BIGSKY);
      readyToSendMidi[0] = false; 
  }
}
