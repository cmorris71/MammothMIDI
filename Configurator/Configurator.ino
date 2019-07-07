#define TL 1 //Timeline
#define BS 2 //Bigsky
#define HX 3 //HX Stomp
#define PC 0
#define PT 1
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
void readConfig(){

//=============================================== PAGE 1 ================================================
int p = 1;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "Song      ");   strcpy(d[p][5][1], "Song     ");    strcpy(d[p][6][1], "Song     ");
strcpy(d[p][4][2], "1         ");   strcpy(d[p][5][2], "2        ");    strcpy(d[p][6][2], "3        ");
b[p][5][1] = {PC,0,BS,0,0,0,0,0};   b[p][5][1] = {PC,1,HX,0,0,0,0,0};   b[p][6][1] = {PC,2,HX,0,0,0,0,0};
b[p][4][2] = {PC,0,HX,0,0,0,0,0};   b[p][5][2] = {PC,2,TL,0,0,0,0,0};   b[p][6][2] = {PC,4,TL,0,0,0,0,0};
b[p][4][3] = {PC,0,TL,0,0,0,0,0}    b[p][5][3] = {PC,3,BS,0,0,0,0,0};   b[p][6][3] = {PC,6,BS,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "Hold for ");    strcpy(d[p][5][1], "This Pedal");   strcpy(d[p][6][1], "Hold to Return");
strcpy(d[p][4][2], "Tuner    ");    strcpy(d[p][5][2], "Is Awesome");   strcpy(d[p][6][2], "To Preset 1   ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};


//================================================ PAGE 2 ===============================================
p = 2;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "Song     ");    strcpy(d[p][5][1], "Song     ");    strcpy(d[p][6][1], "Song     ");
strcpy(d[p][4][2], "4        ");    strcpy(d[p][5][2], "5        ");    strcpy(d[p][6][2], "6        ");
b[p][4][1] = {PC,3,HX,0,0,0,0,0};   b[p][5][1] = {PC,4,HX,0,0,0,0,0};   b[p][6][1] = {PC, 5,HX,0,0,0,0,0};
b[p][4][2] = {PC,6,TL,0,0,0,0,0};   b[p][5][2] = {PC,8,TL,0,0,0,0,0};   b[p][6][2] = {PC,10,TL,0,0,0,0,0};
b[p][4][3] = {PC,9,BS,0,0,0,0,0};   b[p][5][3] = {PC,12,BS,0,0,0,0,0};  b[p][6][3] = {PC,15,BS,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};

//=============================================== PAGE 3 ================================================
p = 3;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};

//=============================================== PAGE 4 ================================================
p = 4;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};


//=============================================== PAGE 5 ================================================
p = 5;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};


//=============================================== PAGE 6 ================================================
p = 6;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};


//=============================================== PAGE 7 ================================================
p = 7;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};


//=============================================== PAGE 8 ================================================
p = 8;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};


//=============================================== PAGE 9 ================================================
p = 9;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};


//=============================================== PAGE 10 ===============================================
p = 10;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};


//=============================================== PAGE 11 ===============================================
p = 11;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};


//=============================================== PAGE 12 ===============================================
p = 12;

/*-------Top Left -------------     ---------Top Middle------------     ---------Top Right------------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][4][1] = {XX,0,0,0,0,0,0,0};    b[p][5][1] = {XX,0,0,0,0,0,0,0};    b[p][6][1] = {XX,0,0,0,0,0,0,0};
b[p][4][2] = {XX,0,0,0,0,0,0,0};    b[p][5][2] = {XX,0,0,0,0,0,0,0};    b[p][6][2] = {XX,0,0,0,0,0,0,0};
b[p][4][3] = {XX,0,0,0,0,0,0,0};    b[p][5][3] = {XX,0,0,0,0,0,0,0};    b[p][6][3] = {XX,0,0,0,0,0,0,0};
b[p][4][4] = {XX,0,0,0,0,0,0,0};    b[p][5][4] = {XX,0,0,0,0,0,0,0};    b[p][6][4] = {XX,0,0,0,0,0,0,0};
b[p][4][5] = {XX,0,0,0,0,0,0,0};    b[p][5][5] = {XX,0,0,0,0,0,0,0};    b[p][6][5] = {XX,0,0,0,0,0,0,0};
b[p][4][6] = {XX,0,0,0,0,0,0,0};    b[p][5][6] = {XX,0,0,0,0,0,0,0};    b[p][6][6] = {XX,0,0,0,0,0,0,0};

/*-------Bottom Left-----------     ---------Bottom Middle----------     ---------Bottom Right---------*/
strcpy(d[p][4][1], "         ");    strcpy(d[p][5][1], "         ");    strcpy(d[p][6][1], "         ");
strcpy(d[p][4][2], "         ");    strcpy(d[p][5][2], "         ");    strcpy(d[p][6][2], "         ");
b[p][1][1] = {XX,0,0,0,0,0,0,0};    b[p][2][1] = {XX,0,0,0,0,0,0,0};    b[p][3][1] = {XX,0,0,0,0,0,0,0};
b[p][1][2] = {XX,0,0,0,0,0,0,0};    b[p][2][2] = {XX,0,0,0,0,0,0,0};    b[p][3][2] = {XX,0,0,0,0,0,0,0};
b[p][1][3] = {XX,0,0,0,0,0,0,0};    b[p][2][3] = {XX,0,0,0,0,0,0,0};    b[p][3][3] = {XX,0,0,0,0,0,0,0};
b[p][1][4] = {XX,0,0,0,0,0,0,0};    b[p][2][4] = {XX,0,0,0,0,0,0,0};    b[p][3][4] = {XX,0,0,0,0,0,0,0};
b[p][1][5] = {XX,0,0,0,0,0,0,0};    b[p][2][5] = {XX,0,0,0,0,0,0,0};    b[p][3][5] = {XX,0,0,0,0,0,0,0};
b[p][1][6] = {XX,0,0,0,0,0,0,0};    b[p][2][6] = {XX,0,0,0,0,0,0,0};    b[p][3][6] = {XX,0,0,0,0,0,0,0};

}

void setDisplay(int page, int display, char displayText[displayBytesPerScreen]){
	page--;		//zero-based index to 1-based convention
	display--;	
	int addr = displayAddrStart
	+ page    * displayBytesPerPage
	+ display * displayBytesPerScreen;
	
	eep.write(addr, displayText, displayBytesPerScreen); //write test to EEPROM
}


void setButton(int page, int button, int actionNum, byte action[buttBytesPerAction]){
	page--;		//zero-based index to 1-based convention
	button--;
	actionNum--; 
	int addr = buttAddrStart
	+ (page   * buttBytesPerPage)
	+ (button * buttBytesPerButton)
	+ (actionNum * buttBytesPerAction);
	
	int status = eep.write(addr, action, buttBytesPerAction); //write test to EEPROM
	// Serial.println("Writing to address: " + String(addr));
	// Serial.println("Writing status: " + String(status));
}

void setString(int page, int button, int stringNum, char str[strBytesPerString]){
	page--;		//zero-based index to 1-based convention
	button--;
	stringNum--; 
	int addr = strAddrStart
	+ (page   * strBytesPerPage)
	+ (button * strBytesPerButton)
	+ (stringNum * strBytesPerString);
	int status = eep.write(addr, str, strBytesPerString); //write test to EEPROM
	
	// Serial.println("Writing to address: " + String(addr));
	// Serial.println(str);
	// Serial.println("Writing status: " + String(status));
}

void printbuttons(){
	word addr = buttAddrStart;
	for (int p =0; p < 2; p++){
		Serial.println("Page: " + String(p+1) + "    Address: 0x" + String(addr));
		for (int b=0;b<numButtons; b++){
			Serial.println("  Button: " + String(b+1)+ "    Address: 0x" + String(addr));
			for (int i=0; i < numActions; i++){
				eep.read(addr, read, action_size);
				Serial.println(	"    " +String(read[0]) +"."+
				String(read[1]) + "."+
				String(read[2]) + "."+
				String(read[3]) + "."+
				String(read[4]) + "."+
				String(read[5]) + "."+
				String(read[6]) + "."+
				String(read[7]));
				addr += buttBytesPerAction;
			}
		}
	}
}

void printstrings(){
	word addr = strAddrStart;
	char str[32];
	for (int p =0; p < numPages; p++){
		Serial.println("Page: " + String(p+1));// + "    Address: 0x" + String(addr));
		for (int b=0;b<numButtons; b++){
			Serial.println("  Button: " + String(b+1));//+ "    Address: 0x" + String(addr));
			for (int i=0; i < strStringsPerButton; i++){
				
				eep.read(addr, str, strBytesPerString);
				Serial.println(str);
				addr += strBytesPerString;
			}
		}
	}
}

void bulkFillStrings(){
	char str[32];
	for (int p =1; p <= numPages; p++){
		//Serial.println("Page: " + String(p+1) + "    Address: 0x" + String(addr));
		for (int b=1;b<=numButtons; b++){
			//Serial.println("  Button: " + String(b+1)+ "    Address: 0x" + String(addr));
			for (int i=1; i <= strStringsPerButton; i++){
				sprintf(str, "Button %i-%i-%i",p,b,i );
				//Serial.println(str);
				//str = "Button " + String(p) + "-" + String(b) + "-" + String(i);
				setString(p,b,i,str);
			}
		}
	}
}

void flushActions(){
	for (word faddr = 0; faddr < (strAddrStart -16); faddr += 16){
		Serial.print("Wirting byte ");
		Serial.print(String(faddr));
		eep.write(faddr, 0000000000000000, 16);
	}

}

const byte btnStart = 6;              //start button

uint32_t addr = 0;

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

	readConfig();
	//Loop through pages
	for (p=1; p<=numPaged; p++){
		printDisplays(1,"Programming page", String(p));
		//Loop through display
		for (d=1, d<=numDisplays; d++){
			setString(p,d,1, d[p][d][1]); // set display line 1
			setString(p,d,2, d[p][d][2]); // set display line 2
		}
		
		//Loop through button actions
		for (a=1; a<=numActions; a++){
			setButton(p, b, a, b[p][b][a]);
		}

	}
	
	printDisplays(1,"Programming", "Complete");

}

void loop(){
}
