// This example code is in the Public Domain (or CC0 licensed, at your option.)
// By Evandro Copercini - 2018
//
// This example creates a bridge between Serial and Classical Bluetooth (SPP)
// and also demonstrate that SerialBT have the same functionalities of a normal Serial
// Note: Pairing is authenticated automatically by this device

#include "BluetoothSerial.h"

String device_name = "ESP32-BT-Slave";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
int TRIG = 0;
int ECHO = 2;
int pos_pins[] = {8,11,12,7}; // 몇번째 세그먼트
int num_of_pos = 4;
int pins[] = {9,13,5,3,2,10,6,4}; // 세그먼트 a, b, c, d, e, f, g, dp
int num_of_pins = 8;
int delaytime = 5;

int minute = 0;
int seconds = 0;

   

class UniqueCounter {
public:
  static UniqueCounter& getInstance() {
    static UniqueCounter s;
    return s;
  }
  float getAVGDist(int sampleCount);
private:
  UniqueCounter() {};
  ~UniqueCounter() {};
  UniqueCounter(const UniqueCounter& ref) {};
  UniqueCounter& operator=(const UniqueCounter& ref) {};

  float measureDistance();
  void segment(int min){
     bool segment[10][8] = {
      {true, true, true, true, true, true, false, false}, //0
      {false, true, true, false, false, false, false, false}, //1
      {true, true, false, true, true, false, true, false}, //2
      {true, true, true, true, false, false, true, false}, //3
      {false, true, true, false, false, true, true, false}, //4
      {true, false, true, true, false, true, true, false}, //5
      {true, false, true, true, true, true, true, false}, //6
      {true, true, true, false, false, false, false, false}, //7
      {true, true, true, true, true, true, true, false}, //8
      {true, true, true, true, false, true, true, false} //9
    };
    
  };
};


float UniqueCounter::getAVGDist(int sampleCount) {
    float sampleAVG = 0.0f;

    sampleAVG = 0.0f;
    for (int i = 0; i < sampleCount; i++)
      sampleAVG += measureDistance() / sampleCount;

    return sampleAVG;
  }

float UniqueCounter::measureDistance() {
  digitalWrite(TRIG, LOW);
  delay(2);
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);

  return 340 * pulseIn(ECHO, HIGH) / 20000;
}

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
}

void loop() {
  UniqueCounter& counter = UniqueCounter::getInstance();
  float distance = counter.getAVGDist(3);
  if (Serial.available()) {
    Serial.print("Distance:");
    Serial.print(distance);
    Serial.println(" cm");
  }
  if (SerialBT.available()) {
    SerialBT.print("Distance:");
    SerialBT.print(distance);
    SerialBT.println(" cm");
  }
}
