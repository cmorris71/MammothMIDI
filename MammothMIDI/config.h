#define TIMELINE 1
#define BIGSKY 2
#define HX_STOMP 3
#define SONG_TEXT_LENGTH 12
#define MUX_Address 0x70 // TCA9548A Encoders address

extern void updateDisplays();
extern void updateDisplay(int display);
extern void megaDisplay();
extern void sendSong();
extern void midiPC(int songNumber, int midiChannel);
extern void midiCC(int midiController, int midiValue, int midiChannel);
extern void getConfig();
extern void writeConfig();
extern void printDisplays(int d, char str1[16], char str2[16]);


typedef void (*function) ();


//========== Clicked Actions ==========




//========== Double Button Actions ==========
void s1_s2() {
  if (song > 0) {
    song--;
  }
  if (debug)Serial.println("Song :" + String(song));
  sendSong();

};

void s1_s6() {
//writeConfig();
progMode = !progMode;
if (progMode){
	printDisplays(1,"Writing", "Config");
	}else{
	printDisplays(1,"Exiting", "Config");
	getConfig();
	}

};

void s2_s3() {
  if (song <= 99) {
    song++;
  }
  sendSong();
  if (debug)Serial.println("Song :" + String(song));

};

void s4_s5() {
  if (page > 1) page--;

//  clickActions[page][0]();
//
//  updateDisplays();
  if (debug)Serial.println("Page: " + String(page));
    getConfig();
};

void s5_s6() {
  if (page<6) page++;

//  clickActions[page][0]();
//  updateDisplays();
 if (debug)Serial.println("Page: " + String(page));
   getConfig();
};

//========== Clicked Actions ==========
void holdS1() {
  if (toggleHold[1]) {
    midiCC(68, 127, HX_STOMP);
  } else {
    midiCC(68, 0, HX_STOMP);
  }
  toggleHold[1] = !toggleHold[1];
  if (debug) Serial.print("s1 held");
};
void holdS2() {

};
void holdS3() {
  midiPC(0, HX_STOMP);
  midiPC(0 * 2, TIMELINE);
  midiPC(0 * 3, BIGSKY);
};
void holdS4() {

};
void holdS5() {

};
void holdS6() {

};



function doubleActions[7][7] {
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL},
  { NULL, NULL, s1_s2, NULL, NULL, NULL, s1_s6},
  { NULL, NULL, NULL, s2_s3, NULL, NULL, NULL},
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL},
  { NULL, NULL, NULL, NULL, NULL, s4_s5, NULL},
  { NULL, s1_s6, NULL, NULL, NULL, NULL, s5_s6}
};

function holdActions[7] {NULL, holdS1, holdS2, holdS3, holdS4, holdS5, holdS6};


