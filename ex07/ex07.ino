// ex07 Web网页无极调光器 - 核心功能版
#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 2

const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);
int brightness = 0;

// 网页HTML代码：包含滑动条 + 实时调光JS逻辑
const String htmlPage = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>无极调光器</title></head><body style='text-align:center;margin-top:100px;'>"
"<h1>LED无极调光控制</h1>"
"<input type='range' min='0' max='255' value='0' id='slider' style='width:400px;height:30px;'>"
"<p>当前亮度：<span id='val'>0</span></p>"
"<script>"
"const slider = document.getElementById('slider');"
"const valText = document.getElementById('val');"
"slider.oninput = function(){"
"  valText.innerText = this.value;"
"  fetch('/set?brightness=' + this.value);"
"}"
"</script></body></html>";

// 处理根路径请求，返回网页
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// 处理调光请求，设置LED亮度
void handleSetBrightness() {
  if (server.hasArg("brightness")) {
    brightness = server.arg("brightness").toInt();
    analogWrite(LED_PIN, brightness);
    server.send(200, "text/plain", "亮度已设置为：" + String(brightness));
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("控制页面地址：");
  Serial.println(WiFi.localIP());

  // 注册网页路由
  server.on("/", handleRoot);
  server.on("/set", handleSetBrightness);
  server.begin();
  Serial.println("Web服务器已启动");
}

void loop() {
  server.handleClient();
}