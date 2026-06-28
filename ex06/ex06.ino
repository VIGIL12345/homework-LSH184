// ex06 双通道反相PWM双闪灯效 - 核心功能版
#define LED_A_PIN 2
#define LED_B_PIN 17

int brightnessA = 0;
int fadeDir = 1;
unsigned long lastFadeTime = 0;
const int fadeInterval = 15;

void setup() {
  Serial.begin(115200);
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  // 初始状态：A全灭，B全亮
  analogWrite(LED_A_PIN, 0);
  analogWrite(LED_B_PIN, 255);
  Serial.println("双通道反相PWM灯效启动");
}

void loop() {
  unsigned long now = millis();

  // 定时更新PWM占空比
  if (now - lastFadeTime >= fadeInterval) {
    lastFadeTime = now;
    
    // 更新LED_A亮度
    brightnessA += fadeDir;
    
    // 到达亮度边界时反转方向
    if (brightnessA <= 0 || brightnessA >= 255) {
      fadeDir = -fadeDir;
    }

    // 输出PWM：LED_B亮度与LED_A完全反相
    analogWrite(LED_A_PIN, brightnessA);
    analogWrite(LED_B_PIN, 255 - brightnessA);
  }
}