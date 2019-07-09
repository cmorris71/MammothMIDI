#define TL 1 //Timeline
#define BS 2 //Bigsky
#define HX 3 //HX Stomp
#define PC 0
#define CT 1
#define CC 2
#define XX 255
#define MUX_Address 0x70 // TCA9548A Encoders address


#include <extEEPROM.h>    //https://github.com/PaoloP74/extEEPROM
#include <U8glib.h>
#include <Wire.h>

// u8g2 display constructor
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

const int numPages    = 12;
const int numButtons  = 6;
const int numDisplays = 8;
const int numActions  = 6;

const int buttAddrStart        = 0;
const int buttBytesPerAction   = 8;


const int buttBytesPerButton   = numActions * buttBytesPerAction; //
const int buttBytesPerPage     = buttBytesPerButton * numButtons;

const int displayAddrStart      = buttBytesPerPage * numPages;
const int displayBytesPerScreen = 32;
const int displayBytesPerPage   = displayBytesPerScreen * numDisplays;


const word strAddrStart         = 4608;
const int strStringsPerButton   = 2;
const int strBytesPerString     = 32;
const int strBytesPerButton     = strStringsPerButton * strBytesPerString;
const int strBytesPerPage       = strBytesPerButton * numButtons;


int action_size = 8;

byte read[8];

char d[numPages+1][numButtons+1][3][32];
byte b[numPages+1][numButtons+1][numActions+1];

//One 24LC256 EEPROMs on the bus
const uint32_t totalKBytes = 32;         //for read and write test functions
extEEPROM eep(kbits_256, 1, 64);         //device size, number of devices, page size

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


/* Button Programming

Program Change    						
{PC, value, channel,...}                                       BS = Bigsky
																TL = Timeline
Control change                                                 HX = HX Stomp
{CC, CtrlNo, CtrlVal, Channel,...}                             XX = No command
																Fill unused parameters with '0'
Control change toggle  
{CT, CtrlNo1, CtrlVal1, CtrlNo2, CtrlVal2 ,...}
*/


// void setDisplay(int page, int display, char displayText[displayBytesPerScreen]){
	// page--;		//zero-based index to 1-based convention
	// display--;	
	// int addr = displayAddrStart
	// + page    * displayBytesPerPage
	// + display * displayBytesPerScreen;
	
	// eep.write(addr, displayText, displayBytesPerScreen); //write test to EEPROM
// }


// void setButton(int page, int button, int actionNum, byte action[buttBytesPerAction]){
	// page--;		//zero-based index to 1-based convention
	// button--;
	// actionNum--; 
	// int addr = buttAddrStart
	// + (page   * buttBytesPerPage)
	// + (button * buttBytesPerButton)
	// + (actionNum * buttBytesPerAction);
	
	// int status = eep.write(addr, action, buttBytesPerAction); //write test to EEPROM
	// Serial.println("Writing to address: " + String(addr));
	// Serial.println("Writing status: " + String(status));
// }

// void setString(int page, int button, int stringNum, char str[strBytesPerString]){
	// page--;		//zero-based index to 1-based convention
	// button--;
	// stringNum--; 
	// int addr = strAddrStart
	// + (page   * strBytesPerPage)
	// + (button * strBytesPerButton)
	// + (stringNum * strBytesPerString);
	// int status = eep.write(addr, str, strBytesPerString); //write test to EEPROM
	
	// Serial.println("Writing to address: " + String(addr));
	// Serial.println(str);
	// Serial.println("Writing status: " + String(status));
// }

// void printbuttons(){
	// word addr = buttAddrStart;
	// for (int p =0; p < 2; p++){
		// Serial.println("Page: " + String(p+1) + "    Address: 0x" + String(addr));
		// for (int b=0;b<numButtons; b++){
			// Serial.println("  Button: " + String(b+1)+ "    Address: 0x" + String(addr));
			// for (int i=0; i < numActions; i++){
				// eep.read(addr, read, action_size);
				// Serial.println(	"    " +String(read[0]) +"."+
				// String(read[1]) + "."+
				// String(read[2]) + "."+
				// String(read[3]) + "."+
				// String(read[4]) + "."+
				// String(read[5]) + "."+
				// String(read[6]) + "."+
				// String(read[7]));
				// addr += buttBytesPerAction;
			// }
		// }
	// }
// }

// void printstrings(){
	// word addr = strAddrStart;
	// char str[32];
	// for (int p =0; p < numPages; p++){
		// Serial.println("Page: " + String(p+1));// + "    Address: 0x" + String(addr));
		// for (int b=0;b<numButtons; b++){
			// Serial.println("  Button: " + String(b+1));//+ "    Address: 0x" + String(addr));
			// for (int i=0; i < strStringsPerButton; i++){
				
				// eep.read(addr, str, strBytesPerString);
				// Serial.println(str);
				// addr += strBytesPerString;
			// }
		// }
	// }
// }

// void bulkFillStrings(){
	// char str[32];
	// for (int p =1; p <= numPages; p++){
		// Serial.println("Page: " + String(p+1) + "    Address: 0x" + String(addr));
		// for (int b=1;b<=numButtons; b++){
			// Serial.println("  Button: " + String(b+1)+ "    Address: 0x" + String(addr));
			// for (int i=1; i <= strStringsPerButton; i++){
				// sprintf(str, "Button %i-%i-%i",p,b,i );
				// Serial.println(str);
				// str = "Button " + String(p) + "-" + String(b) + "-" + String(i);
				// setString(p,b,i,str);
			// }
		// }
	// }
// }

void flushActions(){
	for (word faddr = 0; faddr < (strAddrStart -16); faddr += 16){
		Serial.print("Wirting byte ");
		Serial.print(String(faddr));
		eep.write(faddr, 0000000000000000, 16);
	}

}

void configure(){
	/* 
	================================
	Paste from spreadsheet here	
	================================
	*/
eep.write(4608,"Button 1\0",32);
eep.write(4640,"Action\0",32);
actionWrite(0, 0x0300450000000000);
actionWrite(8, 0x00FF000000000000);
actionWrite(16, 0x00FF000000000000);
actionWrite(24, 0x00FF000000000000);
actionWrite(32, 0x00FF000000000000);
actionWrite(40, 0x00FF000000000000);

eep.write(4672,"Button 2\0",32);
eep.write(4704,"Baby\0",32);
actionWrite(48, 0x0102000000000000);
actionWrite(56, 0x00FF000000000000);
actionWrite(64, 0x00FF000000000000);
actionWrite(72, 0x00FF000000000000);
actionWrite(80, 0x00FF000000000000);
actionWrite(88, 0x00FF000000000000);


eep.write(4736,"Button 3\0",32);
eep.write(4768,"Yeah!\0",32);
actionWrite(96, 0x03002C0000000000);
actionWrite(104, 0x00FF000000000000);
actionWrite(112, 0x00FF000000000000);
actionWrite(120, 0x00FF000000000000);
actionWrite(128, 0x00FF000000000000);
actionWrite(136, 0x00FF000000000000);

eep.write(4800,"Button 4\0",32);
eep.write(4832,"More?\0",32);
actionWrite(144, 0x0100040000000000);
actionWrite(152, 0x00FF000000000000);
actionWrite(160, 0x00FF000000000000);
actionWrite(168, 0x00FF000000000000);
actionWrite(176, 0x00FF000000000000);
actionWrite(184, 0x00FF000000000000);

eep.write(4864,"Button 5\0",32);
eep.write(4896,"Am Ded\0",32);
actionWrite(192, 0x0200030000000000);
actionWrite(200, 0x00FF000000000000);
actionWrite(208, 0x00FF000000000000);
actionWrite(216, 0x00FF000000000000);
actionWrite(224, 0x00FF000000000000);
actionWrite(232, 0x00FF000000000000);

eep.write(4928,"Button 6\0",32);
eep.write(4960,"Oh Heck\0",32);
actionWrite(240, 0x0100060000000000);
actionWrite(248, 0x00FF000000000000);
actionWrite(256, 0x00FF000000000000);
actionWrite(264, 0x00FF000000000000);
actionWrite(272, 0x00FF000000000000);
actionWrite(280, 0x00FF000000000000);


eep.write(4896,"Button 1\0",32);
eep.write(4928,"Action\0",32);
actionWrite(288, 0x0300450000000000);
actionWrite(296, 0x00FF000000000000);
actionWrite(304, 0x00FF000000000000);
actionWrite(312, 0x00FF000000000000);
actionWrite(320, 0x00FF000000000000);
actionWrite(328, 0x00FF000000000000);

eep.write(4960,"Button 2\0",32);
eep.write(4992,"Baby\0",32);
actionWrite(336, 0x0102000000000000);
actionWrite(344, 0x00FF000000000000);
actionWrite(352, 0x00FF000000000000);
actionWrite(360, 0x00FF000000000000);
actionWrite(368, 0x00FF000000000000);
actionWrite(376, 0x00FF000000000000);


eep.write(5024,"Button 3\0",32);
eep.write(5056,"Yeah!\0",32);
actionWrite(384, 0x03002C0000000000);
actionWrite(392, 0x00FF000000000000);
actionWrite(400, 0x00FF000000000000);
actionWrite(408, 0x00FF000000000000);
actionWrite(416, 0x00FF000000000000);
actionWrite(424, 0x00FF000000000000);

eep.write(5088,"Button 4\0",32);
eep.write(5120,"More?\0",32);
actionWrite(432, 0x0100040000000000);
actionWrite(440, 0x00FF000000000000);
actionWrite(448, 0x00FF000000000000);
actionWrite(456, 0x00FF000000000000);
actionWrite(464, 0x00FF000000000000);
actionWrite(472, 0x00FF000000000000);

eep.write(5152,"Button 5\0",32);
eep.write(5184,"Am Ded\0",32);
actionWrite(480, 0x0200030000000000);
actionWrite(488, 0x00FF000000000000);
actionWrite(496, 0x00FF000000000000);
actionWrite(504, 0x00FF000000000000);
actionWrite(512, 0x00FF000000000000);
actionWrite(520, 0x00FF000000000000);

eep.write(5216,"Button 6\0",32);
eep.write(5248,"Oh Heck\0",32);
actionWrite(528, 0x0100060000000000);
actionWrite(536, 0x00FF000000000000);
actionWrite(544, 0x00FF000000000000);
actionWrite(552, 0x00FF000000000000);
actionWrite(560, 0x00FF000000000000);
actionWrite(568, 0x00FF000000000000);

eep.write(5184,"Button 1\0",32);
eep.write(5216,"Action\0",32);
actionWrite(576, 0x0300450000000000);
actionWrite(584, 0x00FF000000000000);
actionWrite(592, 0x00FF000000000000);
actionWrite(600, 0x00FF000000000000);
actionWrite(608, 0x00FF000000000000);
actionWrite(616, 0x00FF000000000000);

eep.write(5248,"Button 2\0",32);
eep.write(5280,"Baby\0",32);
actionWrite(624, 0x0102000000000000);
actionWrite(632, 0x00FF000000000000);
actionWrite(640, 0x00FF000000000000);
actionWrite(648, 0x00FF000000000000);
actionWrite(656, 0x00FF000000000000);
actionWrite(664, 0x00FF000000000000);


eep.write(5312,"Button 3\0",32);
eep.write(5344,"Yeah!\0",32);
actionWrite(672, 0x03002C0000000000);
actionWrite(680, 0x00FF000000000000);
actionWrite(688, 0x00FF000000000000);
actionWrite(696, 0x00FF000000000000);
actionWrite(704, 0x00FF000000000000);
actionWrite(712, 0x00FF000000000000);

eep.write(5376,"Button 4\0",32);
eep.write(5408,"More?\0",32);
actionWrite(720, 0x0100040000000000);
actionWrite(728, 0x00FF000000000000);
actionWrite(736, 0x00FF000000000000);
actionWrite(744, 0x00FF000000000000);
actionWrite(752, 0x00FF000000000000);
actionWrite(760, 0x00FF000000000000);

eep.write(5440,"Button 5\0",32);
eep.write(5472,"Am Ded\0",32);
actionWrite(768, 0x0200030000000000);
actionWrite(776, 0x00FF000000000000);
actionWrite(784, 0x00FF000000000000);
actionWrite(792, 0x00FF000000000000);
actionWrite(800, 0x00FF000000000000);
actionWrite(808, 0x00FF000000000000);

eep.write(5504,"Button 6\0",32);
eep.write(5536,"Oh Heck\0",32);
actionWrite(816, 0x0100060000000000);
actionWrite(824, 0x00FF000000000000);
actionWrite(832, 0x00FF000000000000);
actionWrite(840, 0x00FF000000000000);
actionWrite(848, 0x00FF000000000000);
actionWrite(856, 0x00FF000000000000);


	
}
const byte btnStart = 6;              //start button

uint32_t addr = 0;

void actionWrite(word memAddr, uint64_t action){
	uint64_t buffer = __builtin_bswap64(action);
	eep.write(memAddr, (byte *)&buffer, 8);
}

void setup(void)
{
	
	displayInit(); // Initialize the displays
	
	
	pinMode(btnStart, INPUT_PULLUP);
	Serial.begin(9600);
	byte eepStatus = eep.begin(eep.twiClock400kHz);   //go fast!
	if (eepStatus) {
		Serial.print(F("extEEPROM.begin() failed, status = "));
		Serial.println(eepStatus);
		while (1);
	}

	Serial.println(F("Press button to start..."));
	printDisplays(1,"Press button", "to start");
	while (digitalRead(btnStart) == HIGH) delay(10);    //wait for button push

	configure();




	
	printDisplays(1,"Programming", "Complete");

}

void loop(){
	

}
