// ex04 触摸自锁开关：边缘检测版本
#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 20

bool ledState = false;       // LED当前状态
bool lastTouchState = false; // 上一次触摸状态，用于边缘检测

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // 1. 读取触摸值，判断当前是否被触摸
  int touchValue = touchRead(TOUCH_PIN);
  bool currentTouch = (touchValue < THRESHOLD);

  // 2. 边缘检测：上一次没触摸，本次触摸了 = 按下瞬间
  if (currentTouch == true && lastTouchState == false) {
    // 翻转LED状态
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Serial.print("LED状态切换，当前：");
    Serial.println(ledState ? "点亮" : "熄灭");
  }

  // 3. 更新上一次状态，供下一轮循环判断
  lastTouchState = currentTouch;
}