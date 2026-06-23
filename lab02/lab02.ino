// 定义LED引脚，ESP32板载LED接GPIO2
const int ledPin = 2;

// millis定时变量
unsigned long previousTime = 0;
const unsigned long blinkInterval = 1000;
bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long currentTime = millis();
  if(currentTime - previousTime >= blinkInterval)
  {
    previousTime = currentTime;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}