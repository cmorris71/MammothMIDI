#define TIMELINE 1
#define BIGSKY 2
#define HX_STOMP 3
#define SONG_TEXT_LENGTH 12
#define MUX_Address 0x70 // TCA9548A Encoders address

extern void updateDisplays();

typedef void (*function) ();

//========== Clicked Actions ==========
void p1_display() {
  strcpy(displays[0][0], "Button 1-1");
  strcpy(displays[0][1], "Button 1-2");
  strcpy(displays[1][0], "Button 1-3");
  strcpy(displays[1][1], "Button 1-4");
  strcpy(displays[2][0], "Button 1-5");
  strcpy(displays[2][1], "Button 1-6");
};
void p1_s1() {
  if (debug)Serial.println("Switch :s1");
};
void p1_s2() {

};
void p1_s3() {

};
void p1_s4() {

};
void p1_s5() {

};
void p1_s6() {

};
void p2_display() {
  strcpy(displays[0][0], "Button 2-1");
  strcpy(displays[0][1], "Button 2-2");
  strcpy(displays[1][0], "Button 2-3");
  strcpy(displays[1][1], "Button 2-4");
  strcpy(displays[2][0], "Button 2-5");
  strcpy(displays[2][1], "Button 2-6");
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
  strcpy(displays[0][0], "Button 3-1");
  strcpy(displays[0][1], "Button 3-2");
  strcpy(displays[1][0], "Button 3-3");
  strcpy(displays[1][1], "Button 3-4");
  strcpy(displays[2][0], "Button 3-5");
  strcpy(displays[2][1], "Button 3-6");
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
  strcpy(displays[0][0], "Button 4-1");
  strcpy(displays[0][1], "Button 4-2");
  strcpy(displays[1][0], "Button 4-3");
  strcpy(displays[1][1], "Button 4-4");
  strcpy(displays[2][0], "Button 4-5");
  strcpy(displays[2][1], "Button 4-6");
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
  song--;
  if (debug)Serial.println("Song :" + String(song));
  //  showSong(String(songNumber));
  //  MIDI.sendProgramChange(songNumber, HX_STOMP);
  //  MIDI.sendProgramChange(songNumber * 2, TIMELINE);
  //  MIDI.sendProgramChange(songNumber * 3, BIGSKY);
};

void s2_s3() {
  song++;
  if (debug)Serial.println("Song :" + String(song));
  //  showSong(String(songNumber));
  //  MIDI.sendProgramChange(songNumber, HX_STOMP);
  //  MIDI.sendProgramChange(songNumber * 2, TIMELINE);
  //  MIDI.sendProgramChange(songNumber * 3, BIGSKY);
};

void s4_s5() {
  page--;
  clickActions[page][0]();
  updateDisplays();
  if (debug)Serial.println("Page: " + String(page));
};

void s5_s6() {
  page++;
  clickActions[page][0]();
  updateDisplays();
  if (debug)Serial.println("Page: " + String(page));
};

//========== Clicked Actions ==========
void holdS1() {

};
void holdS2() {

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
