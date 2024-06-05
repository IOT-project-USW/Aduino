float UniqueCounter::getAVGDist(int sampleCount) {
    float sampleAVG = 0.0f;

    sampleAVG = 0.0f;
    for (int i = 0; i < sampleCount; i++)
      sampleAVG += measureDistance() / sampleCount; //평균값 으로 만듦으로써 튀는 값을 잡아내려 함

    return sampleAVG;
  }

float UniqueCounter::measureDistance() {
  digitalWrite(TRIG, LOW);
  delay(2); //측정의 정확도 때문에 어쩔수 없이 사용
  digitalWrite(TRIG, HIGH);
  delay(10); //측정의 정확도 때문에 어쩔수 없이 사용
  digitalWrite(TRIG, LOW);

  return 340 * pulseIn(ECHO, HIGH) / 20000;
}