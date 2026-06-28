// ex04 触摸自锁开关：边缘检测 + 软件防抖 最终版
#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 20      // 触摸触发阈值
#define DEBOUNCE_MS 50    // 防抖时长50毫秒

bool ledState = false;          // LED当前状态
bool lastTouchState = false;    // 上一次稳定的触摸状态
unsigned long lastDebounceTime = 0; // 防抖时间戳

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  unsigned long now = millis();
  int touchValue = touchRead(TOUCH_PIN);
  bool currentReading = (touchValue < THRESHOLD);

  // 1. 检测到触摸状态变化，重置防抖计时
  if (currentReading != lastTouchState) {
    lastDebounceTime = now;
  }

  // 2. 状态稳定超过防抖时长，判定为有效状态
  if (now - lastDebounceTime > DEBOUNCE_MS) {
    // 边缘检测：从无触摸 变为 有触摸 的瞬间
    if (currentReading == true && lastTouchState == false) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      Serial.print("有效触摸，LED切换为：");
      Serial.println(ledState ? "点亮" : "熄灭");
    }
    // 更新稳定状态
    lastTouchState = currentReading;
  }
}