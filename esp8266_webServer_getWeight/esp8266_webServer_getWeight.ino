/**  * Demo ：
  *    演 示 webserver html 功 能
  *    ( 当 wifi 模 块 连 接 上 ap 之 后 ， 在 pc 浏 览 器 中 输 入 ip+uri 来 访 问 )  
  * @author WZR
  * @date 2020/08/19 
*/   

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef Debug     //打开Debug输出
//#ifdef Debug    //关闭Debug输出
#define DebugPrintln Serial.println
#define DebugPrint   Serial.print
#endif

#ifndef STASSID             //设置一个wifi以供esp8266连接,这里修改为自己的wifi名称及密码
#define STASSID "WZR"       //设置esp8266连接的wifi名称
#define STAPSK  "66666666"  //设置esp8266连接的wifi密码
#endif

#define led LED_BUILTIN     //定义LED指示灯

int Weight=0;
String RxString="";
const char *ssid = STASSID;
const char *password = STAPSK;

// 声 明 函 数
void initBasic(void);       //初 始 化 基 础 功 能 ： 波 特 率, led
void initWifi(void);        //初 始 化 wifi 模 块 ： 工 作 模 式 连 接 网 络 
void initWebServer(void);   //初 始 化 webserver  
void initmDNS(void);        //初 始 化 mDNS  服务

ESP8266WebServer server(80);//初始化web服务器，端口为80

void setup(void) {
  initBasic();   
  initWifi();   
  initWebServer();
  initmDNS();
}

void loop(void) {
  //判断串口是否有数据传来，有数据则将数据存入字符串RxString中
  while (Serial.available() != 0)  
  {
      RxString += char(Serial.read());
      delay(2);
  }
  //判断RxString中是否有数据，有则转换得到重量数据赋给Weight并清空字符串
  if (RxString.length() > 0)
  {
      Weight=atoi(RxString.c_str());
      Serial.println(RxString);
      RxString = "";
  }
  //监听客户端请求并处理
  server.handleClient();
  //mdns服务更新
  MDNS.update();
}

/**  
 * 初 始 化 基 础 功 能 ： 波特率,led
 */ 
void initBasic(){   
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  Serial.begin(115200);
}

/**  
 *   * 初 始 化 wifi 模 块 ： 工 作 模 式 连 接 网 络 
 */ 
void initWifi(){   
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  DebugPrintln("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(led, !digitalRead(led));
    DebugPrint(".");
  }
  digitalWrite(led, LOW);
  DebugPrintln("");
  DebugPrint("Connected to ");
  DebugPrintln(ssid);
  DebugPrint("IP address: ");
  DebugPrintln(WiFi.localIP());
}

/**  
  * 初 始 化 webserver  
  */ 
void initWebServer(){ 
  // 以 下 配 置 uri 对 应 的 handler 
  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  // 启 动 webserver   
  server.begin();
  DebugPrintln("HTTP server started");

  DebugPrint("Open http://");   
  DebugPrint(WiFi.localIP());   
  DebugPrintln("/ in your browser to see it working"); 
}
/**  
  * 初 始 化 mDNS  
  */ 
void initmDNS(){   
  while(!MDNS.begin("esp8266")) {     
    DebugPrintln("Error setting up MDNS responder!");        
    delay(1000);     
  }   
  DebugPrintln("mDNS responder started,please input http://esp8266/ in your browser after install Bonjour"); 
} 

/**  * 处 理 根 目 录 uri 请 求
  * uri:http://server_ip/  
*/
void handleRoot() {  
  char temp[400];
  snprintf(temp, 400,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='2'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <h1>ESP8266 WiFi Electronic Scale:</h1>\
    <h1>Weight is %4d g</h1>\
    <p>Weight=(0,5Kg)</p>\
  </body>\
</html>",

           Weight
          );
  server.send(200, "text/html", temp);
}


/**  
 * 处 理 无 效 uri  
 * uri:http://server_ip/xxxx  
*/
void handleNotFound() {
  // 打 印 无 效 uri 的 信 息,包 括 请 求 方 式,请 求 参 数
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
