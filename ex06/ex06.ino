// ex06 双通道反相PWM警车双闪灯效 - 最终版
// 功能：两个LED反相平滑渐变，A亮B暗，A暗B亮，循环往复
#define LED_A_PIN 2    // 灯A引脚（板载LED）
#define LED_B_PIN 17   // 灯B引脚（外接LED）

int brightnessA = 0;          // 灯A的PWM占空比 范围0~255
int fadeDir = 1;              // 渐变方向：1=亮度增加，-1=亮度降低
unsigned long lastFadeTime = 0; // 上一次更新亮度的时间戳
const int fadeInterval = 12;  // 亮度更新间隔，数值越小渐变速度越快

void setup() {
  Serial.begin(115200);
  // 初始化两个引脚为输出模式，启用独立PWM通道
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  
  // 初始状态：灯A全灭，灯B全亮
  analogWrite(LED_A_PIN, 0);
  analogWrite(LED_B_PIN, 255);
  
  Serial.println("=== 双通道反相PWM双闪灯效启动 ===");
}

void loop() {
  unsigned long currentTime = millis();

  // 非阻塞定时更新亮度，不占用CPU资源
  if (currentTime - lastFadeTime >= fadeInterval) {
    lastFadeTime = currentTime;

    // 更新灯A的亮度值
    brightnessA += fadeDir;

    // 边界检测：到达最大/最小亮度时反转方向
    if (brightnessA >= 255) {
      brightnessA = 255; // 防止数值溢出
      fadeDir = -1;
      Serial.println("反转方向：灯A开始变暗，灯B开始变亮");
    } 
    else if (brightnessA <= 0) {
      brightnessA = 0;   // 防止数值溢出
      fadeDir = 1;
      Serial.println("反转方向：灯A开始变亮，灯B开始变暗");
    }

    // 双通道独立PWM输出：灯B亮度 = 255 - 灯A亮度，实现完全反相
    analogWrite(LED_A_PIN, brightnessA);
    analogWrite(LED_B_PIN, 255 - brightnessA);
  }
}