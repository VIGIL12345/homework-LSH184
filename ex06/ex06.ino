// ex06 双通道反相PWM双闪灯效 - 基础框架
#define LED_A_PIN 2   // LED_A引脚（板载LED）
#define LED_B_PIN 17  // LED_B引脚（外接LED）

int brightnessA = 0;   // LED_A占空比 0~255
int fadeDir = 1;       // 渐变方向：1=渐亮，-1=渐暗
unsigned long lastFadeTime = 0; // 渐变更新时间戳
const int fadeInterval = 15;    // 渐变步长间隔（毫秒）

void setup() {
  Serial.begin(115200);
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  analogWrite(LED_A_PIN, 0);
  analogWrite(LED_B_PIN, 255); // 初始状态：A灭，B最亮
}

void loop() {
  // 后续填充反相渐变逻辑
}