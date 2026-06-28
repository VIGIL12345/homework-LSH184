// ex09 实时传感器Web仪表盘 - 核心功能版
#include <WiFi.h>
#include <WebServer.h>

#define TOUCH_PIN 4

const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);

// 仪表盘网页：AJAX定时拉取数据
const String htmlPage = 
"<!DOCTYPE html><html><head><meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>触摸传感器仪表盘</title></head><body style='text-align:center;margin-top:15vh;'>"
"<h1>触摸传感器实时数值</h1>"
"<div id='value' style='font-size:80px;font-weight:bold;margin-top:30px;'>--</div>"
"<script>"
"// 每200毫秒获取一次数据，更新页面"
"setInterval(function(){"
"  fetch('/data').then(res=>res.text()).then(num=>{"
"    document.getElementById('value').innerText = num;"
"  });"
"}, 200);"
"</script></body></html>";

// 返回仪表盘主页
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", htmlPage);
}

// 返回实时触摸数据接口
void handleData() {
  int val = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(val));
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("仪表盘地址：");
  Serial.println(WiFi.localIP());

  // 注册路由
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("传感器仪表盘已启动");
}

void loop() {
  server.handleClient();
}