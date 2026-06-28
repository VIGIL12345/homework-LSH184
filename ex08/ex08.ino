// ex08 物联网安防报警器 - 基础框架
#include <WiFi.h>
#include <WebServer.h>

#define TOUCH_PIN 4   // 触摸检测引脚
#define LED_PIN 2     // 报警LED引脚

// ========== 请修改为你的WiFi名称和密码 ==========
const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);

// 系统状态变量
bool isArmed = false;    // 系统是否布防
bool isAlarming = false; // 是否处于报警状态

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // 连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("控制页面地址：");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient(); // 处理网页请求
  // 后续填充触摸检测 + 报警逻辑
}