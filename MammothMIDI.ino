#include <U8g2lib.h>
//#include <U8x8lib.h>
#include <MIDI.h>
#include <Wire.h>
#include <PushButton.h> // Include the PushButton library
#include "pedals.h"

MIDI_CREATE_DEFAULT_INSTANCE();

// u8g2 constructor
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

/* -------------------------------------- WELCOME MESSAGE AND WHERE TO BEGIN --------------------------------------

    YOU NEED 3 CONNECTIONS ON YOUR 5 PIN DIN (MIDI) SOCKET;
     PIN 2 ON THE SOCKET GOES TO ONE OF THE GND PINS ON YOUR ARDUINO. YOU CAN ALSO JUST WIRE THIS TO A GROUND ON A FOOTSWITCH.
     PIN 4 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE 5V PIN ON THE ARDUINO.
     PIN 5 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE TX1 PIN ON THE ARDUINO.

   FINALLY THE POWER SOCKET. TO POWER VIA A USUAL 9v GUITAR PEDAL POWER SUPPLY;
     CENTER PIN ON THE SOCKET GOES TO GND ON THE ARDUINO.
     OUTER SLEEVE ON THE SOCKED GOES TO VIN ON THE ARDUINO.
   THERE ARE TWO GROUNDS ON THE ARDUINO. ONE IS CONVENIENTLY LOCATED NEXT TO VIN.


   ------------------------------------------------------ THE CODE -----------------------------------------------------

   NOW BEFORE WE ACTUALLY GET TO CODING, WE NEED TO INCLUDE ALL THE CRAZY STUFF THAT WILL TRICK THE ARDUINO INTO
   FUNCTIONING AS A MIDI CONTROLLER. AT THE TOP OF THIS PROGRAM 'OPEN TOOLS>MANAGE LIBRARIES...'. SEARCH FOR 'MIDI LIBRARY'.
   ONE SHOULD POPUP CALLED 'MIDI Library by Forty Seven Effects Version 4.3.1' CLICK INSTALL.

   YOU WILL NEED KRISTIAN KLIEN'S PUSHBUTTON LIBRARY: https://github.com/kristianklein/PushButton

  -----------------------------------------------------------------------------------------------------------------------------*/


PushButton footSwitch[8] = {
  PushButton(2),
  PushButton(3),
  PushButton(4),
  PushButton(5),
  PushButton(6),
  PushButton(7),
  PushButton(8),
  PushButton(9)
};


int songNumber = 0; //STARTING SONG NUMBER
char message[12] = "Initializin";  //display message

//ARRAYS FOR FOOT SWITCH EVENTS
int fsCurrentState[8];
int fsPriorState[8];
bool fsClicked[8], fsReleased[8], fsDoubleClick[8], fsHold[8]; // BUTTON EVENT ARRAYS
bool virtualSwitchA = false, virtualSwitchB = false, virtualSwitchC = false, virtualSwitchD = false; // VIRTUAL BUTTONS (FOR BANK UP/DOWN)
bool oneShotSwitchA, oneShotSwitchB, oneShotSwitchC, oneShotSwitchD; // ONE-SHOT HOUSEKEEPING BIT

bool readyToSendMidi [10];

bool fsState [10];
unsigned long dbTime [10];
bool fsPush [10];
bool fsReady [10];

byte active = 0;

//*****************************************************************************
void setup() {

  // ========= Button Setup ======
  for (int i = 0; i < 8; i++) {
    pinMode(2 + i, INPUT_PULLUP);
    footSwitch[i].setActiveLogic(LOW);
    footSwitch[i].disableDoubleClick();
  }


  MIDI.begin(MIDI_CHANNEL_OFF);
  Serial.begin(31250);


  //setup serial port for monitoring
  Serial.begin(9600);
  while (! Serial); // Wait untilSerial is ready - Leonardo
  u8g2.begin();

}
//*****************************************************************************
void loop() {

  byte whichSwitch;

  // DETERMINE ACTIVE BUTTONS
  active = 0;
  for (int i = 0; i < 8; i++)
  {
    footSwitch[i].update();
    if (footSwitch[i].isActive()) {
      active++;
    }
  }

  //TWO BUTTON SWITCH EVENTS
  if (footSwitch[0].isActive() and footSwitch[1].isActive()) // 1ST AND 2ND FOOTSWTICHES ARE PRESSED
  {
    if (!oneShotSwitchA) { // ONE-SHOT TRIGGER OF VIRTUAL BUTTON ACTION
      oneShotSwitchA = true;
      virtualSwitchAAction(); // CALL ACTION
    }
  } else {
    oneShotSwitchA = false; //RESET ONE-SHOT
  }

  //SINGLE BUTTON HOLD EVENTS
  for (int i = 0; i < 8; i++)
  {
    if (active == 1 and footSwitch[i].isHeld())
    {
      holdAction(i);
    } else if (active == 0 and footSwitch[i].isReleased()) 
    {
        
          releaseAction(i);
        
      }
    }
  


  //DEBUG if(active != 0)Serial.println(active, BIN);
  //Serial.println("hello");   // ============= Processed Button Events ===============







  // ======= DISPLAY ============
  //show(message);
  ///=====================================================================================================================
  /// ======= FOOTSWITCH 1 ================================================================================================
  /// =====================================================================================================================
  if (fsClicked[0] == true)
  {
    fsClicked[0] = false;
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
  if (fsClicked[1] == true)
  {
    fsClicked[1] = false;
    songNumber--;
    Serial.println("FS2 Released");
    showSong(String(songNumber));
    //      MIDI.sendProgramChange(songNumber,HX_STOMP);
    //      MIDI.sendProgramChange(songNumber*2,TIMELINE);
    //      MIDI.sendProgramChange(songNumber*3,BIGSKY);
    readyToSendMidi[0] = false;
  }

}

//======== DISPLAY CLASSES ===========================================================

void showSong(String song) {
  //  int w = u8g2.getStrWidth(song.c_str());
  //  int x = 64 - w/2;
  //  u8g2.firstPage();
  //  do {
  //    u8g2.setFont(u8g2_font_ncenB10_tr);
  //    u8g2.drawStr(0,12,"Song");
  //    u8g2.setFont(u8g2_font_inb33_mf);
  //    u8g2.drawStr(x,60,song.c_str());
  //  } while ( u8g2.nextPage() );

}

// =====================================================================================================================
/// ======= FOOTSWITCH 1 & 2 ================================================================================================
/// =====================================================================================================================
void bankup12()
{
  fsClicked[1] = false;
  songNumber--;
  Serial.println("FS2 Released");
  showSong(String(songNumber));
  //      MIDI.sendProgramChange(songNumber,HX_STOMP);
  //      MIDI.sendProgramChange(songNumber*2,TIMELINE);
  //      MIDI.sendProgramChange(songNumber*3,BIGSKY);
  readyToSendMidi[0] = false;
}

void virtualSwitchAAction()
{
  Serial.println("Virtual Switch A One Shot Triggered");
}

void holdAction(byte switchNo)
{
  Serial.println("Switch " + String(switchNo) + " Held");
}


void releaseAction(byte switchNo)
{
  Serial.println("Switch " + String(switchNo) + " Clicked");
}
