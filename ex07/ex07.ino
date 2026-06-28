// ex07 Web网页无极调光器 - 基础框架
#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 2       // PWM调光LED引脚

// ========== 请修改为你的WiFi名称和密码 ==========
const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);  // Web服务器，端口80
int brightness = 0;    // LED亮度 0~255

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

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
}