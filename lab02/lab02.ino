// 定义LED引脚，ESP32开发板板载LED默认连接GPIO2
const int ledPin = 2;

// 记录上一次LED状态切换的系统时间（毫秒）
unsigned long previousTime = 0;
// 状态切换间隔：1000毫秒 = 1秒
const unsigned long blinkInterval = 1000;
// 保存LED当前电平状态
bool ledState = LOW;

void setup() {
  // 初始化串口通信，波特率115200，用于串口监视器调试
  Serial.begin(115200);
  // 将LED引脚配置为输出模式
  pinMode(ledPin, OUTPUT);
  // 上电默认熄灭LED
  digitalWrite(ledPin, LOW);
}

void loop() {
  // 获取系统当前运行总毫秒数
  unsigned long currentTime = millis();

  // 判断是否达到1秒间隔，满足则翻转LED亮灭
  if(currentTime - previousTime >= blinkInterval)
  {
    // 更新时间戳
    previousTime = currentTime;
    // 电平取反：亮变灭、灭变亮
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
  // 本程序无delay阻塞，loop可以同时运行其他任务
}