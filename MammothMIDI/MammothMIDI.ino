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
#include <SoftwareSerial.h>

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

SoftwareSerial softSerial(2, 11); // RX, TX


// Create and bind the MIDI interface to the default hardware Serial port
//MIDI_CREATE_DEFAULT_INSTANCE();
MIDI_CREATE_INSTANCE(SoftwareSerial, softSerial, MIDI);

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

void serialToEEPROM(){
    Serial.print("Printing: ");
	Serial.println(dataString);
	delay(25);
	eep.write(addr, dataString,32);
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
	actionWrite(8, 0x03000C0000000000);
	actionWrite(16, 0x03000D0000000000);
	actionWrite(24, 0x03000E0000000000);
	actionWrite(32, 0x03000F0000000000);
	actionWrite(40, 0x0300100000000000);

	eep.write(4672,"Button 2\0",32);
	eep.write(4704,"Baby\0",32);
	actionWrite(48, 0x01000C0000000000);
	actionWrite(56, 0x00FF0C000C010000);
	actionWrite(64, 0x00FF000000000000);
	actionWrite(72, 0x00FF000000000000);
	actionWrite(80, 0x00FF000000000000);
	actionWrite(88, 0x00FF000000000000);




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
	if(!debug) MIDI.sendControlChange(midiControler, midiValue, midiChannel);
}

void recvWithStartEndMarkers() {
	static boolean recvInProgress = false;
	static byte ndx = 0;
	char startMarker = '<';
	char endMarker = '>';
	char rc;

	while (Serial.available() > 0 && newData == false) {
		rc = Serial.read();

		if (recvInProgress == true) {
			if (rc != endMarker) {
				receivedChars[ndx] = rc;
				ndx++;
				if (ndx >= numChars) {
					ndx = numChars - 1;
				}
			}
			else {
					receivedChars[ndx] = '\0'; // terminate the string
				recvInProgress = false;
				ndx = 0;
				newData = true;
			}
		}

		else if (rc == startMarker) {
			recvInProgress = true;
		}
	}
}

//============

void parseData() {      // split the data into its parts

	char * strtokIndx; // this is used by strtok() as an index

	strtokIndx = strtok(tempChars,",");      // get the first part - the string
	addr = atoi(strtokIndx); // copy it to addr
	
	byte index = 0;
	
	if (addr >= stringAddrStart){
			strtokIndx = strtok(NULL, ",");
			strcpy(dataString, strtokIndx);
			index++;
		
	}else{
		strtokIndx = strtok(NULL, ",");
		while(strtokIndx != NULL){
			data[index]  = (byte)atoi(strtokIndx);
			index++;			
			strtokIndx = strtok(NULL, ",");
		}
	}
	
	
	dataLength = index;

	// strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
	// integerFromPC = atoi(strtokIndx);     // convert this part to an integer

	// strtokIndx = strtok(NULL, ",");
	// floatFromPC = atof(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
	Serial.println(" ");
	Serial.print("Message ");
	Serial.println(String(addr));
	Serial.print("Data ");
	if (addr >= stringAddrStart){
		Serial.println(dataString);
	}else{
		for (int i=0; i<dataLength; i++){
			Serial.print(String(data[i]));
		}
	}

	Serial.println(" ");
		
}

void setup() {
	// ========= Program Button Stup =============
	//pinMode(btnProgram, INPUT_PULLUP);



	// ========= Foot Switch Button Setup =========
	for (int i = 0; i <= 6; i++) {
		pinMode(4 + i, INPUT_PULLUP);
		footSwitch[i].setActiveLogic(LOW);
		footSwitch[i].disableDoubleClick();
		footSwitch[i].setHoldTime(500);
	}

	MIDI.begin(MIDI_CHANNEL_OFF);

	//setup serial port for monitoring

	Serial.begin(9600); //for serial monitoring

	//softSerial.begin(31250); //for serial MIDI commands
	


	//while (! Serial); // Wait untilSerial is ready - Leonardo


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
	for (int i = 0; i <= 6; i++)
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
		for (int i = 0; i <= 6; i++)
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
		for (int i = 0; i <= 6; i++)
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
	
	//if (digitalRead(btnProgram) == LOW) writeConfig();
	
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
	
	if (progMode){
		recvWithStartEndMarkers();
		if (newData == true) {
			strcpy(tempChars, receivedChars);
			// this temporary copy is necessary to protect the original data
			//   because strtok() used in parseData() replaces the commas with \0
			parseData();
			//showParsedData();
			serialToEEPROM();
			newData = false;
		}
	}
}
