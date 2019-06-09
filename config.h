#define TIMELINE 1
#define BIGSKY 2
#define HX_STOMP 3
#define SONG_TEXT_LENGTH 12

typedef void (*function) ();

//========== Clicked Actions ==========
void p1_s1() {
  Serial.println("p1 s1 Action");
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

//========== Double Button Actions ==========
void s1_s2(){
  
};
void s2_s3(){
  
};
void s4_s5(){
  
};
void s5_s6(){
  
};

//========== Clicked Actions ==========
void holdS1(){
  
};
void holdS2(){
  
};
void holdS3(){
  
};
void holdS4(){
  
};
void holdS5(){
  
};
void holdS6(){
  
};



function clickActions[7][7] {
  { NULL, NULL, NULL , NULL , NULL , NULL , NULL },
  { NULL,p1_s1, p1_s2, p1_s3, p1_s4, p1_s5, p1_s6},
  { NULL,p2_s1, p2_s2, p2_s3, p2_s4, p2_s5, p2_s6},
  { NULL,p3_s1, p3_s2, p3_s3, p3_s4, p3_s5, p3_s6},
  { NULL,p4_s1, p4_s2, p4_s3, p4_s4, p4_s5, p4_s6},
  { NULL,p5_s1, p5_s2, p5_s3, p5_s4, p5_s5, p5_s6},
  { NULL,p6_s1, p6_s2, p6_s3, p6_s4, p6_s5, p6_s6}
};

function doubleActions[7][7] {
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL},
  { NULL, NULL,s1_s2, NULL, NULL, NULL, NULL},
  { NULL, NULL, NULL,s2_s3, NULL, NULL, NULL},
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL},
  { NULL, NULL, NULL, NULL, NULL,s4_s5, NULL},
  { NULL, NULL, NULL, NULL, NULL, NULL,s5_s6}
};

function holdActions[7]{
  {NULL, holdS1, holdS2, holdS3, holdS4, holdS5, holdS6}
};
