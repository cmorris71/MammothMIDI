#include <U8glib.h>
#include <MIDI.h> // MIDI Library by Forty Seven Effects Version 4.3.1
#include <Wire.h>
#include <Encoder.h>
#include <PushButton.h> // Include the PushButton library from https://github.com/kristianklein/PushButton
#include <EEPROM.h>
#include "globals.h"
#include "config.h"

/*
  WIRING INSTRUCTIONS
    MIDI Socket Wiring:
     PIN 2 ON THE SOCKET GOES TO ONE OF THE GND PINS ON YOUR ARDUINO.
     PIN 4 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE 5V PIN ON THE ARDUINO.
     PIN 5 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE TX1 PIN ON THE ARDUINO.
*/

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// Setup encoder. Best to use interrupt pins
Encoder myEnc(2, 3);

long oldPosition  = -999;

// u8g2 display constructor
//U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

//========= Returns number for active switch starting with 'start' =========
int whichSwitch(int start = 0) {

  if (state == CLICKED_BUTTON_STATE)
  {
    //Serial.printlf("clicked button state");
    for (int i = start; i < 8; i++)
    {
      if (footSwitch[i].isReleased())
      {
        return (i);
      }
    }
  }
  if (state == DOUBLE_BUTTON_STATE or state == HELD_BUTTON_STATE)
  {
    for (int i = start; i < 12; i++)
      //Serial.printlf("double/held button state");
    {
      if (footSwitch[i].isActive())
      {
        return (i);
      }
    }
  }

  return -1;

}

//========= Displays current song =========
/*
  void showSong(String song) {
  int w = u8g2.getStrWidth(song.c_str());
  int x = 64 - w / 2;
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 12, "Song");
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(x, 60, song.c_str());
  } while ( u8g2.nextPage() );
  }*/

// Initialize I2C buses using TCA9548A I2C Multiplexer
void tcaSelect(uint8_t i2c_bus) {
  if (i2c_bus > 7) return;
  Wire.beginTransmission(MUX_Address);
  Wire.write(1 << i2c_bus);
  Wire.endTransmission();
}


// Initialize the displays
void displayInit() {
  for (int i = 0; i < 4; i++) {
    tcaSelect(i);   // Loop through each connected displays on the I2C buses
    u8g.begin();  // Initialize display
  }
}


void updateDisplays() {
  for (int i = 0; i < 4; i++) {
    tcaSelect(i);
    if (debug) Serial.println("Updating Displays");
    u8g.firstPage();
    do {
      /******** Display Button Text  *********/
      u8g.setFont(u8g_font_helvB12);
      u8g.setFontPosBaseline();
      u8g.drawStr(64 - u8g.getStrWidth(displays[i][0]) / 2, 26, displays[i][0]);
      u8g.setFont(u8g_font_helvB12);
      u8g.setFontPosBaseline();
      u8g.drawStr(64 - u8g.getStrWidth(displays[i][1]) / 2, 60, displays[i][1]);
      u8g.drawHLine(0, 35, 128);
      u8g.drawHLine(0, 36, 128);
      u8g.drawHLine(0, 37, 128);
      /***************************************/
    } while ( u8g.nextPage() );
    delay(50);
  }
}
void updateDisplay(int disp) {
  tcaSelect(disp);

  u8g.firstPage();
  do {

    //u8g.setFont(u8g_font_helvB12);
    //u8g.setFontPosBaseline();
    u8g.drawStr(64 - u8g.getStrWidth(displays[disp][0]) / 2, 26, displays[disp][0]);
  } while ( u8g.nextPage() );
  //delay(10);
}

void sendSong() {
  midiPC(song, HX_STOMP);
  midiPC(song * 2, TIMELINE);
  midiPC(song * 3, BIGSKY);
}

void megaDisplay () {


}
//========= Execute action for double switch press =========
void doubleAction()
{
  int s1 = whichSwitch();
  int s2 = whichSwitch(s1 + 1);
  //Serial.println(String(s1) + " " + String(s2));
  doubleActions[s1][s2]();
}

//========= Execute action for switch hold =========
void holdAction()
{
  int s = whichSwitch();
  if (debug) Serial.println("hold action fired :" + String(s));
  holdActions[s]();
}

//========= Execute action for single button click =========
void clickAction()
{
  int s = whichSwitch(0);
  clickActions[page][s]();
  if (debug)Serial.println("clickAction Detected" + String(page) + " " + String(s));
}

void midiPC(int songNumber, int midiChannel) {
  MIDI.sendProgramChange(songNumber, midiChannel);
}
void midiCC(int midiControler, int midiValue, int midiChannel) {
  MIDI.sendControlChange(midiControler, midiValue, midiChannel);
}

void setup() {

  // ========= Button Setup =========
  for (int i = 0; i < 8; i++) {
    pinMode(5 + i, INPUT_PULLUP);
    footSwitch[i].setActiveLogic(LOW);
    footSwitch[i].disableDoubleClick();
    footSwitch[i].setHoldTime(500);
  }

  MIDI.begin(MIDI_CHANNEL_OFF);
  //Serial.begin(31250);

  //setup serial port for monitoring
  Serial.begin(9600);
  while (! Serial); // Wait untilSerial is ready - Leonardo

  displayInit(); // Initialize the displays

  p1_display();

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

  //DETECT STATE WHERE NO BUTTONS ARE PRESSED
  if (active == 0 and state != PRE_BUTTON_STATE)
  {
    state = WAITING_STATE;
    //if(debug) Serial.println("waiting state");
  }

  //DETECT STATE WHERE 1 BUTTON HAS BEEN PRESSED
  if (state == WAITING_STATE and active == 1)
  {
    state =   PRE_BUTTON_STATE;
    if (debug) Serial.println("pre button state");
  }

  //DETECT STATE WHERE TWO BUTTONS HAVE BEEN PRESSED
  if (state == PRE_BUTTON_STATE and active == 2)
  {
    state = DOUBLE_BUTTON_STATE;
    if (debug) Serial.println("double button state");
    doubleAction();
  }

  //DETECT STATE WHERE ACTIVITY IS IN PROGRESS
  if (state == PRE_BUTTON_STATE)
  {
    for (int i = 0; i < 8; i++)
    {
      if (footSwitch[i].isHeld())
      {
        state = HELD_BUTTON_STATE;
        if (debug) Serial.println("held button state");
        holdAction();
        break;
      }
    }
  }

  //DETECT STATE WHERE ONE BUTTON HAS BEEN PRESSED
  if (state == PRE_BUTTON_STATE and active == 0)
  {
    for (int i = 0; i < 8; i++)
    {
      if (footSwitch[i].isReleased())
      {
        state = CLICKED_BUTTON_STATE;
        if (debug) Serial.println("clicked button state");
        clickAction();
        break;
      }
    }
  }

  // =============== Encoder ============
  long newPosition = myEnc.read();
  if (newPosition != oldPosition * 4)
  {
    oldPosition = newPosition / 4;
    Serial.println("Inside encoder loop now");
    sprintf(displays[0][0], "%d", oldPosition);
    //displays[0][0] =oldPosition;
    updateDisplay(0);
    Serial.println(newPosition);
  }
}
