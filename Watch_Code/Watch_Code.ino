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

#include "WeatherStationImages.h"
#include "font.h"

// WIFI账号和密码
const char* WIFI_SSID = "CMCC-9Nkm";
const char* WIFI_PWD = "Pm54j#Pm";

//访问心知天气的网站，获取天气数据
WiFiClient client;//创建网络对象
const int httpPort = 80;
const char* host = "api.seniverse.com";
int WeatherFlag=0;
//定义当日天气变量
char *location_name = "Lanzhou";
char *now_temperature = "24";

char *forcast_code1 = "37";
char *forcast_temperaturerange1 ="20/-15";

char *forcast_code2 = "38";
char *forcast_temperaturerange2 ="20/-16";

char *forcast_code3 = "99";
char *forcast_temperaturerange3 ="20/-17";

// 网络时间
// initial time (possibly given by an external RTC)
#define RTC_UTC_TEST 1510592825 // 1510592825 = Monday 13 November 2017 17:07:05 UTC
#define MYTZ TZ_Asia_Shanghai

// Adjust according to your language
const String WDAY_NAMES[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const String MONTH_NAMES[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

static timeval tv;
static time_t now;

static esp8266::polledTimeout::periodicMs showTimeNow(1000);
static int time_machine_days = 0; // 0 = now
static bool time_machine_running = false;


// 显示屏设置
const int I2C_DISPLAY_ADDRESS = 0x3c;
#if defined(ESP8266)
const int SDA_PIN = D1;
const int SDC_PIN = D2;
#endif


//Key Settings
#define SET_KEY 1
#define UP_KEY  2
#define DOWN_KEY 3


//初始化OLED管脚 地址为0x3c
// sda-pin=GPIO5(D1) and sdc-pin=GPIO4(D2)
SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
OLEDDisplayUi   ui( &display );


//数据更新条
void drawProgress(OLEDDisplay *display, int percentage, String label);
//初始数据获取
void updateData(OLEDDisplay *display);
//时间日期获取
void time_is_set_scheduled(void);
//按键函数
void doKeysFunction(void);
int getKeys(void);
//获取当天气数据
void GetCurrentWeather(void);
//获取未来三天的天气
void GetForecastWeather(void);
void Gui_DrawFont_GBK16(uint8_t x, uint8_t y, char *s);

//五个界面框架
//主界面框架
void draw_MeunFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void draw_ClockFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void draw_WeatherFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void draw_MqttFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void draw_SetFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);

// Add frames
// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
FrameCallback frames[] = { draw_MeunFram,draw_ClockFram,draw_WeatherFram,draw_MqttFram,draw_SetFram};
int numberOfFrames = 5;

OverlayCallback overlays[] = { drawHeaderOverlay };
int numberOfOverlays = 1;



void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  
  // setup RTC time   
  // it will be used until NTP server will send us real current time
  time_t rtc = RTC_UTC_TEST;
  timeval tv = { rtc, 0 };
  timezone tz = { 0, 0 };
  settimeofday(&tv, &tz);
  settimeofday_cb(time_is_set_scheduled);
  configTime(MYTZ, "pool.ntp.org");
  //sntp_servermode_dhcp(0);//关闭

  
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
  display->drawString(44 + x, 5 + y, String(buff));
  display->setFont(ArialMT_Plain_24);

  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  display->drawString(44 + x, 15 + y, String(buff));
  display->setTextAlignment(TEXT_ALIGN_LEFT);

  display->drawXbm(98, 0, 10, 8,WIFI); 
  display->drawXbm(112, 0, 16, 9,Power[0]); 

  String temperature = String(now_temperature);
  int temp=temperature.toInt();
  if (temp<0){
    temp=-temp;
    if(temp>0 && temp<10) display->drawXbm(100, 12, 10, 40, Figure[10]);
    if(temp>=10)          display->drawXbm(90, 12, 10, 40, Figure[10]);
    }
  if(temp>0 && temp<10)
      display->drawXbm(110, 12, 10, 40, Figure[temp]); 
    else if(temp>=10){
      display->drawXbm(100, 12, 10, 40,Figure[temp/10]);
      display->drawXbm(110, 12, 10, 40, Figure[temp%10]);
    }
    display->drawXbm(120, 14, 8, 8, thermometer);
 
  
}

//事务闹钟界面
void draw_ClockFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(0, 8, Icon_width, Icon_height, Clock_Icon_bits);//闹钟图标
  //设置三个事务闹钟
  
}
//天气告知界面
void draw_WeatherFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  int WeatherCode=0;//默认为未知
  String temp="";
  
  display->setColor(WHITE);
  display->setFont(DialogInput_bold_12);//设置字体
  if(WeatherFlag==0){
    temp = String(forcast_code1);
    display->drawXbm(40, 0, 16, 16, hz16[0]);//今
    display->drawString(110, 38, String(forcast_temperaturerange1));
  }
      
  else if(WeatherFlag==1){
    temp = String(forcast_code2);
    display->drawXbm(40, 0, 16, 16, hz16[1]);//明
    display->drawString(110, 38, String(forcast_temperaturerange2));
    
  }
      
  else if(WeatherFlag==2){
     temp = String(forcast_code3);
     display->drawXbm(40, 0, 16, 16, hz16[2]);//后
     display->drawString(110, 38, String(forcast_temperaturerange3));
    
  }
     
  //显示预报的日期、天气情况、温度范围
  display->drawXbm(56, 0, 16, 16, hz16[3]);//天
  display->drawString(120, 2, String(location_name));
 
  display->drawXbm(40, 36, 16, 16, hz16[4]);//温
  display->drawXbm(56, 36, 16, 16, hz16[5]);//度
  
  //display->drawXbm(55, 30, 10, 20, thermometer_Icon_bits); //显示温度计计图标
 
      
  WeatherCode = temp.toInt();//转成成整形
  switch(WeatherCode){
    case 0:
        display->drawString(100,18,"(Sunny)");
        display->drawXbm(100, 18, 16, 16, hz16[6]);//晴
        display->drawXbm(0, 6, Icon_width, Icon_height, Sunny_Icon_bits);
    break;
    
    case 1:
        display->drawString(100,18,"(Clear)");
        display->drawXbm(100, 18, 16, 16, hz16[6]);//晴
        display->drawXbm(0, 6, Icon_width, Icon_height, Sunny_Icon_bits);
    break;
    
    case 2:
    case 3:
        display->drawString(100,18,"(Fair)");
        display->drawXbm(100, 18, 16, 16, hz16[6]);//晴
        display->drawXbm(0, 6, Icon_width, Icon_height, Sunny_Icon_bits);
        break;
    
    case 4: 
        display->drawXbm(56, 18, 16, 16, hz16[8]);//多
        display->drawXbm(72, 18, 16, 16, hz16[9]);//云
        display->drawXbm(0, 6, Icon_width, Icon_height, Partly_Cloudy_Icon_bits);
        break; 
        
        
    case 5: case 6:
      display->drawXbm(50, 18, 16, 16, hz16[6]);//晴
      display->drawXbm(66, 18, 16, 16, hz16[7]);//间
      display->drawXbm(82, 18, 16, 16, hz16[8]);//多
      display->drawXbm(98, 18, 16, 16, hz16[9]);//云
      display->drawXbm(0, 6, Icon_width, Icon_height, Partly_Cloudy_Icon_bits);
    break;
    
    case 7:
    case 8:
      display->drawXbm(50, 18, 16, 16, hz16[10]);//大
      display->drawXbm(66, 18, 16, 16, hz16[13]);//部
      display->drawXbm(82, 18, 16, 16, hz16[8]);//多
      display->drawXbm(98, 18, 16, 16, hz16[9]);//云
      display->drawXbm(0, 6, Icon_width, Icon_height, Partly_Cloudy_Icon_bits);
    break;
    
    case 9:
      display->drawString(110,18,"(Overcast)");
      display->drawXbm(110, 18, 16, 16, hz16[14]);//阴
      display->drawXbm(0, 6, Icon_width, Icon_height, Overcast_Icon_bits); 
      break;

    case 10: 
      display->drawXbm(56, 18, 16, 16, hz16[16]);//阵
      display->drawXbm(72, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(0, 6, Icon_width, Icon_height, Rain_Icon_bits);
    break;

    
    case 11: 
      display->drawXbm(60, 18, 16, 16, hz16[15]);//雷
      display->drawXbm(76, 18, 16, 16, hz16[16]);//阵
      display->drawXbm(92, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(0, 6, Icon_width, Icon_height, Thundershower_Icon_bits);
    break;
    
    case 12: 
      display->drawXbm(40, 18, 16, 16, hz16[15]);//雷
      display->drawXbm(54, 18, 16, 16, hz16[16]);//阵
      display->drawXbm(68, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(82, 18, 16, 16, hz16[18]);//加
      display->drawXbm(96, 18, 16, 16, hz16[19]);//冰
      display->drawXbm(110, 18, 16, 16, hz16[20]);//雹
      display->drawXbm(0, 6, Icon_width, Icon_height, Hail_Icon_bits); 
    break;
    
    case 13:
      display->drawXbm(56, 18, 16, 16, hz16[12]);//小
      display->drawXbm(72, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(0, 6, Icon_width, Icon_height, Rain_Icon_bits); 
    break;
    
    case 14:  
      display->drawXbm(56, 18, 16, 16, hz16[11]);//中
      display->drawXbm(72, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(0, 6, Icon_width, Icon_height, Rain_Icon_bits); 
    break;

    
    case 15: 
      display->drawXbm(56, 18, 16, 16, hz16[10]);//大
      display->drawXbm(72, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(0, 6, Icon_width, Icon_height, Rain_Icon_bits); 
    break;

    
    case 16: 
      display->drawXbm(56, 18, 16, 16, hz16[33]);//暴
      display->drawXbm(72, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(0, 6, Icon_width, Icon_height, Rain_Icon_bits); 
    break;

    
    case 17:
      display->drawXbm(60, 18, 16, 16, hz16[10]);//大
      display->drawXbm(76, 18, 16, 16, hz16[33]);//暴
      display->drawXbm(92, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(0, 6, Icon_width, Icon_height, Rain_Icon_bits); 
    break;

    
    case 18:       
      display->drawXbm(50, 18, 16, 16, hz16[41]);//特
      display->drawXbm(60, 18, 16, 16, hz16[10]);//大
      display->drawXbm(76, 18, 16, 16, hz16[33]);//暴
      display->drawXbm(92, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(0, 6, Icon_width, Icon_height, Rain_Icon_bits); 
    break;


    
    case 19:
      display->drawXbm(56, 18, 16, 16, hz16[34]);//冻
      display->drawXbm(72, 18, 16, 16, hz16[17]);//雨 
      display->drawXbm(0, 6, Icon_width, Icon_height, Rain_Icon_bits); 
    break;
    
    case 20:
      display->drawXbm(60, 18, 16, 16, hz16[17]);//雨
      display->drawXbm(76, 18, 16, 16, hz16[35]);//夹
      display->drawXbm(92, 18, 16, 16, hz16[36]);//雪 
      display->drawXbm(0, 6, Icon_width, Icon_height, Sleet_Icon_bits); 
    break;
    
    case 21: 
      display->drawXbm(56, 18, 16, 16, hz16[16]);//阵
      display->drawXbm(72, 18, 16, 16, hz16[36]);//雪 
      display->drawXbm(0, 6, Icon_width, Icon_height, Snow_Icon_bits);
    break;

    
    case 22: 
      display->drawXbm(56, 18, 16, 16, hz16[12]);//小
      display->drawXbm(72, 18, 16, 16, hz16[36]);//雪 
      display->drawXbm(0, 6, Icon_width, Icon_height, Snow_Icon_bits); 
    break;
    
    case 23:
      display->drawXbm(56, 18, 16, 16, hz16[11]);//中
      display->drawXbm(72, 18, 16, 16, hz16[36]);//雪 
      display->drawXbm(0, 6, Icon_width, Icon_height, Snow_Icon_bits); 
    break;
    
    case 24:
      display->drawXbm(56, 18, 16, 16, hz16[10]);//大
      display->drawXbm(72, 18, 16, 16, hz16[36]);//雪 
      display->drawXbm(0, 6, Icon_width, Icon_height, Snow_Icon_bits); 
    break;
    
    case 25: 
      display->drawXbm(56, 18, 16, 16, hz16[33]);//暴
      display->drawXbm(72, 18, 16, 16, hz16[36]);//雪 
      display->drawXbm(0, 6, Icon_width, Icon_height, Snow_Icon_bits); 
    break;
    
    case 26:
      display->drawXbm(56, 18, 16, 16, hz16[21]);//浮
      display->drawXbm(72, 18, 16, 16, hz16[22]);//尘
      display->drawXbm(0, 6, Icon_width, Icon_height, Duststorm_Icon_bits); 
    break;


    
    case 27:
      display->drawXbm(56, 18, 16, 16, hz16[23]);//扬
      display->drawXbm(72, 18, 16, 16, hz16[24]);//沙
      display->drawXbm(0, 6, Icon_width, Icon_height, Duststorm_Icon_bits); 
    break;

    
    case 28:
      display->drawXbm(60, 18, 16, 16, hz16[24]);//沙
      display->drawXbm(76, 18, 16, 16, hz16[22]);//尘
      display->drawXbm(92, 18, 16, 16, hz16[33]);//暴
      display->drawXbm(0, 6, Icon_width, Icon_height, Duststorm_Icon_bits); 
    break;
    
    case 29: 
      display->drawXbm(50, 18, 16, 16, hz16[25]);//强
      display->drawXbm(60, 18, 16, 16, hz16[24]);//沙
      display->drawXbm(76, 18, 16, 16, hz16[22]);//尘
      display->drawXbm(92, 18, 16, 16, hz16[33]);//暴
      display->drawXbm(0, 6, Icon_width, Icon_height, Duststorm_Icon_bits); 
    break;
    
    case 30:
      display->drawString(100,18,"(Foggy)");
      display->drawXbm(100, 18, 16, 16, hz16[28]);//雾
      display->drawXbm(0, 6, Icon_width, Icon_height, Haze_Icon_bits);
    break;
    
    case 31:
      display->drawString(100,18,"(Haze)");
      display->drawXbm(100, 18, 16, 16, hz16[29]);//霾
      display->drawXbm(0, 6, Icon_width, Icon_height, Haze_Icon_bits);
    break;
    
    case 32:
      display->drawString(100,18,"(Windy)");
      display->drawXbm(100, 18, 16, 16, hz16[27]);//风
      display->drawXbm(0, 6, Icon_width, Icon_height, Haze_Icon_bits);
    break;
    
    case 33:
      display->drawXbm(56, 18, 16, 16, hz16[10]);//大
      display->drawXbm(72, 18, 16, 16, hz16[27]);//风
      display->drawXbm(0, 6, Icon_width, Icon_height, Haze_Icon_bits);
    break;
    
    case 34:
      display->drawXbm(56, 18, 16, 16, hz16[30]);//飓
      display->drawXbm(72, 18, 16, 16, hz16[27]);//风
      display->drawXbm(0, 6, Icon_width, Icon_height, Haze_Icon_bits);
    break;
    
    case 35:
      display->drawXbm(50, 18, 16, 16, hz16[38]);//热
      display->drawXbm(60, 18, 16, 16, hz16[26]);//带
      display->drawXbm(76, 18, 16, 16, hz16[27]);//风
      display->drawXbm(92, 18, 16, 16, hz16[33]);//暴
      display->drawXbm(0, 6, Icon_width, Icon_height, Haze_Icon_bits);
    break;
    
    case 36:
      display->drawXbm(60, 18, 16, 16, hz16[31]);//龙
      display->drawXbm(76, 18, 16, 16, hz16[32]);//卷
      display->drawXbm(92, 18, 16, 16, hz16[27]);//风
      display->drawXbm(0, 6, Icon_width, Icon_height, Haze_Icon_bits);
    break;
    
    case 37:
      display->drawString(100,18,"(Cold)");
      display->drawXbm(100, 18, 16, 16, hz16[37]);//冷
      display->drawXbm(0, 6, Icon_width, Icon_height, Cold_Icon_bits);
    break;
    
    case 38:
      display->drawString(100,18,"(Hot)");
      display->drawXbm(100, 18, 16, 16, hz16[38]);//热
      display->drawXbm(0, 6, Icon_width, Icon_height, Sunny_Icon_bits);
    break;
    
    case 99: 
      display->drawXbm(0, 6, Icon_width, Icon_height, Unkonwn_Icon_bits);
      display->drawXbm(56, 18, 16, 16, hz16[39]);//未
      display->drawXbm(72, 18, 16, 16, hz16[40]);//知
      break;
  }
  display->drawXbm(112, 36, 16, 16, Temperature_Icon_bits);
  
}


void draw_SetFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(0, 8, Icon_width, Icon_height, Set_Icon_bits);//闹钟图标
}


void draw_MqttFram(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y){
  display->drawXbm(0, 8, Icon_width, Icon_height, Home_Icon_bits);//闹钟图标


  
}

//底层界面，包括显示时间，当天气温
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
  
  String mtemp = String("100")  +  "%";
  display->drawString(128, 54, mtemp);
  
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
//获取当前天气
void GetCurrentWeather(void)  
{
    //向心知天气的服务器发送请求。
    //连接服务器并判断是否连接成功，若成功就发送GET 请求数据下发 
    String json_from_server; 
    if(client.connect(host, httpPort)==1)                 
    {     
        //主要格式                                        
        client.print("GET /v3/weather/now.json?key=S0z2fnApuw-q9soOI&location=Lanzhou&language=en&unit=c HTTP/1.1\r\nHost:api.seniverse.com\r\n\r\n"); //心知天气的URL格式          
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
        delay(1000);                                            //请求失败等5秒
    } 
                              
    //const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 210;
    //DynamicJsonDocument jsonBuffer(capacity);
    DynamicJsonDocument jsonBuffer(500);
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
    //strcpy(now_text,jsonBuffer["results"][0]["now"]["text"]);
    //strcpy(now_code,jsonBuffer["results"][0]["now"]["code"]);
    strcpy(now_temperature,jsonBuffer["results"][0]["now"]["temperature"]);
    //通过串口打印出需要的信息
    Serial.println("Today's Weather");
    Serial.print("location_name:");
    Serial.println(location_name); 
                        
    //Serial.print("text:");
    //Serial.println(now_text);
   
    //Serial.print("code:");
    //Serial.println(now_code);

    Serial.print("temperature:");
    Serial.println(now_temperature);
 
    client.stop();     //关闭HTTP客户端，采用HTTP短链接，数据请求完毕后要客户端要主动断开   
}


//获取未来三天的天气信息
void GetForecastWeather(void)  
{
    String json_from_server; 
    if(client.connect(host, httpPort)==1)                 
    {     
        client.print("GET /v3/weather/daily.json?key=cinm0okk7gzgtujn&location=lanzhou&language=en&unit=c&start=0&days=4 HTTP/1.1\r\nHost: api.seniverse.com\r\n\r\n"); //心知天气的URL格式          
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
        delay(1000);                                            //请求失败等5秒
    } 
    //1600字节是因为https://arduinojson.org/v6/api/json/deserializejson/                        
    DynamicJsonDocument jsonBuffer(2000);
    //将天气数据放入jsonBuffer
    DeserializationError error = deserializeJson(jsonBuffer, json_from_server);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    //以下代码分别提取地区，当前天气，代码，及湿度
    
      char *Localtemp="000";//临时变量    
      //strcpy(forcast_date1,jsonBuffer["results"][0]["daily"][0]["date"]); //明天的日期
      //Serial.println("forcast_date1:");
      //Serial.println(forcast_date1);
      //strcpy(forcast_text1,jsonBuffer["results"][0]["daily"][0]["text_day"]); //明天天气现象
      //Serial.println("forcast_text1:");
      //Serial.println(forcast_text1);
      strcpy(forcast_code1,jsonBuffer["results"][0]["daily"][0]["code_day"]); //明天天气代码
      Serial.println("forcast_code1:");
      Serial.println(forcast_code1);
      strcpy(forcast_temperaturerange1,jsonBuffer["results"][0]["daily"][0]["high"]); //明天最高温度
      strcpy(Localtemp,jsonBuffer["results"][0]["daily"][0]["low"]); //明天最低温度
      strcat(forcast_temperaturerange1,"/"); //连接两个字符串
      strcat(forcast_temperaturerange1,Localtemp);
      Serial.println("forcast_temperaturerange1:");
      Serial.println(forcast_temperaturerange1);

      //strcpy(forcast_date2,jsonBuffer["results"][0]["daily"][1]["date"]); //明天的日期
      //Serial.println("forcast_date2:");
      //Serial.println(forcast_date2);
      //strcpy(forcast_text2,jsonBuffer["results"][0]["daily"][1]["text_day"]); //明天天气现象
      //Serial.println("forcast_text2:");
      //Serial.println(forcast_text2);
      strcpy(forcast_code2,jsonBuffer["results"][0]["daily"][1]["code_day"]); //明天天气代码
      Serial.println("forcast_code2:");
      Serial.println(forcast_code2);
      strcpy(forcast_temperaturerange2,jsonBuffer["results"][0]["daily"][1]["high"]); //明天最高温度
      strcpy(Localtemp,jsonBuffer["results"][0]["daily"][1]["low"]); //明天最低温度
      strcat(forcast_temperaturerange2,"/"); //连接两个字符串
      strcat(forcast_temperaturerange2,Localtemp);
      Serial.println("forcast_temperaturerange2:");
      Serial.println(forcast_temperaturerange2);


      //strcpy(forcast_date3,jsonBuffer["results"][0]["daily"][2]["date"]); //明天的日期
      //Serial.println("forcast_date3:");
      //Serial.println(forcast_date3);
      //strcpy(forcast_text3,jsonBuffer["results"][0]["daily"][2]["text_day"]); //明天天气现象
      //Serial.println("forcast_text3:");
      //Serial.println(forcast_text3);
      strcpy(forcast_code3,jsonBuffer["results"][0]["daily"][2]["code_day"]); //明天天气代码
      Serial.println("forcast_code3:");
      Serial.println(forcast_code3);
      strcpy(forcast_temperaturerange3,jsonBuffer["results"][0]["daily"][2]["high"]); //明天最高温度
      strcpy(Localtemp,jsonBuffer["results"][0]["daily"][2]["low"]); //明天最低温度
      strcat(forcast_temperaturerange3,"/"); //连接两个字符串
      strcat(forcast_temperaturerange3,Localtemp);
      Serial.println("forcast_temperaturerange3:");
      Serial.println(forcast_temperaturerange3);
    
      client.stop();     //关闭HTTP客户端，采用HTTP短链接，数据请求完毕后要客户端要主动断开   
    
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

void updateData(OLEDDisplay *display) {
  drawProgress(display, 10, "Updating time...");
  //GetCurrentWeather();//获取当天天气
  drawProgress(display, 30, "Updating weather...");
  //GetForecastWeather();//获取未来三天的天气
  
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
    if(uiFrameIndex == 5)
        uiFrameIndex = 0;     
    ui.switchToFrame(uiFrameIndex);
  }
  if(keys == UP_KEY){
    if(uiFrameIndex==2){
        WeatherFlag++;
        if(WeatherFlag==3)
            WeatherFlag=0;  
      }
    
  }
  if(keys == DOWN_KEY){
    if(uiFrameIndex==2){
        WeatherFlag--; 
        if(WeatherFlag<0)
            WeatherFlag=2;     
      }
  }
}
