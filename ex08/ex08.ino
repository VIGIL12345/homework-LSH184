// ex08 物联网安防报警器 - 核心功能版
#include <WiFi.h>
#include <WebServer.h>

#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 20    // 触摸触发阈值

const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);

bool isArmed = false;       // 布防状态：true=布防，false=撤防
bool isAlarming = false;    // 报警状态：true=正在报警
bool ledState = false;      // LED当前状态
unsigned long lastBlinkTime = 0;
const int blinkInterval = 100; // 报警闪烁间隔，数值越小闪得越快

// 网页HTML：布防撤防控制界面
const String htmlPage = 
"<!DOCTYPE html><html><head><meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>安防报警器</title></head><body style='text-align:center;margin-top:100px;'>"
"<h1>安防报警控制系统</h1>"
"<p id='status'>当前状态：已撤防</p>"
"<button onclick='arm()' style='padding:15px 40px;font-size:20px;margin:10px;'>布防</button>"
"<button onclick='disarm()' style='padding:15px 40px;font-size:20px;margin:10px;'>撤防</button>"
"<script>"
"const statusText = document.getElementById('status');"
"function arm(){"
"  fetch('/arm').then(res=>res.text()).then(t=>{statusText.innerText='当前状态：已布防'});"
"}"
"function disarm(){"
"  fetch('/disarm').then(res=>res.text()).then(t=>{statusText.innerText='当前状态：已撤防'});"
"}"
"</script></body></html>";

// 返回主控制页面
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", htmlPage);
}

// 处理布防请求
void handleArm() {
  isArmed = true;
  isAlarming = false;
  digitalWrite(LED_PIN, LOW);
  Serial.println("系统已布防");
  server.send(200, "text/plain", "布防成功");
}

// 处理撤防请求
void handleDisarm() {
  isArmed = false;
  isAlarming = false;
  digitalWrite(LED_PIN, LOW);
  Serial.println("系统已撤防，报警解除");
  server.send(200, "text/plain", "撤防成功");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
  Serial.println("安防系统启动，默认撤防状态");
}

void loop() {
  server.handleClient();
  unsigned long now = millis();

  // ========== 1. 触摸检测，触发报警 ==========
  int touchVal = touchRead(TOUCH_PIN);
  // 布防状态 + 未报警 + 检测到触摸 → 触发报警
  if (isArmed && !isAlarming && touchVal < THRESHOLD) {
    isAlarming = true;
    Serial.println("⚠️ 触发报警！");
  }

  // ========== 2. 报警状态下LED高频闪烁 ==========
  if (isAlarming) {
    if (now - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
}