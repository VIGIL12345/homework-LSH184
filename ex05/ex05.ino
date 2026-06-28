// ex05 多档位触摸调速呼吸灯 - 最终版（边缘检测 + 软件防抖）
#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 20    // 触摸触发阈值
#define DEBOUNCE_MS 50  // 软件防抖时长

int gear = 1;                  // 呼吸档位：1慢 / 2中 / 3快
int brightness = 0;            // PWM占空比 0~255
int fadeDir = 1;               // 呼吸方向：1渐亮，-1渐暗
unsigned long lastFadeTime = 0;// 呼吸更新时间戳

// 三档呼吸速度配置（间隔毫秒，值越小速度越快）
const int fadeInterval[4] = {0, 30, 12, 4};

// 触摸防抖相关变量
bool lastStableTouch = false;   // 上一次稳定的触摸状态
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  Serial.println("呼吸灯启动，当前1档（慢速呼吸）");
}

void loop() {
  unsigned long now = millis();

  // ========== 1. 触摸检测 + 软件防抖 ==========
  int touchVal = touchRead(TOUCH_PIN);
  bool curReading = (touchVal < THRESHOLD);

  // 状态变化时重置防抖计时
  if (curReading != lastStableTouch) {
    lastDebounceTime = now;
  }

  // 状态稳定超过防抖时长，判定为有效状态
  if (now - lastDebounceTime > DEBOUNCE_MS) {
    // 边缘检测：无触摸 → 有触摸 的瞬间
    if (curReading == true && lastStableTouch == false) {
      gear++;
      if (gear > 3) gear = 1;
      Serial.print("档位切换：");
      Serial.println(gear);
    }
    lastStableTouch = curReading;
  }

  // ========== 2. 非阻塞PWM呼吸渐变 ==========
  if (now - lastFadeTime >= fadeInterval[gear]) {
    lastFadeTime = now;
    brightness += fadeDir;

    // 到达亮度边界反转方向
    if (brightness <= 0 || brightness >= 255) {
      fadeDir = -fadeDir;
    }
    analogWrite(LED_PIN, brightness);
  }
}