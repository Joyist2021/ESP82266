/**************************************************************************
  日期：2020/11/23
  功能：自动连接WiFi，APP端获取传感器温度DHT11，天猫精灵控制/APP控制 四路继电器
  作者：冷月满天，B站ID:173951713,   https://space.bilibili.com/173951713
  调用库Blinker、ESP8266、DHT
  注：APP是用的"点灯·blinker"   点灯科技官网https://www.diandeng.tech/home
**************************************************************************/

#define BLINKER_WIFI                  //定义wifi模块
#define BLINKER_ALIGENIE_MULTI_OUTLET //天猫精灵设置为多插座模式
#define BLINKER_WITHOUT_SSL           //非SSL加密通信接入，省堆栈
#define BLINKER_PRINT Serial          //给debug打开串口
#define BLINKER_ESP_SMARTCONFIG       //定义EspTouch/SmartConfig模式进行配网，可以用APP配网或者安信可官方微信公众号里的配网小程序
#include <ESP8266WiFi.h>              //包含ESP8266头文件
#include <Blinker.h>                  //包含Blinker头文件，用于连接点灯科技
#include "DHT.h"                      //引入传感器DHT头文件
#define DHTPIN D7         // 定义温湿度传感器的引脚
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22
//#define DHTTYPE DHT21   // DHT 21

DHT dht(DHTPIN, DHTTYPE);

/***********************************************************************
  定义全局变量
************************************************************************/
char auth[] = "bc8e32da90fb";       //你的设备key，点灯·blinker里的key
float humi_read = 0, temp_read = 0; //定义浮点型全局变量 储存传感器读取的温湿度数据

#define Smart_RELAY1 D6 //继电器1
#define Smart_RELAY2 D5 //继电器2
#define Smart_RELAY3 D2 //继电器3
#define Smart_RELAY4 D1 //继电器4


// flag标识符 按键状态 0关闭off,1打开on
bool switch0_onoff = 0;
bool switch1_onoff = 0;
bool switch2_onoff = 0;
bool switch3_onoff = 0;
bool switch4_onoff = 0;

bool oState[5] = { false };

/*********************************************************************************************************
  新建数据类型组件对象，作用：将数据传输到手机blinker app
*********************************************************************************************************/
BlinkerButton Button0("switch0");
BlinkerButton Button1("switch1");
BlinkerButton Button2("switch2");
BlinkerButton Button3("switch3");
BlinkerButton Button4("switch4");
BlinkerNumber HUMI("humi");           //定义湿度数据键名
BlinkerNumber TEMP("temp");           //定义温度数据键名

void aligeniePowerState(const String & state, uint8_t num)
{
    BLINKER_LOG("need set outlet: ", num, ", power state: ", state);

    if (state == BLINKER_CMD_ON) {
        if (num == 0)
        {
          switch0_onoff = 1;
          digitalWrite(LED_BUILTIN, LOW);
          // 反馈开关状态
          //Button0.icon("fas fa-lightbulb-on");
          Button0.color("#fddb00");               
          Button0.print("on");
        }
        else if(num == 1)//打开客厅的灯
        {
          switch1_onoff = 1;
          digitalWrite(Smart_RELAY1, HIGH);
          // 反馈开关状态
          //Button1.icon("fas fa-lightbulb-on");
          Button1.color("#fddb00");               
          Button1.print("on");
        }
        else if(num == 2)//打开餐厅的灯
        {
          switch2_onoff = 1;
          digitalWrite(Smart_RELAY2, HIGH);
          // 反馈开关状态
          //Button2.icon("fas fa-lightbulb-on");
          Button2.color("#fddb00");               
          Button2.print("on");
        }
        else if(num == 3)//打开厨房的灯
        {
          switch3_onoff = 1;
          digitalWrite(Smart_RELAY3, HIGH);
          // 反馈开关状态
          //Button3.icon("fas fa-lightbulb-on");
          Button3.color("#fddb00");               
          Button3.print("on");
        }
        else if(num == 4)//打开浴室的热水器
        {
          switch4_onoff = 1;
          digitalWrite(Smart_RELAY4, HIGH);
          // 反馈开关状态
          //Button4.icon("fas fa-lightbulb-on");
          Button4.color("#fddb00");               
          Button4.print("on");
        }
        

        BlinkerAliGenie.powerState("on", num);
        BlinkerAliGenie.print();

        oState[num] = true;
    }
    else if (state == BLINKER_CMD_OFF) {
             
        if (num == 0)
        {
          switch0_onoff = 0;
          digitalWrite(LED_BUILTIN, HIGH);
          // 反馈开关状态
          //Button0.icon("fal fa-lightbulb");
          Button0.color("#565656");        
          Button0.print("off");
        }
        else if(num == 1)//关闭客厅的灯
        {
          switch1_onoff = 0;
          digitalWrite(Smart_RELAY1, LOW);
          //Button1.icon("fal fa-lightbulb");
          Button1.color("#565656");        
          Button1.print("off");
        }
        else if(num == 2)//关闭餐厅的灯
        {
          switch2_onoff = 0;
          digitalWrite(Smart_RELAY2, LOW);
          //Button2.icon("fal fa-lightbulb");
          Button2.color("#565656");        
          Button2.print("off");
        }
        else if(num == 3)//关闭厨房的灯
        {
          switch3_onoff = 0;
          digitalWrite(Smart_RELAY3, LOW);
          //Button3.icon("fal fa-lightbulb");
          Button3.color("#565656");        
          Button3.print("off");
        }
        else if(num == 4)//关闭浴室的热水器
        {
          switch4_onoff = 0;
          digitalWrite(Smart_RELAY4, LOW);
          //Button4.icon("fal fa-lightbulb");
          Button4.color("#565656");        
          Button4.print("off");
        }

        BlinkerAliGenie.powerState("off", num);
        BlinkerAliGenie.print();

        oState[num] = true;

        if (num == 0)
        {
            for (uint8_t o_num = 0; o_num < 5; o_num++)
            {
                oState[o_num] = false;
            }
        }
    }
}

void aligenieQuery(int32_t queryCode, uint8_t num)
{
    BLINKER_LOG("AliGenie Query outlet: ", num,", codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("AliGenie Query All");
            BlinkerAliGenie.powerState(oState[num] ? "on" : "off", num);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("AliGenie Query Power State");
            BlinkerAliGenie.powerState(oState[num] ? "on" : "off", num);
            BlinkerAliGenie.print();
            break;
        default :
            BlinkerAliGenie.powerState(oState[num] ? "on" : "off", num);
            BlinkerAliGenie.print();
            break;
    }
}


void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}



// 按下按键0即会执行该函数
void button0_callback(const String & state)
{
    switch0_onoff = ! switch0_onoff;  //记录开关状态

    if(switch0_onoff == 1)
    {
      
      digitalWrite(LED_BUILTIN,LOW);
      // 反馈开关状态
      //Button0.icon("fas fa-lightbulb-on");
      Button0.color("#fddb00");               
      Button0.print("on");
    }
    else
    {
      digitalWrite(LED_BUILTIN,HIGH);
      // 反馈开关状态
      //Button0.icon("fal fa-lightbulb");
      Button0.color("#565656");        
      Button0.print("off");
    }
    BLINKER_LOG("get button state: ", state);    
}

// 按下按键1即会执行该函数
void button1_callback(const String & state)
{
    switch1_onoff = ! switch1_onoff;  //记录开关状态
    if(switch1_onoff == 1)
    {
      digitalWrite(Smart_RELAY1,HIGH);
      // 反馈开关状态
      //Button1.icon("fas fa-lightbulb-on");
      Button1.color("#fddb00");               
      Button1.print("on");
    }
    else
    {
      digitalWrite(Smart_RELAY1,LOW);
      // 反馈开关状态
      //Button1.icon("fal fa-lightbulb");
      Button1.color("#565656");        
      Button1.print("off");
    }
    BLINKER_LOG("get button state: ", state);    
}
// 按下按键2即会执行该函数
void button2_callback(const String & state)
{
    switch2_onoff = ! switch2_onoff;  //记录开关状态
    if(switch2_onoff == 1)
    {
      digitalWrite(Smart_RELAY2,HIGH);
      // 反馈开关状态
      //Button2.icon("fas fa-lightbulb-on");
      Button2.color("#fddb00");               
      Button2.print("on"); 
    }
    else
    {
      digitalWrite(Smart_RELAY2,LOW);
      // 反馈开关状态
      //Button2.icon("fal fa-lightbulb");
      Button2.color("#565656");        
      Button2.print("off");
    }
    BLINKER_LOG("get button state: ", state);    
}
// 按下按键3即会执行该函数
void button3_callback(const String & state)
{
    switch3_onoff = ! switch3_onoff;  //记录开关状态
    if(switch3_onoff == 1)
    {
      digitalWrite(Smart_RELAY3,HIGH);
      // 反馈开关状态
      //Button3.icon("fas fa-lightbulb-on");
      Button3.color("#fddb00");               
      Button3.print("on"); 
    }
    else
    {
      digitalWrite(Smart_RELAY3,LOW);
      // 反馈开关状态
      //Button3.icon("fal fa-lightbulb");
      Button3.color("#565656");        
      Button3.print("off");
    }
    BLINKER_LOG("get button state: ", state);    
}
// 按下按键4即会执行该函数
void button4_callback(const String & state)
{
    switch4_onoff = ! switch4_onoff;  //记录开关状态
    if(switch4_onoff == 1)
    {
      digitalWrite(Smart_RELAY4,HIGH);
      // 反馈开关状态
      //Button4.icon("fas fa-lightbulb-on");
      Button4.color("#fddb00");               
      Button4.print("on");  
    }
    else
    {
      digitalWrite(Smart_RELAY4,LOW);
      // 反馈开关状态
      //Button4.icon("fal fa-lightbulb");
      Button4.color("#565656");        
      Button4.print("off");
    }
    BLINKER_LOG("get button state: ", state);    
}



void heartbeat()
{
    HUMI.print(humi_read);
    TEMP.print(temp_read);
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);

    pinMode(LED_BUILTIN, OUTPUT);    
    pinMode(Smart_RELAY1, OUTPUT);
    pinMode(Smart_RELAY2, OUTPUT);
    pinMode(Smart_RELAY3, OUTPUT);
    pinMode(Smart_RELAY4, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(Smart_RELAY1, LOW);
    digitalWrite(Smart_RELAY2, LOW);
    digitalWrite(Smart_RELAY3, LOW);
    digitalWrite(Smart_RELAY4, LOW);

    
    
    Button0.attach(button0_callback);
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);
    Button4.attach(button4_callback);
    
    dht.begin();
    Blinker.begin(auth);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    BlinkerAliGenie.attachPowerState(aligeniePowerState);
    BlinkerAliGenie.attachQuery(aligenieQuery);
}

void loop()
{
    Blinker.run();
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
        BLINKER_LOG("Failed to read from DHT sensor!");
    }
    else
    {
        //BLINKER_LOG("Humidity: ", h, " %");
        //BLINKER_LOG("Temperature: ", t, " *C");
        humi_read = h;
        temp_read = t;
    }

    Blinker.delay(500);
}
