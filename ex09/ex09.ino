// ex09 实时传感器Web仪表盘 - 基础框架
#include <WiFi.h>
#include <WebServer.h>

#define TOUCH_PIN 4   // 触摸传感器引脚

// ========== 请修改为你的WiFi名称和密码 ==========
const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);
int touchValue = 0; // 触摸传感器实时数值

void setup() {
  Serial.begin(115200);

  // 连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("仪表盘地址：");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient(); // 处理网页请求
  // 后续填充数据采集与接口逻辑
}