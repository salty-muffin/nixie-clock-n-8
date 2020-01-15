// nixie clock r1.1
// number test
// zeno gries

// libraries -------------------------------------------------------------------
#include <Metro.h>
#include <Shifty.h>

// settings -------------------------------------------------------------------
// bit table for tube driver
const uint8_t table[16][4] = {{0, 0, 0, 0},  // 0: -
                              {0, 0, 0, 1},  // 0: 8
                              {0, 0, 1, 0},  // 0: 4
                              {0, 0, 1, 1},  // 0: 
                              {0, 1, 0, 0},  // 0: 2
                              {0, 1, 0, 1},  // 0: -
                              {0, 1, 1, 0},  // 0: 6
                              {0, 1, 1, 1},  // 0: 
                              {1, 0, 0, 0},  // 0: 
                              {1, 0, 0, 1},  // 0: 
                              {1, 0, 1, 0},  // 0: 5
                              {1, 0, 1, 1},  // 0: 
                              {1, 1, 0, 0},  // 0: 
                              {1, 1, 0, 1},  // 0: -
                              {1, 1, 1, 0},  // 0: 7
                              {1, 1, 1, 1}}; // 0: 

// pins ------------------------------------------------------------------------
const uint8_t DATA  = 2;
const uint8_t CLOCK = 3;
const uint8_t LATCH = 4;

const uint8_t DOT = 5;

// objects ---------------------------------------------------------------------
Metro time(5000);
Metro dot(1000);

Shifty shift;

// variables -------------------------------------------------------------------
uint8_t counter = 0;
uint8_t dot_state = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("number test");

  // set pin mode
  pinMode(DOT, OUTPUT);

  // initialize shift registers
  shift.setBitCount(16);
  shift.setPins(DATA, CLOCK, LATCH);
}

void loop()
{
  if (time.check())
  {
    Serial.println(String(counter) + ": " +
                   String(table[counter][0]) + 
                   String(table[counter][1]) + 
                   String(table[counter][2]) + 
                   String(table[counter][3]));
                   
    shift.batchWriteBegin();

    // minute 1
    shift.writeBit(0, table[counter][0]);
    shift.writeBit(1, table[counter][1]);
    shift.writeBit(2, table[counter][2]);
    shift.writeBit(3, table[counter][3]);
  
    // minute 2
    shift.writeBit(4, table[counter][0]);
    shift.writeBit(5, table[counter][1]);
    shift.writeBit(6, table[counter][2]);
    shift.writeBit(7, table[counter][3]);
  
    // hour 1
    shift.writeBit(8, table[counter][0]);
    shift.writeBit(9, table[counter][1]);
    shift.writeBit(10, table[counter][2]);
    shift.writeBit(11, table[counter][3]);
  
    // hour 2
    shift.writeBit(12, table[counter][0]);
    shift.writeBit(13, table[counter][1]);
    shift.writeBit(14, table[counter][2]);
    shift.writeBit(15, table[counter][3]);
  
    shift.batchWriteEnd();

    if (++counter >= 16) counter = 0;
  }
  if (dot.check())
  {
    if (dot_state) digitalWrite(DOT, HIGH);
    else           digitalWrite(DOT, LOW);

    dot_state = !dot_state;
  }
}
