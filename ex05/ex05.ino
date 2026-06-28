// ex05 多档位触摸调速呼吸灯 - 基础框架
#define TOUCH_PIN 4    // 触摸引脚
#define LED_PIN 2      // PWM输出LED引脚
#define THRESHOLD 200   // 触摸触发阈值

int gear = 1;         // 呼吸速度档位，初始1档，范围1~3
int brightness = 0;   // PWM占空比 0~255
int fadeDir = 1;      // 呼吸方向：1=渐亮，-1=渐暗

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
}

void loop() {
  // 后续填充触摸检测 + 呼吸逻辑
}