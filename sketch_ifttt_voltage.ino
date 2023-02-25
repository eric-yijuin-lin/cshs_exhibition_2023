// TODO:
// Get Datetime: https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/

#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define RXp2 16
#define TXp2 17
#define TIME_ZONE 8

struct simpleDatetime {
  String date;
  String hour;
};

// 從 NTP 國家標準時間伺服器取得時間需要的工具
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ssid = "CSHS_T36_AP_2G"; //輸入wifi ssid
const char* password = "51685168"; //輸入wifi 密碼
// const char* ssid = "iPhone-YJL"; //輸入wifi ssid
// const char* password = "newpsw714734"; //輸入wifi 密碼
String eventName = "cshs_voltage_3";
String key="dDlLrxTmY0OT2FcwExJT9AgeNpX55yYjkWfAdvTx0d4";
String iftttUrl="https://maker.ifttt.com/trigger/" + eventName + "/with/key/" + key;
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void initWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("STAIP address: ");
  Serial.println(WiFi.localIP());  
}

void initNTP() {
  Serial.print("Initializing NTP client");
  timeClient.begin();
  timeClient.setTimeOffset(TIME_ZONE * 3600);
  while(!timeClient.update()) {
    Serial.print(".");
    timeClient.forceUpdate();
  }
  Serial.println("\nNTP client is ready.");
}

String getCurrentDate() {
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  String currentMonthName = months[currentMonth-1];
  int currentYear = ptm->tm_year+1900;
  return String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
}

struct simpleDatetime getCurrentDateTime() {
  struct simpleDatetime dt;
  // String formattedDate = timeClient.getFormattedTime();
  // int splitT = formattedDate.indexOf("T");
  // dt.date = formattedDate.substring(0, splitT);
  // dt.hour = formattedDate.substring(splitT+1, formattedDate.length()-1);
  dt.date = getCurrentDate();
  dt.hour = timeClient.getFormattedTime();
  return dt;
}

int sendIFTTT(double voltage) {
  HTTPClient http;
  Serial.print("===HTTP Send data to IFTTT===\n");
  http.begin(iftttUrl + "?value1=" + String(voltage));
  Serial.print("===HTTP GET Status===\n");
  int httpCode = http.GET(); //Get status
    if(httpCode > 0) {
      Serial.printf("HTTP to get Feeback status: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload); //印出feedback字串
      }
      else {
        Serial.printf("HTTP to get Feedback failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    Serial.println("=================================");
    http.end();
  }
}

String getRequestUrl(double voltage) {
  struct simpleDatetime dtNow = getCurrentDateTime();
  return iftttUrl 
    + "?value1=" + String(voltage)
    + "&value2=" + String(dtNow.date)
    + "&value3=" + String(dtNow.hour);
}

void setup() {
  Serial.begin(115200); // console display
  initWiFi();
  initNTP();
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2); // UART protocol
}

void loop() {
  Serial.println("Reading...");
  String voltageStr = Serial2.readString();
  if (voltageStr.length() > 0) {
    Serial.print("Message Received: ");
    Serial.println(voltageStr);
    
    HTTPClient http;
    Serial.print("===HTTP Send data to IFTTT===\n");
    // String requestStr = iftttUrl + "?value1=" + String(voltageStr.toFloat());
    String requestStr = getRequestUrl(voltageStr.toFloat());
    Serial.println("Getting: " + requestStr);
    http.begin(requestStr);
    int httpCode = http.GET(); //Get status
      Serial.println(String(httpCode) + "===\n");
      if(httpCode > 0) {
        Serial.printf("HTTP to get Feeback status: %d\n", httpCode);
        if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload); //印出feedback字串
        }
        else {
          Serial.printf("HTTP to get Feedback failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
      Serial.println("=================================");
      http.end();
      delay(1500);
    }
    else {
      delay(100);
    }
  }
}