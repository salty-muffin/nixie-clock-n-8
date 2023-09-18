#include <MCP7940.h>

#define LED     13
#define DOT     17
#define DATA    2
#define CLOCK   3
#define LATCH   4
#define SWITCH1 6
#define SWITCH2 7
#define SWITCH3 8

MCP7940_Class MCP7940;

void writeTime(uint8_t hour, uint8_t minute) {
  uint8_t hour1 = hour / 10;
  uint8_t hour2 = hour % 10;
  uint8_t minute1 = minute / 10;
  uint8_t minute2 = minute % 10;

  uint8_t hourBinary = hour1 << 4 | hour2;
  uint8_t minuteBinary = minute1 << 4 | minute2;

  // shift out
  digitalWrite(LATCH, LOW);

  shiftOut(DATA, CLOCK, LSBFIRST, minuteBinary);
  shiftOut(DATA, CLOCK, LSBFIRST, hourBinary);

  digitalWrite(LATCH, HIGH);
}

void setup() {
  Serial.begin(9600);

  // set pins
  pinMode(LED, OUTPUT);
  pinMode(DOT, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);

  // connect to rtc
  Serial.println("Connecting to rtc...");
  while (!MCP7940.begin()) {
    Serial.println("Unable to find rtc. Checking again in 1 second.");
    delay(1000);
  }

  // if connected, turn dot on and start rtc
  Serial.println("Starting oscillator...");
  if(!MCP7940.deviceStart()) Serial.println("Unable to start oscillator");
}

void loop() {
  // get time from rtc
  DateTime now = MCP7940.now();

  // debug
  // Serial.println("H: " + String(now.hour()) +
  //                "\tM: " + String(now.minute()) +
  //                "\tS: " + String(now.second()));

  writeTime(now.minute(), now.second());

  digitalWrite(DOT, HIGH);
  delay(500);
  digitalWrite(DOT, LOW);
  delay(500);
}