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

typedef void (*function) ();


//========== Clicked Actions ==========
void p1_display() {
  strcpy(displays[0][0], "Chain Breaker");
  strcpy(displays[1][0], "Holy Ground");
  strcpy(displays[2][0], "Build My Life");
  strcpy(displays[0][1], "<Tuner>");
  strcpy(displays[1][1], "Good Good...");
  strcpy(displays[2][1], "No Longer Sl...");
  updateDisplays();
};
void p1_s1() {
 midiCC(68, 0, HX_STOMP);
};
void p1_s2() {
  song = 3;
  sendSong();
};
void p1_s3() {
EEPROM.put(10, displays[0][0]);
if(debug) Serial.println("EEPROM put");
};
void p1_s4() {
    song = 0;
  sendSong();
};
void p1_s5() {
  song = 1;
  sendSong();
};
void p1_s6() {
EEPROM.get(10,displays[0][0]);
if(debug) Serial.println("EEPROM get" + String(displays[0][0]));
updateDisplay(0);

};
void p2_display() {
};
void p2_s1() {

};
void p2_s2() {

};
void p2_s3() {

};
void p2_s4() {

};
void p2_s5() {

};
void p2_s6() {

};
void p3_display() {
};
void p3_s1() {

};
void p3_s2() {

};
void p3_s3() {

};
void p3_s4() {

};
void p3_s5() {

};
void p3_s6() {

};
void p4_display() {

};
void p4_s1() {

};
void p4_s2() {

};
void p4_s3() {

};
void p4_s4() {

};
void p4_s5() {

};
void p4_s6() {

};
void p5_display() {

};
void p5_s1() {

};
void p5_s2() {

};
void p5_s3() {

};
void p5_s4() {

};
void p5_s5() {

};
void p5_s6() {

};
void p6_display() {

};
void p6_s1() {

};
void p6_s2() {

};
void p6_s3() {

};
void p6_s4() {

};
void p6_s5() {

};
void p6_s6() {

};

function clickActions[7][7] {
  { NULL, NULL, NULL , NULL , NULL , NULL , NULL },
  { p1_display, p1_s1, p1_s2, p1_s3, p1_s4, p1_s5, p1_s6},
  { p2_display, p2_s1, p2_s2, p2_s3, p2_s4, p2_s5, p2_s6},
  { p3_display, p3_s1, p3_s2, p3_s3, p3_s4, p3_s5, p3_s6},
  { p4_display, p4_s1, p4_s2, p4_s3, p4_s4, p4_s5, p4_s6},
  { p5_display, p5_s1, p5_s2, p5_s3, p5_s4, p5_s5, p5_s6},
  { p6_display, p6_s1, p6_s2, p6_s3, p6_s4, p6_s5, p6_s6}
};

//========== Double Button Actions ==========
void s1_s2() {
  if (song > 0) {
    song--;
  }
  if (debug)Serial.println("Song :" + String(song));
  sendSong();

};

void s2_s3() {
  if (song <= 99) {
    song++;
  }
  sendSong();
  if (debug)Serial.println("Song :" + String(song));

};

void s4_s5() {
//  page--;
//  clickActions[page][0]();
//
//  updateDisplays();
//  if (debug)Serial.println("Page: " + String(page));
};

void s5_s6() {
//  page++;
//  clickActions[page][0]();
//  updateDisplays();
//  if (debug)Serial.println("Page: " + String(page));
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
  midiPC(0, HX_STOMP);
  midiPC(0 * 2, TIMELINE);
  midiPC(0 * 3, BIGSKY);
};
void holdS3() {

};
void holdS4() {

};
void holdS5() {

};
void holdS6() {

};





function doubleActions[7][7] {
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL},
  { NULL, NULL, s1_s2, NULL, NULL, NULL, NULL},
  { NULL, NULL, NULL, s2_s3, NULL, NULL, NULL},
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL},
  { NULL, NULL, NULL, NULL, NULL, s4_s5, NULL},
  { NULL, NULL, NULL, NULL, NULL, NULL, s5_s6}
};

function holdActions[7] {NULL, holdS1, holdS2, holdS3, holdS4, holdS5, holdS6};
