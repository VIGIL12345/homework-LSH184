// ex05 多档位触摸调速呼吸灯 - 核心功能版
#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 20

int gear = 1;                  // 档位 1/2/3
int brightness = 0;            // PWM占空比
int fadeDir = 1;               // 呼吸方向
unsigned long lastFadeTime = 0;// 呼吸更新时间戳

// 三档对应的呼吸更新间隔（毫秒），数值越小呼吸越快
const int fadeInterval[4] = {0, 30, 12, 4}; // 索引1/2/3对应三档

bool lastTouchState = false;   // 上一次触摸状态

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  Serial.println("呼吸灯启动，当前1档（慢速）");
}

void loop() {
  unsigned long now = millis();

  // ========== 1. 触摸检测 + 档位切换 ==========
  int touchVal = touchRead(TOUCH_PIN);
  bool curTouch = (touchVal < THRESHOLD);

  // 边缘检测：从无触摸变为有触摸的瞬间
  if (curTouch == true && lastTouchState == false) {
    gear++;
    if (gear > 3) gear = 1; // 3档后回到1档，循环
    Serial.print("切换到");
    Serial.print(gear);
    Serial.println("档");
  }
  lastTouchState = curTouch;

  // ========== 2. 非阻塞PWM呼吸灯 ==========
  if (now - lastFadeTime >= fadeInterval[gear]) {
    lastFadeTime = now;
    brightness += fadeDir;

    // 到边界反转呼吸方向
    if (brightness <= 0 || brightness >= 255) {
      fadeDir = -fadeDir;
    }
    analogWrite(LED_PIN, brightness);
  }
}