// ex08 物联网安防报警器 - 最终版（带触摸防抖 + 状态保护）
#include <WiFi.h>
#include <WebServer.h>

#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 20      // 触摸触发阈值
#define DEBOUNCE_MS 80    // 触摸防抖时长，防止误触发

const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);

bool isArmed = false;       // 系统布防状态
bool isAlarming = false;    // 报警触发状态
bool ledState = false;      // LED闪烁状态
unsigned long lastBlinkTime = 0;
const int blinkInterval = 100; // 报警闪烁间隔（毫秒）

// 触摸防抖变量
bool lastStableTouch = false;
unsigned long lastDebounceTime = 0;

// 优化后的网页界面
const String htmlPage = 
"<!DOCTYPE html><html lang='zh-CN'><head>"
"<meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>安防报警主机</title>"
"<style>"
"body{font-family:微软雅黑,sans-serif;text-align:center;margin-top:12vh;background:#f0f2f5;}"
".card{background:white;padding:50px 40px;border-radius:16px;display:inline-block;box-shadow:0 4px 20px rgba(0,0,0,0.1);}"
"h2{color:#333;margin-bottom:30px;}"
".status{font-size:22px;margin-bottom:30px;font-weight:bold;}"
".status.armed{color:#e74c3c;}"
".status.disarmed{color:#27ae60;}"
"button{padding:16px 50px;font-size:18px;border:none;border-radius:8px;cursor:pointer;margin:0 10px;color:white;}"
".btn-arm{background:#e74c3c;}"
".btn-disarm{background:#27ae60;}"
"button:hover{opacity:0.9;}"
"</style></head><body>"
"<div class='card'>"
"<h2>🏠 安防报警控制中心</h2>"
"<div id='status' class='status disarmed'>当前状态：已撤防</div>"
"<button class='btn-arm' onclick='arm()'>布防</button>"
"<button class='btn-disarm' onclick='disarm()'>撤防</button>"
"</div>"
"<script>"
"const statusEl = document.getElementById('status');"
"function arm(){"
"  fetch('/arm').then(res=>res.text()).then(()=>{"
"    statusEl.innerText='当前状态：已布防';"
"    statusEl.className='status armed';"
"  });"
"}"
"function disarm(){"
"  fetch('/disarm').then(res=>res.text()).then(()=>{"
"    statusEl.innerText='当前状态：已撤防';"
"    statusEl.className='status disarmed';"
"  });"
"}"
"</script></body></html>";

// 返回控制主页
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", htmlPage);
}

// 布防处理
void handleArm() {
  if (!isArmed) {
    isArmed = true;
    isAlarming = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("[系统] 进入布防状态");
  }
  server.send(200, "text/plain", "OK");
}

// 撤防处理
void handleDisarm() {
  isArmed = false;
  isAlarming = false;
  digitalWrite(LED_PIN, LOW);
  Serial.println("[系统] 撤防，报警已解除");
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.printf("正在连接WiFi：%s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi连接成功");
  Serial.print("🌐 控制页面：");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
  Serial.println("✅ 安防系统启动，默认撤防状态");
}

void loop() {
  server.handleClient();
  unsigned long now = millis();

  // ========== 1. 触摸检测 + 软件防抖 ==========
  int touchVal = touchRead(TOUCH_PIN);
  bool curReading = (touchVal < THRESHOLD);

  if (curReading != lastStableTouch) {
    lastDebounceTime = now;
  }

  if (now - lastDebounceTime > DEBOUNCE_MS) {
    // 布防 + 未报警 + 稳定触摸 → 触发报警
    if (isArmed && !isAlarming && curReading == true) {
      isAlarming = true;
      Serial.println("🚨 报警触发！有人触碰检测引脚");
    }
    lastStableTouch = curReading;
  }

  // ========== 2. 报警状态：LED高频闪烁 ==========
  if (isAlarming) {
    if (now - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
}