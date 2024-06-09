#ifndef _UNIQUECOUNTER_H_
#define _UNIQUECOUNTER_H_
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

enum class EUniqueCounterState {
    IDLE = 0b0,
    MEASURING = 0b10,
    COMPLETED = 0b100
};

enum class EPushupState {
    ERROR = 0b000,
    UP = 0b10,
    DOWN = 0b100
};

class UniqueCounter {
public:
  static UniqueCounter& getInstance(int trig, int echo, int startBtn, int SPK) {
    static UniqueCounter s(trig, echo, startBtn, SPK);
    return s;
  }
  float getAVGDist();
  EUniqueCounterState getState();
  void setState(EUniqueCounterState s);
  void sendMeasurementData();
  void beep(int freq);
  bool checkPushup();
  void increaseCount();
  void segmentOutput();
  void segmentOutputPos(int x, int y, char* string);
  void segmentOutputPos(int x, int y, int num);
  void setTimer(int min);
  void updateTimer();
  bool isCompleted();

  EPushupState userPushupState;
  LiquidCrystal_I2C* lcd;
private:
  UniqueCounter(int trig, int echo, int startBtn, int SPK) {
    state = EUniqueCounterState::IDLE;
    count = 0;
    timeLimitInMin = 2 * 60 * 1000;
    timeLimitInMilli = 2 * 60 * 1000;
    timeLeftInMilli = 2 * 60 * 1000;
    startTime = 0;
    trigPin = trig;
    echoPin = echo;
    startBtnPin = startBtn;
    speakerPin = SPK;
    userPushupState = EPushupState::ERROR;
    lcd = new LiquidCrystal_I2C(0x27, 16, 2);
    
    lcd->begin();
    lcd->setCursor(0, 0);

    Serial.begin(115200);
    SerialBT.begin(device_name);  //Bluetooth device name
    //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
    Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  };
  UniqueCounter();
  ~UniqueCounter() {
    delete lcd;
  };
  UniqueCounter(const UniqueCounter& ref) {};
  UniqueCounter& operator=(const UniqueCounter& ref) {};

  float measureDistance();
  
  int timeLimitInMin;
  uint32_t timeLimitInMilli;
  uint32_t timeLeftInMilli;
  uint32_t startTime;
  int count;
  EUniqueCounterState state;
  int trigPin;
  int echoPin;
  int startBtnPin;
  int speakerPin;
  BluetoothSerial SerialBT;
  String device_name = "ESP32-BT-Slave";
};
#endif