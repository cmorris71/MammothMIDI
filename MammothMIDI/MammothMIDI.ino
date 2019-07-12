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

const byte btnProgram = 4; //button to write configuration to EEPROM  

//Setup external 24LC256 EEPROMs on the bus
extEEPROM eep(kbits_256, 1, 64);         //device size, number of devices, page size



// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// Setup encoder. Best to use interrupt pins
// Encoder myEnc(2, 3);

long oldPosition  = -999;

// u8g2 display constructor
//U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

void actionWrite(word memAddr, uint64_t action){
	uint64_t buffer = __builtin_bswap64(action);
	eep.write(memAddr, (byte *)&buffer, 8);
}


void writeConfig(){
	/* 
	================================
	Paste from spreadsheet here	
	================================
	*/
	
	printDisplays(1,"Writing", "Config");
	
eep.write(4608,"Test\0",32);
eep.write(4640,"1\0",32);
actionWrite(0, 0x03000B0000000000);
actionWrite(8, 0x00FF000000000000);
actionWrite(16, 0x00FF000000000000);
actionWrite(24, 0x00FF000000000000);
actionWrite(32, 0x00FF000000000000);
actionWrite(40, 0x00FF000000000000);

eep.write(4672,"Button 2\0",32);
eep.write(4704,"Baby\0",32);
actionWrite(48, 0x01000C0000000000);
actionWrite(56, 0x00FF0C000C010000);
actionWrite(64, 0x00FF000000000000);
actionWrite(72, 0x00FF000000000000);
actionWrite(80, 0x00FF000000000000);
actionWrite(88, 0x00FF000000000000);


eep.write(4736,"Button 3\0",32);
eep.write(4768,"Yeah!\0",32);
actionWrite(96, 0x03000D0000000000);
actionWrite(104, 0x00FF000000000000);
actionWrite(112, 0x00FF000000000000);
actionWrite(120, 0x00FF000000000000);
actionWrite(128, 0x00FF000000000000);
actionWrite(136, 0x00FF000000000000);

eep.write(4800,"Button 4\0",32);
eep.write(4832,"More?\0",32);
actionWrite(144, 0x01000E0000000000);
actionWrite(152, 0x00FF000000000000);
actionWrite(160, 0x00FF000000000000);
actionWrite(168, 0x00FF000000000000);
actionWrite(176, 0x00FF000000000000);
actionWrite(184, 0x00FF000000000000);

eep.write(4864,"Button 5\0",32);
eep.write(4896,"Am Ded\0",32);
actionWrite(192, 0x02000F0000000000);
actionWrite(200, 0x00FF000000000000);
actionWrite(208, 0x00FF000000000000);
actionWrite(216, 0x00FF000000000000);
actionWrite(224, 0x00FF000000000000);
actionWrite(232, 0x00FF000000000000);

eep.write(4928,"Button 6\0",32);
eep.write(4960,"Oh Heck\0",32);
actionWrite(240, 0x0100100000000000);
actionWrite(248, 0x00FF000000000000);
actionWrite(256, 0x00FF000000000000);
actionWrite(264, 0x00FF000000000000);
actionWrite(272, 0x00FF000000000000);
actionWrite(280, 0x00FF000000000000);


eep.write(4992,"Page\0",32);
eep.write(5024,"2\0",32);
actionWrite(288, 0x0300150000000000);
actionWrite(296, 0x00FF000000000000);
actionWrite(304, 0x00FF000000000000);
actionWrite(312, 0x00FF000000000000);
actionWrite(320, 0x00FF000000000000);
actionWrite(328, 0x00FF000000000000);

eep.write(5056,"Button 2\0",32);
eep.write(5088,"Baby\0",32);
actionWrite(336, 0x0100160000000000);
actionWrite(344, 0x00FF000000000000);
actionWrite(352, 0x00FF000000000000);
actionWrite(360, 0x00FF000000000000);
actionWrite(368, 0x00FF000000000000);
actionWrite(376, 0x00FF000000000000);


eep.write(5120,"Button 3\0",32);
eep.write(5152,"Yeah!\0",32);
actionWrite(384, 0x0300170000000000);
actionWrite(392, 0x00FF000000000000);
actionWrite(400, 0x00FF000000000000);
actionWrite(408, 0x00FF000000000000);
actionWrite(416, 0x00FF000000000000);
actionWrite(424, 0x00FF000000000000);

eep.write(5184,"Button 4\0",32);
eep.write(5216,"More?\0",32);
actionWrite(432, 0x0100180000000000);
actionWrite(440, 0x00FF000000000000);
actionWrite(448, 0x00FF000000000000);
actionWrite(456, 0x00FF000000000000);
actionWrite(464, 0x00FF000000000000);
actionWrite(472, 0x00FF000000000000);

eep.write(5248,"Button 5\0",32);
eep.write(5280,"Am Ded\0",32);
actionWrite(480, 0x0200190000000000);
actionWrite(488, 0x00FF000000000000);
actionWrite(496, 0x00FF000000000000);
actionWrite(504, 0x00FF000000000000);
actionWrite(512, 0x00FF000000000000);
actionWrite(520, 0x00FF000000000000);

eep.write(5312,"Button 6\0",32);
eep.write(5344,"Oh Heck\0",32);
actionWrite(528, 0x01001A0000000000);
actionWrite(536, 0x00FF000000000000);
actionWrite(544, 0x00FF000000000000);
actionWrite(552, 0x00FF000000000000);
actionWrite(560, 0x00FF000000000000);
actionWrite(568, 0x00FF000000000000);

eep.write(5376,"Page\0",32);
eep.write(5408,"3\0",32);
actionWrite(576, 0x0300450000000000);
actionWrite(584, 0x00FF000000000000);
actionWrite(592, 0x00FF000000000000);
actionWrite(600, 0x00FF000000000000);
actionWrite(608, 0x00FF000000000000);
actionWrite(616, 0x00FF000000000000);

eep.write(5440,"Button 2\0",32);
eep.write(5472,"Baby\0",32);
actionWrite(624, 0x0102000000000000);
actionWrite(632, 0x00FF000000000000);
actionWrite(640, 0x00FF000000000000);
actionWrite(648, 0x00FF000000000000);
actionWrite(656, 0x00FF000000000000);
actionWrite(664, 0x00FF000000000000);


eep.write(5504,"Button 3\0",32);
eep.write(5536,"Yeah!\0",32);
actionWrite(672, 0x03002C0000000000);
actionWrite(680, 0x00FF000000000000);
actionWrite(688, 0x00FF000000000000);
actionWrite(696, 0x00FF000000000000);
actionWrite(704, 0x00FF000000000000);
actionWrite(712, 0x00FF000000000000);

eep.write(5568,"Button 4\0",32);
eep.write(5600,"More?\0",32);
actionWrite(720, 0x0100040000000000);
actionWrite(728, 0x00FF000000000000);
actionWrite(736, 0x00FF000000000000);
actionWrite(744, 0x00FF000000000000);
actionWrite(752, 0x00FF000000000000);
actionWrite(760, 0x00FF000000000000);

eep.write(5632,"Button 5\0",32);
eep.write(5664,"Am Ded\0",32);
actionWrite(768, 0x0200030000000000);
actionWrite(776, 0x00FF000000000000);
actionWrite(784, 0x00FF000000000000);
actionWrite(792, 0x00FF000000000000);
actionWrite(800, 0x00FF000000000000);
actionWrite(808, 0x00FF000000000000);

eep.write(5696,"Button 6\0",32);
eep.write(5728,"Oh Heck\0",32);
actionWrite(816, 0x0100060000000000);
actionWrite(824, 0x00FF000000000000);
actionWrite(832, 0x00FF000000000000);
actionWrite(840, 0x00FF000000000000);
actionWrite(848, 0x00FF000000000000);
actionWrite(856, 0x00FF000000000000);

eep.write(5760,"Page\0",32);
eep.write(5792,"4\0",32);
actionWrite(864, 0x0300450000000000);
actionWrite(872, 0x00FF000000000000);
actionWrite(880, 0x00FF000000000000);
actionWrite(888, 0x00FF000000000000);
actionWrite(896, 0x00FF000000000000);
actionWrite(904, 0x00FF000000000000);

eep.write(5824,"Button 2\0",32);
eep.write(5856,"Baby\0",32);
actionWrite(912, 0x0102000000000000);
actionWrite(920, 0x00FF000000000000);
actionWrite(928, 0x00FF000000000000);
actionWrite(936, 0x00FF000000000000);
actionWrite(944, 0x00FF000000000000);
actionWrite(952, 0x00FF000000000000);


eep.write(5888,"Button 3\0",32);
eep.write(5920,"Yeah!\0",32);
actionWrite(960, 0x03002C0000000000);
actionWrite(968, 0x00FF000000000000);
actionWrite(976, 0x00FF000000000000);
actionWrite(984, 0x00FF000000000000);
actionWrite(992, 0x00FF000000000000);
actionWrite(1000, 0x00FF000000000000);

eep.write(5952,"Button 4\0",32);
eep.write(5984,"More?\0",32);
actionWrite(1008, 0x0100040000000000);
actionWrite(1016, 0x00FF000000000000);
actionWrite(1024, 0x00FF000000000000);
actionWrite(1032, 0x00FF000000000000);
actionWrite(1040, 0x00FF000000000000);
actionWrite(1048, 0x00FF000000000000);

eep.write(6016,"Button 5\0",32);
eep.write(6048,"Am Ded\0",32);
actionWrite(1056, 0x0200030000000000);
actionWrite(1064, 0x00FF000000000000);
actionWrite(1072, 0x00FF000000000000);
actionWrite(1080, 0x00FF000000000000);
actionWrite(1088, 0x00FF000000000000);
actionWrite(1096, 0x00FF000000000000);

eep.write(6080,"Button 6\0",32);
eep.write(6112,"Oh Heck\0",32);
actionWrite(1104, 0x0100060000000000);
actionWrite(1112, 0x00FF000000000000);
actionWrite(1120, 0x00FF000000000000);
actionWrite(1128, 0x00FF000000000000);
actionWrite(1136, 0x00FF000000000000);
actionWrite(1144, 0x00FF000000000000);

eep.write(6144,"Page\0",32);
eep.write(6176,"5\0",32);
actionWrite(1152, 0x0300450000000000);
actionWrite(1160, 0x00FF000000000000);
actionWrite(1168, 0x00FF000000000000);
actionWrite(1176, 0x00FF000000000000);
actionWrite(1184, 0x00FF000000000000);
actionWrite(1192, 0x00FF000000000000);

eep.write(6208,"Button 2\0",32);
eep.write(6240,"Baby\0",32);
actionWrite(1200, 0x0102000000000000);
actionWrite(1208, 0x00FF000000000000);
actionWrite(1216, 0x00FF000000000000);
actionWrite(1224, 0x00FF000000000000);
actionWrite(1232, 0x00FF000000000000);
actionWrite(1240, 0x00FF000000000000);


eep.write(6272,"Button 3\0",32);
eep.write(6304,"Yeah!\0",32);
actionWrite(1248, 0x03002C0000000000);
actionWrite(1256, 0x00FF000000000000);
actionWrite(1264, 0x00FF000000000000);
actionWrite(1272, 0x00FF000000000000);
actionWrite(1280, 0x00FF000000000000);
actionWrite(1288, 0x00FF000000000000);

eep.write(6336,"Button 4\0",32);
eep.write(6368,"More?\0",32);
actionWrite(1296, 0x0100040000000000);
actionWrite(1304, 0x00FF000000000000);
actionWrite(1312, 0x00FF000000000000);
actionWrite(1320, 0x00FF000000000000);
actionWrite(1328, 0x00FF000000000000);
actionWrite(1336, 0x00FF000000000000);

eep.write(6400,"Button 5\0",32);
eep.write(6432,"Am Ded\0",32);
actionWrite(1344, 0x0200030000000000);
actionWrite(1352, 0x00FF000000000000);
actionWrite(1360, 0x00FF000000000000);
actionWrite(1368, 0x00FF000000000000);
actionWrite(1376, 0x00FF000000000000);
actionWrite(1384, 0x00FF000000000000);

eep.write(6464,"Button 6\0",32);
eep.write(6496,"Oh Heck\0",32);
actionWrite(1392, 0x0100060000000000);
actionWrite(1400, 0x00FF000000000000);
actionWrite(1408, 0x00FF000000000000);
actionWrite(1416, 0x00FF000000000000);
actionWrite(1424, 0x00FF000000000000);
actionWrite(1432, 0x00FF000000000000);

eep.write(6528,"Page\0",32);
eep.write(6560,"6\0",32);
actionWrite(1440, 0x0300450000000000);
actionWrite(1448, 0x00FF000000000000);
actionWrite(1456, 0x00FF000000000000);
actionWrite(1464, 0x00FF000000000000);
actionWrite(1472, 0x00FF000000000000);
actionWrite(1480, 0x00FF000000000000);

eep.write(6592,"Button\0",32);
eep.write(6624,"2\0",32);
actionWrite(1488, 0x0102000000000000);
actionWrite(1496, 0x00FF000000000000);
actionWrite(1504, 0x00FF000000000000);
actionWrite(1512, 0x00FF000000000000);
actionWrite(1520, 0x00FF000000000000);
actionWrite(1528, 0x00FF000000000000);


eep.write(6656,"Button 3\0",32);
eep.write(6688,"Yeah!\0",32);
actionWrite(1536, 0x03002C0000000000);
actionWrite(1544, 0x00FF000000000000);
actionWrite(1552, 0x00FF000000000000);
actionWrite(1560, 0x00FF000000000000);
actionWrite(1568, 0x00FF000000000000);
actionWrite(1576, 0x00FF000000000000);

eep.write(6720,"Button 4\0",32);
eep.write(6752,"More?\0",32);
actionWrite(1584, 0x0100040000000000);
actionWrite(1592, 0x00FF000000000000);
actionWrite(1600, 0x00FF000000000000);
actionWrite(1608, 0x00FF000000000000);
actionWrite(1616, 0x00FF000000000000);
actionWrite(1624, 0x00FF000000000000);

eep.write(6784,"Button 5\0",32);
eep.write(6816,"Am Ded\0",32);
actionWrite(1632, 0x0200030000000000);
actionWrite(1640, 0x00FF000000000000);
actionWrite(1648, 0x00FF000000000000);
actionWrite(1656, 0x00FF000000000000);
actionWrite(1664, 0x00FF000000000000);
actionWrite(1672, 0x00FF000000000000);

eep.write(6848,"Button 6\0",32);
eep.write(6880,"Oh Heck\0",32);
actionWrite(1680, 0x0100060000000000);
actionWrite(1688, 0x00FF000000000000);
actionWrite(1696, 0x00FF000000000000);
actionWrite(1704, 0x00FF000000000000);
actionWrite(1712, 0x00FF000000000000);
actionWrite(1720, 0x00FF000000000000);

	printDisplays(1,"Whew!", "Done");
	
	getConfig(); //load the new config for the current page
}

//========= Returns number for active switch starting with 'start' =========
int whichSwitch(int start = 0) {
	//if (debug) Serial.println("Entered Which Switch");
	if (state == CLICKED_BUTTON_STATE)
	{
		
		for (int i = start; i < 8; i++)
		{
			if (footSwitch[i].isReleased())
			{
				//if (debug) Serial.print(F("Returning button: "));
				//if (debug) Serial.print(i);
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

	//Serial.println(F("WhichSwitch returned no switch"));
	return -1;

}



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

void getConfig(){
	//read button actions from EEPROM
	char str[bytesPerString];
	word eepromAddress = (page-1) * BYTES_PER_PAGE;
	
	word eepromDispAddress;
	//if (debug Serial.println(eepromAddress);//Serial.print(String(testVar));
	byte configAction[SIZE_OF_ACTION];

	//if (debug Serial.println(F("Getting configuration."));
	//read display text from EEPROM
	eepromDispAddress = stringAddrStart + (page-1) *  stringBytesPerPage;
	
	for (byte configButton = 0; configButton < NUMBER_OF_BUTTONS; configButton++){
		for (byte actionNo=0; actionNo< ACTIONS_PER_BUTTON; actionNo++){
			// Serial.print(F("Reading from address: "));
			// Serial.println(String(eepromAddress));
			delay(10);
			eep.read(eepromAddress, configAction, SIZE_OF_ACTION);

			//Serial.println(F(""));
			delay(10);

			memcpy(&buttonActions[configButton][actionNo], configAction, SIZE_OF_ACTION); 
			//Serial.println(String(buttonActions[configButton][actionNo].param3));
			//} 
			eepromAddress += SIZE_OF_ACTION;
		}

		eep.read(eepromDispAddress, str, bytesPerString);
		memcpy(&displays[configButton][0], str, bytesPerString);
		eepromDispAddress += bytesPerString;
		eep.read(eepromDispAddress, str, bytesPerString);
		memcpy(&displays[configButton][1], str, bytesPerString);
		eepromDispAddress += bytesPerString;
		//Serial.print(("Button")+ String(configButton) + ": ");
		//delay(20);
		//Serial.println(str);
		//delay(20);
		//displays[configButton][0] = str;

	}
	
	updateDisplays();
	//if (debug) printConfig();
}


void updateDisplays() {
	for (int i=1; i<= NUMBER_OF_BUTTONS; i++) updateDisplay(i);
}


void updateDisplay(int disp) {
	char line1[bytesPerString];
	char line2[bytesPerString];
	word addr = stringAddrStart 
	+ stringBytesPerPage  * (page-1)
	+ stringBytesPerButton * (disp-1);
	
	tcaSelect(disp-1);
	//if (debug Serial.println(F("Updating Displays"));
	
	//load display text from EEPROM
	eep.read(addr, line1, bytesPerString);
	addr += bytesPerString;
	eep.read(addr, line2, bytesPerString);
	
	/******** Display Button Text  *********/
	u8g.firstPage();
	do {
		u8g.setFont(u8g_font_helvB14);
		u8g.setFontPosBaseline();
		u8g.drawStr(64 - u8g.getStrWidth(line1) / 2, 26, line1);
		u8g.setFontPosBaseline();
		u8g.drawStr(64 - u8g.getStrWidth(line2) / 2, 50, line2);

		/***************************************/
	} while ( u8g.nextPage() );
	delay(10);

}

// Print output to a display
void printDisplays(int d, char str1[16], char str2[16]) {
	tcaSelect(d);
	u8g.firstPage();
	do {
		u8g.setFont(u8g_font_helvB14);
		u8g.setFontPosBaseline();
		u8g.drawStr(64 - u8g.getStrWidth(str1) / 2, 26, str1);
		u8g.setFontPosBaseline();
		u8g.drawStr(64 - u8g.getStrWidth(str2) / 2, 50, str2);
	} while ( u8g.nextPage() );
	delay(50);
	
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
			if(debug)Serial.print(F("PC command detected. Device:"));
			if(debug)Serial.println(buttonActions[s][i].device);
			if(debug)Serial.print(F("Command: "));
			if(debug)Serial.println(buttonActions[s][i].command);
			midiPC(buttonActions[s][i].param1, buttonActions[s][i].device);
			break;
		case CC:
			if(debug)Serial.print(F("CC command detected..."));
			midiCC(buttonActions[s][i].param1, buttonActions[s][i].param2, buttonActions[s][i].device);
			break;
		default:
			if(debug) Serial.print(F("Bogusness detected..."));
			//if(debug) Serial.println(String(buttonActions[s][i].command));
			break;
		}

	}
	//if (debugSerial.println("clickAction Detected " + String(page) + "-" + String(s));
}

void midiPC(int songNumber, int midiChannel) {
	//Serial.println("Executing MIDI PC command. PC: " + String(songNumber) + "  Channel: " + String(midiChannel));
	delay(10);
	if(!debug) MIDI.sendProgramChange(songNumber, midiChannel);
}
void midiCC(int midiControler, int midiValue, int midiChannel) {
	//if (debug Serial.println("Executing MIDI CC command");
	if(!debug)MIDI.sendControlChange(midiControler, midiValue, midiChannel);
}



void setup() {
	// ========= Program Button Stup =============
	pinMode(btnProgram, INPUT_PULLUP);



	// ========= Foot Switch Button Setup =========
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
	
	if (digitalRead(btnProgram) == LOW) writeConfig();
	
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
