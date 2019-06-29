/* LEFTOFF
MIDI commands wern't being sent. verify WhichSwitch and midiPC/CC are working
*/

#include <U8glib.h>
#include <MIDI.h> // MIDI Library by Forty Seven Effects Version 4.3.1
#include <Wire.h>
#include <Encoder.h>
#include <PushButton.h> // Include the PushButton library from https://github.com/kristianklein/PushButton
#include <extEEPROM.h>    //https://github.com/PaoloP74/extEEPROM
#include "globals.h"
#include "config.h"

/*
WIRING INSTRUCTIONS
	MIDI Socket Wiring:
	PIN 2 ON THE SOCKET GOES TO ONE OF THE GND PINS ON YOUR ARDUINO.
	PIN 4 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE 5V PIN ON THE ARDUINO.
	PIN 5 ON THE SOCKET GOES TO A 220ohm RESISTOR AND THEN TO THE TX1 PIN ON THE ARDUINO.
*/


//Setup external 24LC256 EEPROMs on the bus
const uint32_t totalKBytes = 32;         //for read and write test functions
extEEPROM eep(kbits_256, 1, 64);         //device size, number of devices, page size
uint32_t addr = 0;


// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// Setup encoder. Best to use interrupt pins
// Encoder myEnc(2, 3);

long oldPosition  = -999;

// u8g2 display constructor
//U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

//========= Returns number for active switch starting with 'start' =========
int whichSwitch(int start = 0) {
	if (debug) Serial.println("Entered Which Switch");
	if (state == CLICKED_BUTTON_STATE)
	{
		
		for (int i = start; i < 8; i++)
		{
			if (footSwitch[i].isReleased())
			{
				if (debug) Serial.print(F("Returning button: "));
				if (debug) Serial.print(i);
				delay(20);
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

	Serial.println(F("WhichSwitch returned no switch"));
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
	for (int i = 0; i < 6; i++) {
		tcaSelect(i);   // Loop through each connected displays on the I2C buses
		u8g.begin();  // Initialize display
	}
}

void printConfig(){
	Serial.println(F("Printing Config"));
	for (int pcbutton = 0; pcbutton < NUMBER_OF_BUTTONS; pcbutton++){
		Serial.println(String(pcbutton));
		for (int act = 0; act < ACTIONS_PER_BUTTON; act++){
			Serial.print(String(buttonActions[pcbutton][act].command)+ ".");
			Serial.print(String(buttonActions[pcbutton][act].param1) + ".");
			Serial.print(String(buttonActions[pcbutton][act].param2) + ".");
			Serial.print(String(buttonActions[pcbutton][act].param3) + ".");
			Serial.print(String(buttonActions[pcbutton][act].param4) + ".");
			Serial.print(String(buttonActions[pcbutton][act].param5) + ".");
			Serial.print(String(buttonActions[pcbutton][act].param6) + ".");
			Serial.println(String(buttonActions[pcbutton][act].param7));
		}
	}
}

void printBytes(byte read[8]){
	
	Serial.println(String(read[2]));
	// Serial.println(	"    " +String(read[0]) +"."+
	// String(read[1]) + "."+
	// String(read[2]) + "."+
	// String(read[3]) + "."+
	// String(read[4]) + "."+
	// String(read[5]) + "."+
	// String(read[6]) + "."+
	// String(read[7]));
}



void getConfig(){
	//read button actions from EEPROM
	char str[bytesPerString];
	word eepromAddress = (page-1) * BYTES_PER_PAGE;
	//if (debug Serial.println(eepromAddress);//Serial.print(String(testVar));
	byte configAction[SIZE_OF_ACTION];

	//if (debug Serial.println(F("Getting configuration."));
	
	for (byte configButton = 0; configButton < NUMBER_OF_BUTTONS; configButton++){
		for (byte actionNo=0; actionNo< ACTIONS_PER_BUTTON-6; actionNo++){
			eep.read(eepromAddress, configAction, SIZE_OF_ACTION);
			delay(10);
			if (configAction[0] != NO_COMMAND){
				//printBytes(configAction);
				//array -> Struct; sturct element boundaries must lie on byte boundaries
				memcpy(&buttonActions[configButton][actionNo], configAction, SIZE_OF_ACTION); 
				//Serial.println(String(buttonActions[configButton][actionNo].param3));
			}
			eepromAddress += SIZE_OF_ACTION;
		}
		
		
		//read display text from EEPROM
		eepromAddress = stringAddrStart 
		+ (page-1) *  stringBytesPerPage
		+ configButton * stringBytesPerButton;
		
		eep.read(eepromAddress, str, bytesPerString);
		memcpy(&displays[configButton][0], str, bytesPerString);
		eepromAddress += bytesPerString;
		eep.read(eepromAddress, str, bytesPerString);
		memcpy(&displays[configButton][1], str, bytesPerString);
		//Serial.print(("Button")+ String(configButton) + ": ");
		//delay(20);
		//Serial.println(str);
		//delay(20);
		//displays[configButton][0] = str;

	}
	updateDisplays();
	////if (debug printConfig();
}

void updateDisplays() {
	for (int i = 0; i < 6; i++) {
		tcaSelect(i);
		//if (debug Serial.println(F("Updating Displays"));
		u8g.firstPage();
		do {
			//      /******** Display Button Text  *********/
			u8g.setFont(u8g_font_helvB14);
			u8g.setFontPosBaseline();
			u8g.drawStr(64 - u8g.getStrWidth(displays[i][0]) / 2, 26, displays[i][0]);

			//    u8g.setFont(u8g_font_helvB12);
			u8g.setFontPosBaseline();
			u8g.drawStr(64 - u8g.getStrWidth(displays[i][1]) / 2, 50, displays[i][1]);
			// u8g.drawHLine(0, 35, 128);
			// u8g.drawHLine(0, 36, 128);
			// u8g.drawHLine(0, 37, 128);
			/***************************************/
		} while ( u8g.nextPage() );
		delay(50);
	}
}
void updateDisplay(int disp) {
	tcaSelect(disp);

	u8g.firstPage();
do {
			//      /******** Display Button Text  *********/
			u8g.setFont(u8g_font_helvB12);
			u8g.setFontPosBaseline();
			u8g.drawStr(64 - u8g.getStrWidth(displays[disp][0]) / 2, 26, displays[disp][0]);

			//    u8g.setFont(u8g_font_helvB12);
			u8g.setFontPosBaseline();
			u8g.drawStr(64 - u8g.getStrWidth(displays[disp][1]) / 2, 60, displays[disp][1]);
			// u8g.drawHLine(0, 35, 128);
			// u8g.drawHLine(0, 36, 128);
			// u8g.drawHLine(0, 37, 128);
			/***************************************/
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
	//if (debug Serial.println("hold action fired :" + String(s));
	holdActions[s]();
}

//========= Execute action for single button click =========
void clickAction()
{
	int s = whichSwitch(0);
	s--;
	for (int i = 0; i < ACTIONS_PER_BUTTON; i++){
		//Serial.println("Executing action " + String(i) + " for switch " + String(s));
		//Serial.println(F("executing click action"));
		switch (buttonActions[s][i].command) {
		case PC:
			Serial.print(F("PC command detected. S: "));
			Serial.println(String(s));
			midiPC(buttonActions[s][i].param1, buttonActions[s][i].param2);
			break;
		case CC:
			midiCC(buttonActions[s][i].param1, buttonActions[s][i].param2, buttonActions[s][i].param3);
			break;
		default:
			break;
		}

	}
	//if (debugSerial.println("clickAction Detected " + String(page) + "-" + String(s));
}

void midiPC(int songNumber, int midiChannel) {
	Serial.println("Executing MIDI PC command. PC: " + String(songNumber) + "  Channel: " + String(midiChannel));
	if(!debug) MIDI.sendProgramChange(songNumber, midiChannel);
}
void midiCC(int midiControler, int midiValue, int midiChannel) {
	//if (debug Serial.println("Executing MIDI CC command");
	if(!debug)MIDI.sendControlChange(midiControler, midiValue, midiChannel);
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

	//setup serial port for monitoring
	if (debug){
		Serial.begin(9600); //for serial monitoring
	}else{
		Serial.begin(31250); //for serial MIDI commands
	}


	while (! Serial); // Wait untilSerial is ready - Leonardo

	//setup i2c eeprom connection
	byte eepStatus = eep.begin(eep.twiClock400kHz);   //go fast!
	if (eepStatus) {
		//if (debug Serial.print(F("extEEPROM.begin() failed, status = "));
		//if (debug Serial.println(eepStatus);
		while (1);
	}


	displayInit(); // Initialize the displays

	getConfig();
	//p1_display();

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
		////if (debug Serial.println("waiting state");
	}

	//DETECT STATE WHERE 1 BUTTON HAS BEEN PRESSED
	if (state == WAITING_STATE and active == 1)
	{
		state =   PRE_BUTTON_STATE;
		////if (debug Serial.println("pre button state");
	}

	//DETECT STATE WHERE TWO BUTTONS HAVE BEEN PRESSED
	if (state == PRE_BUTTON_STATE and active == 2)
	{
		state = DOUBLE_BUTTON_STATE;
		////if (debug Serial.println("double button state");
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
				////if (debug Serial.println("held button state");
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
				////if (debug Serial.println("clicked button state");
				clickAction();
				break;
			}
		}
	}

	// =============== Encoder ============
	// long newPosition = myEnc.read();
	// if (newPosition != oldPosition * 4)
	// {
		// oldPosition = newPosition / 4;
		// if (debug Serial.println("Inside encoder loop now");
		// sprintf(displays[0][0], "%d", oldPosition);
		// displays[0][0] =oldPosition;
		// updateDisplay(0);
		// if (debug Serial.println(newPosition);
	// }
}
