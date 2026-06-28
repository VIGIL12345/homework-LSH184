// ex04 触摸自锁开关实验
#define TOUCH_PIN 4    // 触摸引脚T0，对应GPIO4
#define LED_PIN 2      // 板载LED引脚
#define THRESHOLD 200   // 触摸触发阈值，低于该值判定为触摸

// 布尔型LED状态变量
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // 后续填充触摸检测逻辑
}