#include "clock.h"

// methods ---------------------------------------------------------------------
void Clock::init(uint8_t data, uint8_t clock, uint8_t latch, uint8_t dot,
                 uint8_t switch_1, uint8_t switch_2, uint8_t switch_3, uint16_t debounce)
{
  // set variables
  pin_dot = dot;

  // set pin mode
  pinMode(pin_dot, OUTPUT);

  // set debounced buttons
  button_1.attach(switch_1, INPUT_PULLUP);
  button_2.attach(switch_2, INPUT_PULLUP);
  button_3.attach(switch_3, INPUT_PULLUP);
  button_1.interval(debounce);
  button_2.interval(debounce);
  button_3.interval(debounce);

  // initialize shift registers
  shift.setBitCount(16);
  shift.setPins(data, clock, latch);

  // set diplay to zero with dot on
  digitalWrite(pin_dot, HIGH);

  // begin communication with rtc and start oscillator
  uint8_t counter = 0;
  // if cannot connect to rtc
  while (!real_time.begin())
  {
    // count
    this->display(counter, counter, true, true);
    if (++counter > 99) counter = 0;
    // and blink fast
    digitalWrite(pin_dot, HIGH);
    delay(200);
    digitalWrite(pin_dot, LOW);
    delay(800);

    // debug
    Serial.println("cannot connect. reattempting...");
  }
  // if connected -> turn dot on and start rtc
  while (!real_time.deviceStatus())
  {
    Serial.println("Oscillator is off, turning it on.");
    bool deviceStatus = real_time.deviceStart();
    if (!deviceStatus)
    {
      Serial.println("Oscillator did not start, trying again.");

      this->display(counter, counter, true, true);
      if (++counter > 99) counter = 0;
      // and blink fast
      digitalWrite(pin_dot, HIGH);
      delay(200);
      digitalWrite(pin_dot, LOW);
      delay(800);
    }
  }
  digitalWrite(pin_dot, HIGH);

  // debug
  Serial.println("connected");

  // update and display the time
  this->updateTime();
  this->display();
}

bool Clock::updateTime()
{
  // get time from rtc
  now_time = real_time.now();

  // debug
  Serial.println("H: " + String(now_time.hour()) +
                 "\tM: " + String(now_time.minute()) +
                 "\tS: " + String(now_time.second()));

  // return true if time has changed
  if (now_time.minute() != last.minute() || now_time.hour() != last.hour()) // if time changed
  {
    last = now_time;
    return true;
  }

  return false;
}

void Clock::setTime(uint8_t hour, uint8_t minute)
{
  real_time.adjust(DateTime(2019, 3, 23, hour, minute, 0));
}

uint8_t Clock::getMinute()
{
  return now_time.minute();
}

uint8_t Clock::getHour()
{
  return now_time.hour();
}

uint8_t Clock::updateButtons()
{
  button_1.update();
  button_2.update();
  button_3.update();

  if (button_1.fell()) return pressed_1;
  if (button_2.fell()) return pressed_2;
  if (button_3.fell()) return pressed_3;
  return 0;
}

bool Clock::evenSecond()
{
  if (now_time.second() % 2 == 0) return true;
  else                       return false;
}

void Clock::setDot(bool state)
{
  if (state) digitalWrite(pin_dot, HIGH);
  else       digitalWrite(pin_dot, LOW);
}

bool Clock::display()
{
  return display(now_time.hour(), now_time.minute(), true, true);
}

bool Clock::display(uint8_t hour, uint8_t minute,
                    bool hour_on, bool minute_on)
{
  // test out of bounds
  if (hour > 99 || minute > 99)
  {
    return false;
  }

  uint8_t h_1;
  uint8_t h_2;
  uint8_t m_1;
  uint8_t m_2;
  // get single digits or turn display off
  if (hour_on)
  {
    h_1 = hour / 10;
    h_2 = hour % 10;
  }
  else
  {
    h_1 = 10;
    h_2 = 10;
  }
  if (minute_on)
  {
    m_1 = minute / 10;
    m_2 = minute % 10;
  }
  else
  {
    m_1 = 10;
    m_2 = 10;
  }

  shift.batchWriteBegin();

  // minute 1
  shift.writeBit(0, table[m_1][3]);
  shift.writeBit(1, table[m_1][2]);
  shift.writeBit(2, table[m_1][1]);
  shift.writeBit(3, table[m_1][0]);

  // minute 2
  shift.writeBit(4, table[m_2][3]);
  shift.writeBit(5, table[m_2][2]);
  shift.writeBit(6, table[m_2][1]);
  shift.writeBit(7, table[m_2][0]);

  // hour 1
  shift.writeBit(8, table[h_1][3]);
  shift.writeBit(9, table[h_1][2]);
  shift.writeBit(10, table[h_1][1]);
  shift.writeBit(11, table[h_1][0]);

  // hour 2
  shift.writeBit(12, table[h_2][3]);
  shift.writeBit(13, table[h_2][2]);
  shift.writeBit(14, table[h_2][1]);
  shift.writeBit(15, table[h_2][0]);

  shift.batchWriteEnd();

  return true;
}
