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


const bool debug = false;

char displays[3][2][24];

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
