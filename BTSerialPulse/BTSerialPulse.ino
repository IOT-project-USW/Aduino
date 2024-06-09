// This example code is in the Public Domain (or CC0 licensed, at your option.)
// By Evandro Copercini - 2018
//
// This example creates a bridge between Serial and Classical Bluetooth (SPP)
// and also demonstrate that SerialBT have the same functionalities of a normal Serial
// Note: Pairing is authenticated automatically by this device

#include "UniqueCounter.h"

int TRIG = 0;
int ECHO = 26;
int START_BTN = 27;
int SPK = 13;
// int LCD_SDA = 22;
// int LCD_SCL = 23;
// int _CONN_PINS[12] = { 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19 }; // 12 11 10 9 8 7 1 2 3 4 5 6
// int DIGIT_SELECT[4] = { 2, 5, 12, 19 };
// int SEG_PINS[8] = { 3, 13, 17, 15, 14, 4, 18, 16 };

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(START_BTN, INPUT);

  UniqueCounter::getInstance(TRIG, ECHO, START_BTN, SPK);
}

void loop() {
  UniqueCounter& counter = UniqueCounter::getInstance(TRIG, ECHO, START_BTN, SPK);

  switch (counter.getState()) {
  case EUniqueCounterState::IDLE:
      if (digitalRead(START_BTN) == HIGH) {
        counter.setState(EUniqueCounterState::MEASURING);
        counter.lcd->clear();
        counter.beep(261);
        counter.segmentOutputPos(7, 0, 2);
        delay(200);
        counter.beep(0);
        delay(800);
        counter.beep(261);
        counter.segmentOutputPos(7, 0, 1);
        delay(200);
        counter.beep(0);
        delay(800);
        counter.beep(261);
        counter.segmentOutputPos(5, 0, "Start!");
        counter.setTimer(2);
        delay(1000);
        counter.beep(0);
        counter.lcd->clear();
      }
      break;
  case EUniqueCounterState::MEASURING:
      //float distance = counter.getAVGDist();
      counter.updateTimer();
      if (counter.checkPushup()) {
        counter.increaseCount();
        counter.beep(261);
      }
      if (counter.isCompleted()){
        counter.setState(EUniqueCounterState::COMPLETED);
        Serial.println("ChState.");
      }
      break;
  case EUniqueCounterState::COMPLETED:
      counter.sendMeasurementData();
      counter.setState(EUniqueCounterState::IDLE);
      break;
  }
  counter.segmentOutput();
}
