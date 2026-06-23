// 定义LED引脚，ESP32板载LED接GPIO2
const int ledPin = 2;

void setup() {
  // 初始化串口，波特率115200
  Serial.begin(115200);
  // 设置LED引脚为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 后续填充millis闪烁逻辑
}