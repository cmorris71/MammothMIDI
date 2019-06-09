#define TIMELINE 1
#define BIGSKY 2
#define HX_STOMP 3
#define SONG_TEXT_LENGTH 12

// Data structure for midi configurations
struct midiAction {
  byte messageType;  //midiPC or midiCC
  byte data1;        //PC number or CC controller number
  byte data2;        //CC controler value
  byte channel;      //Device number
  char scribble[12]; //"Text" for button
};

const byte midiPC = 0; //MIDI Program Change
const byte midiCC = 1; //MIDI Control Change

//size of MIDI struct
const int maSize = sizeof(struct midiAction); 

struct midiAction p1_s1[] = {
  {midiCC, 0, 0, BIGSKY, "Text" }
};
struct midiAction p1_s2[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p1_s3[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p1_s4[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p1_s5[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p1_s6[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p2_s1[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p2_s2[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p2_s3[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p2_s4[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p2_s5[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p2_s6[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p3_s1[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p3_s2[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p3_s3[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p3_s4[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p3_s5[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p3_s6[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p4_s1[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p4_s2[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p4_s3[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p4_s4[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p4_s5[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p4_s6[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p5_s1[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p5_s2[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p5_s3[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p5_s4[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p5_s5[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p5_s6[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p6_s1[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p6_s2[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p6_s3[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p6_s4[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p6_s5[] = {
  {0, 0, 0, 0, "Text" }
};
struct midiAction p6_s6[] = {
  {0, 0, 0, 0, "Text" }
};


const midiAction * actionMatrix[6][6] {
  {p1_s1, p1_s2, p1_s3, p1_s4, p1_s5, p1_s6},
  {p2_s1, p2_s2, p2_s3, p2_s4, p2_s5, p2_s6},
  {p3_s1, p3_s2, p3_s3, p3_s4, p3_s5, p3_s6},
  {p4_s1, p4_s2, p4_s3, p4_s4, p4_s5, p4_s6},
  {p5_s1, p5_s2, p5_s3, p5_s4, p5_s5, p5_s6},
  {p6_s1, p6_s2, p6_s3, p6_s4, p6_s5, p6_s6}
};

const int sizeMatrix[6][6] {
  {sizeof(p1_s1) / maSize, sizeof(p1_s2) / maSize, sizeof(p1_s3) / maSize, sizeof(p1_s4) / maSize, sizeof(p1_s5) / maSize, sizeof(p1_s6) / maSize},
  {sizeof(p2_s1) / maSize, sizeof(p2_s2) / maSize, sizeof(p2_s3) / maSize, sizeof(p2_s4) / maSize, sizeof(p2_s5) / maSize, sizeof(p2_s6) / maSize},
  {sizeof(p3_s1) / maSize, sizeof(p3_s2) / maSize, sizeof(p3_s3) / maSize, sizeof(p3_s4) / maSize, sizeof(p3_s5) / maSize, sizeof(p3_s6) / maSize},
  {sizeof(p4_s1) / maSize, sizeof(p4_s2) / maSize, sizeof(p4_s3) / maSize, sizeof(p4_s4) / maSize, sizeof(p4_s5) / maSize, sizeof(p4_s6) / maSize},
  {sizeof(p5_s1) / maSize, sizeof(p5_s2) / maSize, sizeof(p5_s3) / maSize, sizeof(p5_s4) / maSize, sizeof(p5_s5) / maSize, sizeof(p5_s6) / maSize},
  {sizeof(p6_s1) / maSize, sizeof(p6_s2) / maSize, sizeof(p6_s3) / maSize, sizeof(p6_s4) / maSize, sizeof(p6_s5) / maSize, sizeof(p6_s6) / maSize}
};

/*
   #include <Wire.h>
  #include <Arduino.h>


  #define MIDIPC 0
  #define MIDICC 1

  #define TIMELINE 1
  #define BIGSKY 2
  #define HX_STOMP 3



  struct midiAction {
  byte messageType;
  byte data1;
  byte data2;
  byte channel;
  };

  const int maSize = sizeof(struct midiAction);

  struct midiAction a1_1[] = {
  {MIDICC, 11, 1, BIGSKY},
  {MIDICC, 11, 2, TIMELINE},
  {MIDICC, 11, 2, TIMELINE},
  {MIDIPC, 11, 2, TIMELINE},
  {MIDICC, 11, 2, TIMELINE},
  {MIDICC, 11, 2, TIMELINE}
  };

  midiAction a1_2[] = {
  {MIDICC, 12, 3, BIGSKY},
  {MIDICC, 12, 4, TIMELINE}
  };

  midiAction a2_1[] = {
  {MIDIPC, 21, 5, BIGSKY},
  };

  midiAction a2_2[] = {
  {MIDIPC, 21, 5, BIGSKY},
  };


  midiAction * matrix[2][2] {
  {a1_1, a1_2},
  {a2_1, a2_2}
  };

  const int sizeMatrix[2][2] {
  {sizeof a1_1 / maSize, sizeof a1_2 / maSize},
  {sizeof a2_1 / maSize, sizeof a2_2 / maSize}
  };


  void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  int actionQty = sizeMatrix[0][0];//sizeof(midiAction);
  //Serial.println(actionQty);

  for (int i = 0; i < actionQty; i++) {
    Serial.println((matrix[0][0]+i)->messageType);
    Serial.println((matrix[0][0]+i)->data2);
    Serial.println((matrix[0][0]+i)->channel);
    Serial.println("============");
  }




  //  Serial.println("Printing (0,1)");
  //  int arr_size = sizeof(*matrix[0][1]) / 2;
  //  for (int i = 0; i < arr_size; i++)
  //  {
  //    Serial.println(String(*matrix[0][1]));
  //  }
  //
  //  Serial.println("Printing (0,0)");
  //  arr_size = sizeof(*matrix[0][0]) * 2;
  //  Serial.println("Size of arrary: " + String(arr_size));
  //  for (int j = 0; j < arr_size + 4; j++)
  //  {
  //    Serial.println(String(*(matrix[0][0] + j)));
  //  }
  }

  void loop() {
  // put your main code here, to run repeatedly:



  }
*/
