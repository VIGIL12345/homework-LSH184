// ex09 实时传感器Web仪表盘 - 最终版
// 功能：网页实时显示触摸传感器模拟量数值，AJAX无刷新自动更新
#include <WiFi.h>
#include <WebServer.h>

#define TOUCH_PIN 4   // 触摸检测引脚

// ========== WiFi配置 ==========
const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);
const long refreshInterval = 150; // 网页数据刷新间隔（毫秒）

// 美化后的仪表盘网页
const String htmlPage = 
"<!DOCTYPE html><html lang='zh-CN'><head>"
"<meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>触摸传感器实时仪表盘</title>"
"<style>"
"* {margin: 0; padding: 0; box-sizing: border-box;}"
"body {"
"  font-family: '微软雅黑', sans-serif;"
"  background: linear-gradient(135deg, #1e3c72, #2a5298);"
"  min-height: 100vh;"
"  display: flex;"
"  align-items: center;"
"  justify-content: center;"
"  color: white;"
"}"
".dashboard {"
"  background: rgba(255,255,255,0.1);"
"  backdrop-filter: blur(10px);"
"  padding: 60px 80px;"
"  border-radius: 20px;"
"  text-align: center;"
"  box-shadow: 0 8px 32px rgba(0,0,0,0.2);"
"}"
".title {"
"  font-size: 24px;"
"  margin-bottom: 30px;"
"  opacity: 0.9;"
"}"
".value-box {"
"  font-size: 100px;"
"  font-weight: bold;"
"  font-family: 'Courier New', monospace;"
"  margin: 20px 0;"
"  text-shadow: 0 0 20px rgba(255,255,255,0.5);"
"}"
".tip {"
"  font-size: 16px;"
"  opacity: 0.8;"
"  margin-top: 20px;"
"}"
"</style></head><body>"
"<div class='dashboard'>"
"<div class='title'>📊 触摸传感器实时监测</div>"
"<div class='value-box' id='value'>--</div>"
"<div class='tip'>手指靠近引脚 → 数值变小 | 手指离开 → 数值恢复</div>"
"</div>"
"<script>"
"const valueEl = document.getElementById('value');"
"// 定时拉取最新数据，更新页面"
"setInterval(() => {"
"  fetch('/data')"
"    .then(res => res.text())"
"    .then(num => {"
"      valueEl.innerText = num;"
"    });"
"}, " + String(refreshInterval) + ");"
"</script></body></html>";

// 处理主页请求，返回仪表盘页面
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", htmlPage);
}

// 处理数据请求，返回实时触摸数值
void handleData() {
  int touchVal = touchRead(TOUCH_PIN);
  Serial.printf("实时触摸值：%d\n", touchVal);
  server.send(200, "text/plain", String(touchVal));
}

void setup() {
  Serial.begin(115200);

  // 连接WiFi
  Serial.printf("正在连接WiFi：%s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi连接成功");
  Serial.print("🌐 仪表盘地址：");
  Serial.println(WiFi.localIP());

  // 注册Web路由
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("✅ 实时传感器仪表盘已启动");
}

void loop() {
  server.handleClient(); // 持续处理客户端请求
}