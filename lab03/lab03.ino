// lab03 实验三：millis非阻塞实现SOS摩斯信号 三短 三长 三短
const int ledPin = 2;

// 时序基准单位 200ms
const unsigned long UNIT = 200;
const unsigned long DOT_ON = UNIT;        // 短闪亮灯
const unsigned long DOT_OFF = UNIT;       // 短闪间隔熄灭
const unsigned long DASH_ON = 3 * UNIT;   // 长闪亮灯
const unsigned long DASH_OFF = UNIT;      // 长闪间隔熄灭
const unsigned long CHAR_INTERVAL = 3*UNIT;// S与O、O与S字母间隔
const unsigned long LOOP_PAUSE = 7*UNIT;  // 一轮SOS结束长停顿

unsigned long lastTime = 0;
int step = 0;
bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  lastTime = millis();
}

void loop() {
  unsigned long now = millis();
  unsigned long waitTime = 0;

  switch(step){
    // S：三次短闪
    case 0: ledState=1; waitTime=DOT_ON; break;
    case 1: ledState=0; waitTime=DOT_OFF; break;
    case 2: ledState=1; waitTime=DOT_ON; break;
    case 3: ledState=0; waitTime=DOT_OFF; break;
    case 4: ledState=1; waitTime=DOT_ON; break;
    case 5: ledState=0; waitTime=CHAR_INTERVAL; break;

    // O：三次长闪
    case 6: ledState=1; waitTime=DASH_ON; break;
    case 7: ledState=0; waitTime=DASH_OFF; break;
    case 8: ledState=1; waitTime=DASH_ON; break;
    case 9: ledState=0; waitTime=DASH_OFF; break;
    case 10: ledState=1; waitTime=DASH_ON; break;
    case 11: ledState=0; waitTime=CHAR_INTERVAL; break;

    // S：三次短闪
    case 12: ledState=1; waitTime=DOT_ON; break;
    case 13: ledState=0; waitTime=DOT_OFF; break;
    case 14: ledState=1; waitTime=DOT_ON; break;
    case 15: ledState=0; waitTime=DOT_OFF; break;
    case 16: ledState=1; waitTime=DOT_ON; break;
    case 17: ledState=0; waitTime=LOOP_PAUSE; break;
  }

  digitalWrite(ledPin, ledState);
  if(now - lastTime >= waitTime){
    lastTime = now;
    step++;
    if(step > 17) step = 0;
  }
}