#include "UniqueCounter.h"

float UniqueCounter::getAVGDist() {
    float sampleAVG = 0.0f;

    sampleAVG = 0.0f;
    sampleAVG += measureDistance() / 3; //평균값 으로 만듦으로써 튀는 값을 잡아내려 함
    sampleAVG += measureDistance() / 3;
    sampleAVG += measureDistance() / 3;

    return sampleAVG;
}

EUniqueCounterState UniqueCounter::getState() {
  return state;
}

void UniqueCounter::setState(EUniqueCounterState s) {
  state = s;
  if (s == EUniqueCounterState::IDLE) {
    timeLeftInMilli = timeLimitInMin * 60 * 1000;
  }
}

void UniqueCounter::sendMeasurementData() {
  Serial.println("{");
  Serial.print("\"time\": ");
  Serial.print(timeLimitInMin);
  Serial.println(",");
  Serial.print("\"count\": ");
  Serial.println(count);
  Serial.println("}");
  SerialBT.println("{");
  SerialBT.print("\"time\": ");
  SerialBT.print(timeLimitInMin);
  SerialBT.println(",");
  SerialBT.print("\"count\": ");
  SerialBT.println(count);
  SerialBT.println("}");
}

void UniqueCounter::beep(int freq) {
  if (!freq)
    noTone(speakerPin);
  else
    tone(speakerPin, freq);
}
  
bool UniqueCounter::checkPushup() {
  return false;
}

void UniqueCounter::increaseCount() {
  count++;
}

void UniqueCounter::segmentOutput(){
  static int prevMin = 0;
  static int prevSec = 0;

  segmentOutputPos(7, 0, ":");
  if (prevMin != timeLeftInMilli/1000/60) {
    prevMin = timeLeftInMilli/1000/60;
    segmentOutputPos(5, 0, "  ");
    segmentOutputPos(5, 0, (int)(timeLeftInMilli/1000/60));
  }
  if (prevSec != (timeLeftInMilli/1000)%60) {
    prevSec = (timeLeftInMilli/1000)%60;
    segmentOutputPos(8, 0, "  ");
    segmentOutputPos(8, 0, (int)((timeLeftInMilli/1000)%60));
  }
};

void UniqueCounter::segmentOutputPos(int x, int y, char* string){
  lcd->setCursor(x, y);
  lcd->print(string);
}

void UniqueCounter::segmentOutputPos(int x, int y, int num){
  lcd->setCursor(x, y);
  lcd->print(num);
}

void UniqueCounter::setTimer(int min) {
  timeLimitInMin = min;
  timeLimitInMilli = min * 60 * 1000;
  timeLeftInMilli = min * 60 * 1000;
  startTime = millis();
  count = 0;
}

void UniqueCounter::updateTimer() {
  timeLeftInMilli = timeLimitInMilli - (millis() - startTime);
}

bool UniqueCounter::isCompleted() {
  if (timeLeftInMilli > 2100000000) {
    Serial.println("completed.");
    return true;
  }
  else
    return false;
}

float UniqueCounter::measureDistance() {
  digitalWrite(trigPin, LOW);
  delay(2); //측정의 정확도 때문에 어쩔수 없이 사용
  digitalWrite(trigPin, HIGH);
  delay(10); //측정의 정확도 때문에 어쩔수 없이 사용
  digitalWrite(trigPin, LOW);

  return 340 * pulseIn(echoPin, HIGH) / 20000;
}