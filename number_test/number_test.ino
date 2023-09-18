// nixie clock r1.1
// number test
// zeno gries

// libraries -------------------------------------------------------------------
#include <Shifty.h>

// settings -------------------------------------------------------------------
// bit table for tube driver
const uint8_t table[16][4] = {{0, 0, 0, 0},  // 0:
                              {0, 0, 0, 1},  // 0:
                              {0, 0, 1, 0},  // 0:
                              {0, 0, 1, 1},  // 0:
                              {0, 1, 0, 0},  // 0:
                              {0, 1, 0, 1},  // 0:
                              {0, 1, 1, 0},  // 0:
                              {0, 1, 1, 1},  // 0:
                              {1, 0, 0, 0},  // 0:
                              {1, 0, 0, 1},  // 0:
                              {1, 0, 1, 0},  // 0:
                              {1, 0, 1, 1},  // 0:
                              {1, 1, 0, 0},  // 0:
                              {1, 1, 0, 1},  // 0:
                              {1, 1, 1, 0},  // 0:
                              {1, 1, 1, 1}}; // 0:

// pins ------------------------------------------------------------------------
#define DATA    2
#define CLOCK   3
#define LATCH   4

#define DOT    17

// objects ---------------------------------------------------------------------
Shifty shift;

// variables -------------------------------------------------------------------
uint8_t number = 2;
bool dot = true;

void setup()
{
  Serial.begin(9600);
  Serial.println("testing number " + String(number) + ". Dot is " + String(dot));

  // set pin mode
  pinMode(DOT, OUTPUT);

  // initialize shift registers
  shift.setBitCount(16);
  shift.setPins(DATA, CLOCK, LATCH);

  Serial.println(String(number) + ": " +
                 String(table[number][0]) +
                 String(table[number][1]) +
                 String(table[number][2]) +
                 String(table[number][3]));

  shift.batchWriteBegin();

  // minute 1
  shift.writeBit(0, table[number][3]);
  shift.writeBit(1, table[number][2]);
  shift.writeBit(2, table[number][1]);
  shift.writeBit(3, table[number][0]);

  // minute 2
  shift.writeBit(4, table[number][3]);
  shift.writeBit(5, table[number][2]);
  shift.writeBit(6, table[number][1]);
  shift.writeBit(7, table[number][0]);

  // hour 1
  shift.writeBit(8, table[number][3]);
  shift.writeBit(9, table[number][2]);
  shift.writeBit(10, table[number][1]);
  shift.writeBit(11, table[number][0]);

  // hour 2
  shift.writeBit(12, table[number][3]);
  shift.writeBit(13, table[number][2]);
  shift.writeBit(14, table[number][1]);
  shift.writeBit(15, table[number][0]);

  shift.batchWriteEnd();

  if (dot)
    digitalWrite(DOT, HIGH);
  else
    digitalWrite(DOT, LOW);
}

void loop() {}
