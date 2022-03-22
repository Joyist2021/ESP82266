
/*

  0， 上电：开发板上电，前5秒等待配置阶段LED灯0.1s亮，0.1s灭; 5秒后1秒亮1秒灭尝试连接到sta
  
  1,  smartconfig配置
  
  前5s内，LED灯快闪（0.1s亮，0.1s灭），此时短按Flash或者key键进入smartconfig配置模式，led灯0.3秒亮0.3秒灭
  
  在这种情况下可使用微信安信可官方微信公众号里的配网小程序，wifi配置功能可实现wifi配置上网
  
  或者使用ESP-touch app软件进行配置。wifi配置功能可实现wifi配置上网。
  
  如果超过3分钟没有配置成功，模块重启
  
  2，  Webserver配置：开发板上电5s后发出ap信号，启动httpserver
  
  在httpserver启动后，开发板发出wifi，ssid为：“ESP_xxxxxx”，其中xxxxxx为芯片mac地址后六位
  
  使用笔记本或者手机连接该ssid，密码为空，然后使用浏览器设置，地址为192.168.4.1
  
  3,  开发板上LED灯常亮表示开发板已经连接上无线路由器
  
  4，OTA升级固件
  
  使用webserver可以对开发板的固件进行OTA升级。地址为：http;//xxx.xxx.xxx.xxx:8080，或者在web配置页面连接过去。
  
  5，恢复出厂设置
  
  启动后，长按Flash或者Key键超过3秒钟，恢复出厂设置
  
*/

#define DHTPIN D7         // 定义温湿度传感器的引脚

#define Smart_RELAY1 D6 //继电器1

#define Smart_RELAY2 D5 //继电器2

#define Smart_RELAY3 D2 //继电器3

#define Smart_RELAY4 D1 //继电器4








