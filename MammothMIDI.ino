#include <U8g2lib.h>
//#include <U8x8lib.h>
#include <MIDI.h>
#include <Wire.h>
#include <PushButton.h> // Include the PushButton library
#include "pedals.h"

#define WAITING_STATE 0
#define PRE_BUTTON_STATE 1
#define CLICKED_BUTTON_STATE 2
#define HELD_BUTTON_STATE 3
#define DOUBLE_BUTTON_STATE 4

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
   YOU WILL NEED THE FOLLOWING LIBRARIES:
   1) KRISTIAN KLIEN'S PUSHBUTTON LIBRARY: https://github.com/kristianklein/PushButton
   2) MIDI Library by Forty Seven Effects Version 4.3.1

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

int state = WAITING_STATE;
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
byte old_active = 0;
byte old_state = 0;

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

/*-----------------------------------------------------------------------------
Button State Machine

The code below implements a 5-state, finite state machine for servicing button
events.

WAITING_STATE (initial state)
  No buttons pressed. Nothing to do.
  
PRE_BUTTON_STATE (entry only from WAITING_STATE)
  Button pressed. Waiting to see if this will be a double button, hold, or 
  single button clicked event
  
CLICKED_BUTTON_STATE (entry only from PRE_BUTTON_STATE)
  A single button has been clicked
  
HELD_BUTTON_STATE (entry only from PRE_BUTTON_STATE)
  A button was held
  
DOUBLE_BUTTON_STATE (entry only from PRE_BUTTON_STATE)
  A pair of bottons has been clicked

------------------------------------------------------------------------------*/
  // DETERMINE ACTIVE BUTTONS
  active = 0;
  for (int i = 0; i < 8; i++)
  {
    footSwitch[i].update();
    if (footSwitch[i].isActive())
    {
      active++;
    }
  }

   //CHECK FOR STATE WHERE NO BUTTONS ARE PRESSED
  if (active == 0 and state != PRE_BUTTON_STATE)
  {
    state = WAITING_STATE;
  }

  //CHECK FOR STATE WHERE 1 BUTTON HAS BEEN PRESSED
  if (state == WAITING_STATE and active == 1)
  {
    state =   PRE_BUTTON_STATE;
  }

  //TWO BUTTON SWITCH EVENTS
  if (state == PRE_BUTTON_STATE and active == 2)
  {
    state = DOUBLE_BUTTON_STATE;
    doubleAction();
    //    // ONE-SHOT TRIGGER OF VIRTUAL BUTTON ACTION
    //    if (!oneShotSwitch)
    //    {
    //      oneShotSwitch = true;
    //      virtualSwitchAction(); // CALL ACTION
    //    } else {
    //      oneShotSwitch = false; //RESET ONE-SHOT
    //    }
  }

  //HOLD BUTTON  EVENTS
  if (state == PRE_BUTTON_STATE)
  {
    for (int i = 0; i < 8; i++)
    {
      if (footSwitch[i].isHeld())
      {
        state = HELD_BUTTON_STATE;
        holdAction();
        break;
      }
    }
  }

  //SINGLE BUTTON EVENT
  if (state == PRE_BUTTON_STATE and active == 0)
  {
    for (int i = 0; i < 8; i++)
    {
      if (footSwitch[i].isReleased())
      {
        state = CLICKED_BUTTON_STATE;
        clickAction();
        break;
      }
    }
  }


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
    int w = u8g2.getStrWidth(song.c_str());
    int x = 64 - w/2;
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(0,12,"Song");
      u8g2.setFont(u8g2_font_inb33_mf);
      u8g2.drawStr(x,60,song.c_str());
    } while ( u8g2.nextPage() );

}


void doubleAction()
{
  Serial.println("Double State");
}

void holdAction()
{
  Serial.println("Hold State");
}


void clickAction()
{
  
  Serial.println("Click State");
}
