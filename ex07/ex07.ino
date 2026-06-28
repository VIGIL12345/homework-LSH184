// ex07 Web网页无极调光器 - 最终版
#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 2

// ========== WiFi配置 ==========
const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);
int brightness = 0;

// 优化后的网页：适配移动端，样式更清晰
const String htmlPage = 
"<!DOCTYPE html><html lang='zh-CN'><head>"
"<meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>LED无极调光器</title>"
"<style>"
"body{font-family:微软雅黑,sans-serif;text-align:center;margin-top:15vh;background:#f5f5f5;}"
".container{background:white;padding:40px;border-radius:12px;display:inline-block;box-shadow:0 2px 12px rgba(0,0,0,0.1);}"
"#slider{width:320px;height:24px;cursor:pointer;}"
".val-text{font-size:24px;font-weight:bold;color:#333;margin-top:20px;}"
"</style></head><body>"
"<div class='container'>"
"<h2>LED 无极调光控制</h2>"
"<input type='range' min='0' max='255' value='0' id='slider'>"
"<div class='val-text'>当前亮度：<span id='val'>0</span> / 255</div>"
"</div>"
"<script>"
"const slider = document.getElementById('slider');"
"const valText = document.getElementById('val');"
"// 监听滑动条拖动，实时发送调光请求"
"slider.addEventListener('input', function(){"
"  const val = this.value;"
"  valText.innerText = val;"
"  fetch('/set?brightness=' + val);"
"});"
"</script></body></html>";

// 返回主控制页面
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", htmlPage);
}

// 处理亮度设置请求
void handleSetBrightness() {
  if (server.hasArg("brightness")) {
    int val = server.arg("brightness").toInt();
    // 边界保护，限制在0-255范围内
    brightness = constrain(val, 0, 255);
    analogWrite(LED_PIN, brightness);
    Serial.print("亮度已更新为：");
    Serial.println(brightness);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "参数错误");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  // 连接WiFi
  Serial.printf("正在连接WiFi：%s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi连接成功");
  Serial.print("🌐 控制页面地址：");
  Serial.println(WiFi.localIP());

  // 注册路由
  server.on("/", handleRoot);
  server.on("/set", handleSetBrightness);
  server.begin();
  Serial.println("✅ Web服务器启动完成");
}

void loop() {
  server.handleClient(); // 持续处理客户端请求
}