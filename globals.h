// State machine definitions
#define WAITING_STATE 0
#define PRE_BUTTON_STATE 1
#define CLICKED_BUTTON_STATE 2
#define HELD_BUTTON_STATE 3
#define DOUBLE_BUTTON_STATE 4

bool debug = true;

char displays[3][2][12];


// Create pushbuttons on digital pins 2-9
PushButton footSwitch[7] = {
  NULL,
  PushButton(2),
  PushButton(3),
  PushButton(4),
  PushButton(5),
  PushButton(6),
  PushButton(7)
};

int state = WAITING_STATE; //Initial state
int song = 0; //Song number for PC messages
int page = 1; //Page number for changing MammothMIDI pedal banks

byte active = 0; //Number of active switches
