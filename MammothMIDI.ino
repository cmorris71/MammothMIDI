#include <U8g2lib.h>
#include <MIDI.h> // MIDI Library by Forty Seven Effects Version 4.3.1
#include <Wire.h>
#include <PushButton.h> // Include the PushButton library from https://github.com/kristianklein/PushButton
#include "config.h"

// State machine definitions
#define WAITING_STATE 0
#define PRE_BUTTON_STATE 1
#define CLICKED_BUTTON_STATE 2
#define HELD_BUTTON_STATE 3
#define DOUBLE_BUTTON_STATE 4

/*
  WIRING INSTRUCTIONS
    MIDI Socket Wiring:
     PIN 2 ON THE SOCKET GOES TO ONE OF THE GND PINS ON YOUR ARDUINO.
     PIN 4 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE 5V PIN ON THE ARDUINO.
     PIN 5 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE TX1 PIN ON THE ARDUINO.
*/



const byte midiPC = 0; //MIDI Program Change
const byte midiCC = 1; //MIDI Control Change

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// u8g2 display constructor
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Create pushbuttons on digital pins 2-9
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

int state = WAITING_STATE; //Initial state
int songNumber = 0; //Initial song number
byte active = 0; //Number of active switches

//========= Returns number for active switch starting with 'start' =========
int whichSwitch(int start = 0) {

  if (state == CLICKED_BUTTON_STATE)
  {
    //Serial.printlf("clicked button state");
    for (int i = start; i < 8; i++)
    {
      if (footSwitch[i].isReleased())
      {
        return (i + 1);
      }
    }
  }
  if (state == DOUBLE_BUTTON_STATE or state == HELD_BUTTON_STATE)
  {
    for (int i = start; i < 8; i++)
      //Serial.printlf("double/held button state");
    {
      if (footSwitch[i].isActive())
      {
        return (i + 1);
      }
    }
  }

  return -1;

}

//========= Displays current song =========
void showSong(String song) {
  int w = u8g2.getStrWidth(song.c_str());
  int x = 64 - w / 2;
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 12, "Song");
    u8g2.setFont(u8g2_font_inb33_mf);
    u8g2.drawStr(x, 60, song.c_str());
  } while ( u8g2.nextPage() );
}

//========= Execute action for double switch press =========
void doubleAction()
{
  int s1 = whichSwitch();
  int s2 = whichSwitch(s1);
  Serial.println("Double State " + String(s1) + " & " + String(s2));

  if (s1 == 1 and s2 == 2) songNumber--;
  if (s1 == 2 and s2 == 3) songNumber++;
  showSong(String(songNumber));
  //      MIDI.sendProgramChange(songNumber,HX_STOMP);
  //      MIDI.sendProgramChange(songNumber*2,TIMELINE);
  //      MIDI.sendProgramChange(songNumber*3,BIGSKY);
  
}

//========= Execute action for switch hold =========
void holdAction()
{
  int s = whichSwitch();
  Serial.println("Hold State on " + String(s));
}

//========= Execute action for single button click =========
void clickAction()
{
  int s = whichSwitch(0);
  Serial.println("Click State on " + String(s));
}


void setup() {

  // ========= Button Setup =========
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

  u8g2.begin(); //Initialize the display

}
//*****************************************************************************
void loop() {

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
  // DETERMINE NUMBER OF ACTIVE BUTTONS
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
  //  if (fsClicked[0] == true)
  //  {

  //  }

  // =====================================================================================================================
  /// ======= FOOTSWITCH 2 ================================================================================================
  /// =====================================================================================================================
  //  if (fsClicked[1] == true)
  //  {
  //    fsClicked[1] = false;
  //    songNumber--;
  //    Serial.println("FS2 Released");
  //    showSong(String(songNumber));
  //    //      MIDI.sendProgramChange(songNumber,HX_STOMP);
  //    //      MIDI.sendProgramChange(songNumber*2,TIMELINE);
  //    //      MIDI.sendProgramChange(songNumber*3,BIGSKY);
  //    readyToSendMidi[0] = false;
  //  }
  //
}
