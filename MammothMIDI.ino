#include <U8g2lib.h>
#include <U8x8lib.h>
#include <MIDI.h>  
#include <Wire.h>
#include <PushButton.h> // Include the PushButton library
#include "pedals.h"                      


MIDI_CREATE_DEFAULT_INSTANCE();          
                                         
#define NUM_FOOTSWITCHES 10              
#define NUM_LEDS 10  



// u8g2 constructor
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 

//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************


/* -------------------------------------- WELCOME MESSAGE AND WHERE TO BEGIN --------------------------------------
 
   GREETINGS AND WELCOME TO CREATING YOUR VERY OWN MIDI FOOTSWITCH CONTROLLER! FIRST YOU NEED TO WIRE UP YOUR
   MIDI FOOTSWITCH.

   HERE IS HOW WE ARE GOING TO WIRE UP ALL OF YOUR LED'S AND FOOTSWITCHES! WIRE THEM AS FOLLOWS. THE CODE ALWAYS ASSUMES
   YOU ARE USING TEN FOOTSWITCHES (THE MAXIMUM FOR AN ARDUINO NANO) JUST STOP AT AS MANY FOOTSWITCHES AS YOU LIKE
   AND THE CODE WILL IGNORE THE REST.
   
     FS 1 - D2,  LED 1 - D3,         EACH LED WILL HAVE A LONG AND A SHORT LEG. YOU WILL NEED TO ATTACH A 220ohm
     FS 2 - D4,  LED 2 - D5,         RESISTOR TO THE LONG LEG. THE RESISTOR DOES NOT HAVE A CORRECT WAY ROUND. 
     FS 3 - D6,  LED 3 - D7,         THIS GIVES YOU    -PIN - RESISTOR - LED - GROUND
     FS 4 - D8,  LED 4 - D9,         
     FS 5 - D10,  LED 5 - D11,       THE FOOTSWITCHES ARE EASIER, ONE SIDE GOES TO THE APPROPRIATE PIN, THE 
     FS 6 - D12,  LED 6 - D13,       OTHER SIDE GOES TO GROUND.
     FS 7 - A0,  LED 7 - A1,         
     FS8 - A2,  LED 8 - A3,          FOR CONVENIENCE I JUST SOLDER THE GROUND OF EACH LED TO THE GROUND ON THE SWITCH
     FS9 - A4,  LED 9 - A5,          AND THEN SOLDER ALL OF THESE TOGETHER. THEN ATTACH A WIRE FROM A GND PIN ON THE ARDUINO
     FS10 - A6,  LED 10 - A7         TO THEN SOLDER THAT WIRE TO ONE OF THE FOOTSWITCH GROUNDS "DAISY CHAINING" THEM TOGETHER.

   THE KEEN EYED AMONGST YOU WILL HAVE NOTICED THAT I HAVE CONNECTED EVERYTHING IN SEQUENCE AND PAIRED UP EACH FOOTSWITCH AND LED. 
 
   YOU NEED 3 CONNECTIONS ON YOUR 5 PIN DIN (MIDI) SOCKET;
     PIN 2 ON THE SOCKET GOES TO ONE OF THE GND PINS ON YOUR ARDUINO. YOU CAN ALSO JUST WIRE THIS TO A GROUND ON A FOOTSWITCH.
     PIN 4 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE 5V PIN ON THE ARDUINO.
     PIN 5 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE TX1 PIN ON THE ARDUINO.

   FINALLY THE POWER SOCKET. TO POWER VIA A USUAL 9v GUITAR PEDAL POWER SUPPLY;
     CENTER PIN ON THE SOCKET GOES TO GND ON THE ARDUINO.
     OUTER SLEEVE ON THE SOCKED GOES TO VIN ON THE ARDUINO.
   THERE ARE TWO GROUNDS ON THE ARDUINO. ONE IS CONVENIENTLY LOCATED NEXT TO VIN.

   AFTER THIS YOU ARE READY FOR CODING!
   
   ------------------------------------------------------ THE CODE -----------------------------------------------------
   
   I HAVE ENDEAVORED TO MAKE THIS AS PAIN FREE AS POSSIBLE WITH AS LITTLE CODING EXPERIENCE NEEDED AS POSSIBLE. 
   TRY NOT TO BE TOO SCARED IF YOU HAVE NO EXPERIENCE AT ALL, ALTHOUGH THERE IS ALOT OF INFORMATION HERE WE ARE USING 
   VERY LITTLE OF IT.

   TO GET STARTED, HERE IS A QUICK OVERVIEW OF HOW THIS ARDUINO PROGRAM WORKS.

   ABOVE YOU WILL SEE 4 TABS, MUCH LIKE AN INTERNET BROWSER ^^^
   MIDI_FOOTSWITCH_CONTROLLER    -   THIS TAB YOU ARE LOOKING AT NOW
   A_DO_NOT_TOUCH                -   YOU CAN GUESS WHAT I WANT YOU TO DO WITH THIS ONE
   B_FOOTSWITCH_DECLARATION      -   THIS IS WHERE WE WILL DECLARE WHAT YOUR FOOTSWITCHES ACTUALLY DO WHEN PRESSED
   C_DO_NOT_TOUCH                -   YOU'VE PROBABLY GUESSED BY NOW

   IT'S VERY IMPORTANT THAT YOU LEAVE ALL OF THESE FILES INSIDE THE FOLDER AND DON'T RENAME THEM. DON'T EVEN RENAME THE
   FOLDER OTHERWISE THIS SOFTWARE CANNOT FIND EVERYTHING FOR YOUR PROGRAM TO WORK.

   I'VE TRIED TO HIDE MOST OF THE SCARY STUFF SO THAT A, IT'S NOT SCARY AND B, YOU'RE LESS LIKELY TO ACCIDENTALLY
   BREAK SOMETHING AND HAVE TO START ALL OVER AGAIN WITH A FRESH DOWNLOAD.

   NOW BEFORE WE ACTUALLY GET TO CODING, WE NEED TO INCLUDE ALL THE CRAZY STUFF THAT WILL TRICK THE ARDUINO INTO
   FUNCTIONING AS A MIDI CONTROLLER. AT THE TOP OF THIS PROGRAM 'OPEN TOOLS>MANAGE LIBRARIES...'. SEARCH FOR 'MIDI LIBRARY'.
   ONE SHOULD POPUP CALLED 'MIDI Library by Forty Seven Effects Version 4.3.1' CLICK INSTALL. 
   
   THAT SHOULD BE ALL THERE IS TO IT!

   NOW TO START CODING WE NEED TO DEFINE SOME GENERAL INFORMATION ABOUT YOUR SHINY NEW MIDI CONTROLLER.
   
 -----------------------------------------------------------------------------------------------------------------------------*/
// IF YOU FOLLOWED THE WIRING INSTRUCTIONS IN THE PDF FILE, THEN YOU ONLY HAVE TWO QUESTIONS TO ANSWER HERE;

PushButton footSwitch[6] = {
  PushButton(2),
  PushButton(3),
  PushButton(4),
  PushButton(5),
  PushButton(6),
  PushButton(7)
};

//ARE YOU USING TAP TEMPO? true OR false
bool tapTempoUsed = false;    //(MUST BE lowercase, will turn blue)

//WHICH FOOTSWITCH FOR TAP TEMPO? IF NONE = 0
int tapTempo = 0;    //REPLACE 0 WITH THE FOOTSWITCH NUMBER YOU ARE USING.


//IF YOU WIRED YOUR FOOTSWITCHES AND LED'S TO DIFFERENT PINS ON YOUR ARDUINO, YOU'LL NEED TO CHANGE THEM HERE.

//WHICH PINS HAVE YOU WIRED YOUR FOOTSWITCHES TO?
const byte FOOTSWITCH[NUM_FOOTSWITCHES] = { 2,  4,  6,  8, 10, 12, A0, A2, A4, A6};

//WHICH PINS HAVE YOU WIRED YOUR LED'S TO?
const byte LED[NUM_LEDS] = {3, 5, 7, 9, 11, 13, A1, A3, A5, A7};

int songNumber = 0; //STARTING SONG NUMBER
char message[12] = "Initializin";  //display message

//ARRAYS FOR FOOT SWITCH EVENTS
int fsCurrentState[6];
int fsPriorState[6];
bool fsClicked[6];
bool fsReleased[6];
bool fsDoubleClick[6];
bool fsHold[6];

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

  // ========= Button Setup ======
  for (int i = 0; i<6; i++){
    pinMode(2+i, INPUT_PULLUP);
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

   int tapRate;
   unsigned long currentTapMillis = millis();

   // ============= Raw Button Event Handling ============
  for(int i=0; i<6; i++)
  {
    footSwitch[i].update();
    if (footSwitch[i].isClicked()) fsClicked[i]  = true;// Click event
    if (footSwitch[i].isReleased())fsReleased[i] = true; // Release event
    if (footSwitch[i].isHeld())    fsHold[i]     = true; // Hold/long push event
   }

   // ============= Processed Button Events ===============
   

// ======= DISPLAY ============
  //show(message);
  ///=====================================================================================================================
/// ======= FOOTSWITCH 1 ================================================================================================
/// =====================================================================================================================
  if (fsClicked[0]== true)
  {
      fsClicked[0]=false;
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
  if (fsClicked[1]==true)
  {
      fsClicked[1] =false;
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
  //delay(1000);
}
