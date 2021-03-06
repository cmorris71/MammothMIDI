// State machine definitions
#define WAITING_STATE 0
#define PRE_BUTTON_STATE 1
#define CLICKED_BUTTON_STATE 2
#define HELD_BUTTON_STATE 3
#define DOUBLE_BUTTON_STATE 4
#define PC 0
#define PC_TOGGLE 1
#define CC 2
#define DISPLAY 3
#define NO_COMMAND 255
#define ACTIONS_PER_BUTTON 6
#define SIZE_OF_ACTION 8
#define BYTES_PER_PAGE 384
#define NUMBER_OF_PAGES 12
#define NUMBER_OF_BUTTONS 6

const word stringAddrStart      = 4608;
const int  stringsPerButton     = 2;
const int  bytesPerString       = 32; 
const int  stringBytesPerButton = stringsPerButton * bytesPerString;
const int  stringBytesPerPage   = stringBytesPerButton * NUMBER_OF_BUTTONS;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
char dataString[32];

// variables to hold the parsed data
word addr;
byte data[32];

int dataLength;

boolean newData = false;

bool progMode = false;


const bool debug = false;

char displays[6][2][32];

bool toggle[7][7];
bool toggleHold[7];

// Create pushbuttons on digital pins 5-12
PushButton footSwitch[7] = {
  0,
  PushButton(5),
  PushButton(6),
  PushButton(7),
  PushButton(8),
  PushButton(9),
  PushButton(10)
};

int state = WAITING_STATE; //Initial state
int song = 0; //Song number for PC messages
int page = 1; //Page number for changing MammothMIDI pedal banks

byte active = 0; //Number of active switches

struct configCommand{
	byte device;
	byte command;
	byte param1;
	byte param2;
	byte param3;
	byte param4;
	byte param5;
	byte param6;	
};

configCommand buttonActions[NUMBER_OF_BUTTONS][ACTIONS_PER_BUTTON];
