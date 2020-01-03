#include <ESP8266WiFi.h>

//SNTP网络时间获取
#include <coredecls.h>                  // settimeofday_cb()
#include <Schedule.h>
#include <PolledTimeout.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <sntp.h>                       // sntp_servermode_dhcp()
#include <TZ.h>

//解析天气有关的库
#include <ArduinoJson.h> 

//0.96寸OLED显示屏库
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
//图片有关库
#include "WeatherStationFonts.h"
#include "WeatherStationImages.h"

/***************************
 * Begin Settings
 **************************/
// WIFI
const char* WIFI_SSID = "MERCURY_ADAE";
const char* WIFI_PWD = "5477271..";

//天气
//访问心知天气的网站，获取天气数据
WiFiClient client;//创建网络对象
const int httpPort = 80;
const char* host = "api.seniverse.com";
//定义当日天气变量
char *location_name = "Lanzhou";
char *now_text = "Sunny";
char *now_code = "0";
char *now_temperature = "27";


// 网络时间
// initial time (possibly given by an external RTC)
#define RTC_UTC_TEST 1510592825 // 1510592825 = Monday 13 November 2017 17:07:05 UTC

// This database is autogenerated from IANA timezone database
//    https://www.iana.org/time-zones
// and can be updated on demand in this repository

// "TZ_" macros follow DST change across seasons without source code change
// check for your nearest city in TZ.h

// espressif headquarter TZ
#define MYTZ TZ_Asia_Shanghai

// Adjust according to your language
const String WDAY_NAMES[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const String MONTH_NAMES[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

static timeval tv;
static time_t now;

static esp8266::polledTimeout::periodicMs showTimeNow(1000);
static int time_machine_days = 0; // 0 = now
static bool time_machine_running = false;


// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
#if defined(ESP8266)
const int SDA_PIN = D1;
const int SDC_PIN = D2;
#endif


//Key Settings
#define SET_KEY 1
#define UP_KEY  2
#define DOWN_KEY 3
/***************************
* End Settings
**************************/
// Initialize the oled display for address 0x3c
// sda-pin=14 and sdc-pin=12
SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
OLEDDisplayUi   ui( &display );


//数据更新条
void drawProgress(OLEDDisplay *display, int percentage, String label);
//初始数据获取
void updateData(OLEDDisplay *display);
//时间日期获取
void getCurrentTime(void);
void time_is_set_scheduled(void);
//按键函数
void doKeysFunction(void);
int getKeys(void);
//获取当天气数据
void GetCurrentWeather(void);

//五个界面框架
//主界面框架
void draw_MeunFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void draw_ClockFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void draw_WeatherFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void draw_SysSetFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);

// Add frames
// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
FrameCallback frames[] = { draw_MeunFram,draw_ClockFram,draw_WeatherFram,draw_SysSetFram};
int numberOfFrames = 4;

OverlayCallback overlays[] = { drawHeaderOverlay };
int numberOfOverlays = 1;



void setup() {
  pinMode(D0, OUTPUT);
  
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  
  // initialize dispaly
  display.init();
  display.clear();
  display.display();
  //设置为垂直显示模式
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(255);

  pinMode(D0,OUTPUT);//LED灯管脚
  pinMode(D3,INPUT); //Back按键
  pinMode(D5,INPUT);//Down按键
  pinMode(D6,INPUT);//Up按键
  pinMode(D7,INPUT);//Set按键
  
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, LOT_Watch_Logo_bits);
  display.display();
  digitalWrite(D0, LOW); // 亮灯
  delay(200);           // 延时500ms
  digitalWrite(D0, HIGH);// 灭灯
  delay(200);          // 延时500ms
  digitalWrite(D0, LOW); // 亮灯
  delay(200);           // 延时500ms
  digitalWrite(D0, HIGH);// 灭灯
  delay(2000);
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.clear();
    display.drawString(64, 10, "Connecting to WiFi");
    display.drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbole : inactiveSymbole);
    display.drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbole : inactiveSymbole);
    display.drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbole : inactiveSymbole);
    display.display();
    counter++;
  }
  ui.setTargetFPS(30);
  ui.setActiveSymbol(activeSymbole);
  ui.setInactiveSymbol(inactiveSymbole);

  ui.setIndicatorPosition(BOTTOM);//设置指示圈位置，该出为底部，还有TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorDirection(LEFT_RIGHT);//指示圈方向模式
  ui.setFrameAnimation(SLIDE_LEFT);//滑动模式，这里为左滑动模式，还有SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrames(frames, numberOfFrames);
  ui.setOverlays(overlays, numberOfOverlays);
  ui.disableAutoTransition();//取消自动模式
  ui.switchToFrame(0);//选择第一个开机画面

  ui.init();  //Inital UI takes care of initalising the display too.
  display.flipScreenVertically();
  Serial.println("");
  Serial.println("");
  updateData(&display);
}

void loop() {
    int remainingTimeBudget = ui.update();
        if (remainingTimeBudget > 0) {
          // You can do some work here
          // Don't do stuff if you are below your
          // time budget.
          delay(remainingTimeBudget);
    }
    doKeysFunction();
}

//主界面
//主要显示日期、时间、天气
void draw_MeunFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[16];

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  String date = WDAY_NAMES[timeInfo->tm_wday];

  sprintf_P(buff, PSTR("%s, %02d/%02d/%04d"), WDAY_NAMES[timeInfo->tm_wday].c_str(), timeInfo->tm_mday, timeInfo->tm_mon+1, timeInfo->tm_year + 1900);
  display->drawString(64 + x, 5 + y, String(buff));
  display->setFont(ArialMT_Plain_24);

  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  display->drawString(64 + x, 15 + y, String(buff));
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}


void draw_ClockFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
 
}

void draw_WeatherFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
 
}

void draw_SysSetFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
 
}


void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);

  display->setColor(WHITE);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0, 54, String(buff));
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  String temp = String(now_temperature)  +  "°C";
  display->drawString(128, 54, temp);
  display->drawHorizontalLine(0, 52, 128);
}


// Json数据解析并串口打印

/*  请求的Json数据格式如下：
 * {
 *    "results": [
 *        {
 *            "location": {
 *                "id": "WX4FBXXFKE4F",
 *                "name": "北京",
 *                "country": "CN",
 *                "path": "北京,北京,中国",
 *                "timezone": "Asia/Shanghai",
 *                "timezone_offset": "+08:00"
 *            },
 *            "now": {
 *                "text": "多云",
 *                "code": "4",
 *                "temperature": "23"
 *            },
 *            "last_update": "2019-10-13T09:51:00+08:00"
 *        }
 *    ]
 *}
 */

void GetCurrentWeather(void)  
{
    //向心知天气的服务器发送请求。
    //连接服务器并判断是否连接成功，若成功就发送GET 请求数据下发 
    String json_from_server; 
    if(client.connect(host, httpPort)==1)                 
    {     
        //主要格式                                        
        client.print("GET /v3/weather/now.json?key=smtq3n0ixdggurox&location=Lanzhou&language=en&unit=c HTTP/1.1\r\nHost:api.seniverse.com\r\n\r\n"); //心知天气的URL格式          
        String status_code = client.readStringUntil('\r');        //读取GET数据，服务器返回的状态码，若成功则返回状态码200
        Serial.println(status_code);
        if(client.find("\r\n\r\n")==1)                            //跳过返回的数据头，直接读取后面的JSON数据，
        {
          json_from_server=client.readStringUntil('\n');  //读取返回的JSON数据
          Serial.println(json_from_server);
        }
    }
    else                                        
    { 
        Serial.println("connection failed this time");
        delay(5000);                                            //请求失败等5秒
    } 
                              
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 210;
    DynamicJsonDocument jsonBuffer(capacity);
    // Parse JSON object
    //将天气数据放入jsonBuffer
    DeserializationError error = deserializeJson(jsonBuffer, json_from_server);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    //以下代码分别提取地区，当前天气，代码，及湿度
    Serial.println("Start Decode:");
    strcpy(location_name,jsonBuffer["results"][0]["location"]["name"]);
    strcpy(now_text,jsonBuffer["results"][0]["now"]["text"]);
    strcpy(now_code,jsonBuffer["results"][0]["now"]["code"]);
    strcpy(now_temperature,jsonBuffer["results"][0]["now"]["temperature"]);
     //通过串口打印出需要的信息
    Serial.println(location_name);                      
    Serial.println(now_text);
    Serial.println(now_code);
    Serial.println(now_temperature);
    Serial.print("\r\n");
    client.stop();     //关闭HTTP客户端，采用HTTP短链接，数据请求完毕后要客户端要主动断开   
}

void GetForestWeather(void)  
{
    
   
}



void drawProgress(OLEDDisplay *display, int percentage, String label) {
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  //设置字体
  display->setFont(ArialMT_Plain_10);
  display->drawString(64, 10, label);
  display->drawProgressBar(2, 28, 124, 10, percentage);
  display->display();
}


void getCurrentTime(void){
    // setup RTC time   
  // it will be used until NTP server will send us real current time
  time_t rtc = RTC_UTC_TEST;
  timeval tv = { rtc, 0 };
  timezone tz = { 0, 0 };
  settimeofday(&tv, &tz);
  // install callback - called when settimeofday is called (by SNTP or us)
  // once enabled (by DHCP), SNTP is updated every hour
  settimeofday_cb(time_is_set_scheduled);
  // NTP servers may be overriden by your DHCP server for a more local one
  // (see below)
  configTime(MYTZ, "pool.ntp.org");
  delay(5000);//等待获取完成
  // OPTIONAL: disable obtaining SNTP servers from DHCP
  //sntp_servermode_dhcp(0); // 0: disable obtaining SNTP servers from DHCP (enabled by default)
}

void updateData(OLEDDisplay *display) {
  drawProgress(display, 10, "Updating time...");
  getCurrentTime();

  drawProgress(display, 30, "Updating weather...");
  GetCurrentWeather();

  drawProgress(display, 50, "Updating forecasts...");


  drawProgress(display, 100, "Done...");
  
}






void time_is_set_scheduled() {
  // everything is allowed in this function

  if (time_machine_days == 0) {
    time_machine_running = !time_machine_running;
  }

  // time machine demo
  if (time_machine_running) {
    if (time_machine_days == 0)
      Serial.printf("---- settimeofday() has been called - possibly from SNTP\n"
                    "     (starting time machine demo to show libc's automatic DST handling)\n\n");
    now = time(nullptr);
    const tm* tm = localtime(&now);
    Serial.printf("future=%3ddays: DST=%s - ",
                  time_machine_days,
                  tm->tm_isdst ? "true " : "false");
    Serial.print(ctime(&now));
    gettimeofday(&tv, nullptr);
    constexpr int days = 30;
    time_machine_days += days;
    if (time_machine_days > 360) {
      tv.tv_sec -= (time_machine_days - days) * 60 * 60 * 24;
      time_machine_days = 0;
    } else {
      tv.tv_sec += days * 60 * 60 * 24;
    }
    settimeofday(&tv, nullptr);
  } else {
      gettimeofday(&tv, nullptr);
      now = time(nullptr);
      // human readable
      Serial.print("ctime:     ");
      Serial.print(ctime(&now));
  }
}

int getKeys(void){
    //Set按键按下
    int i=0;
    if(digitalRead(D7) == HIGH){
      delay(5);
      if(digitalRead(D7) == HIGH){
        while(digitalRead(D7) == HIGH){
          i++;//防止进入死循环
          if(i>=500000){
            i=0;
            break;
          }
        }
        return SET_KEY;
      }
    }
    //Up按键被按下
     if(digitalRead(D6) == LOW){
      delay(5);
      if(digitalRead(D6) == LOW){
        while(digitalRead(D6) == LOW){
          i++;//防止进入死循环
          if(i>=500000){
            i=0;
            break;
          }
        }
        return UP_KEY;
      }
    }
    //Down按键被按下
     if(digitalRead(D5) == LOW){
      delay(5);
      if(digitalRead(D5) == LOW){
        while(digitalRead(D5) == LOW){
          i++;//防止进入死循环
          if(i>=500000){
            i=0;
            break;
          }
        }
        return DOWN_KEY;
      }
    }
    return 0;
}

void doKeysFunction(void){
  static int uiFrameIndex = 0;
  int keys = getKeys();
  if(keys == SET_KEY){
    uiFrameIndex++;
    if(uiFrameIndex == 4)
        uiFrameIndex = 0;     
    ui.switchToFrame(uiFrameIndex);
  }
  if(keys == UP_KEY){
  }
  if(keys == DOWN_KEY){
    
  }
}
