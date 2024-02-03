#include<ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>

#define rx D6
#define tx D5

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial gpsSerial(rx, tx); //rx,tx

const char* ssid = "iot";
const char* password = "12345678";
char serverName[] = "http://iotcloud22.in/vellore_vehicle_tracking/post_value.php";

WiFiClient client;
HTTPClient http;

int flag, Acc, Gas, Rpm;
String acc, gas, rpm;
int len;
int cnt = 0;
String inString = "";
String httpRequestData;

int updates;
int failedUpdates;
int pos;
int stringplace = 0;


String timeUp;
String nmea[15];
String labels[12] {"Time: ", "Status: ", "Latitude: ", "Hemisphere: ", "Longitude: ", "Hemisphere: ", "Speed: ", "Track Angle: ", "Date: "};

String ch;
String lat = "0.0000";
String lon = "0.0000";
int sms;
void setup() {
  lcd.init();
  lcd.backlight();
  //  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print("Vehicle Tracking");
  lcd.setCursor(0, 1);
  lcd.print("    System    ");
  delay(2000);
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print(WiFi.localIP());
  delay(1000);

}

void loop()
{
  while (gpsSerial.available() > 0) {

    char c = gpsSerial.read();
    Serial.print(c);
    if (c == '\n' || c == '\r') {
      if (cnt >= 2) {
        status_filter(inString);
        status_filter1(inString);
        status_filter2(inString);
        //Serial.println(inString);
        inString = "";
        cnt = 0;
      }
      else
      {
        inString = "";
        cnt = 0;
      }
    }
    else
    {
      inString += c;
      cnt++;
    }
  }
}
void status_filter(String buff)
{
  len = buff.length();
  //Serial.println(len);
  if (buff[0] == 'A')
    if (buff[2] == 'G')
      if (buff[4] == 'R')
        if (buff[6] == '#')

          //A0G0R0#
          //0123456

        {
          acc = buff.substring(1, 2);
          gas = buff.substring(3, 4);
          rpm = buff.substring(5, 6);

          Acc = acc.toInt();
          Gas = gas.toInt();
          Rpm = map(rpm.toInt(), 0, 255, 0, 100);
          if (Acc == 1)
          {
            sendsms();
            flag = 1;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Accident detected");
          }
          else if (Gas == 1)
          {
            Sendsms();
            flag = 2;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("  Gas detected  ");
          }
          else
          {
            flag = 0;
            sms = 0;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Vehicle  Speed ");
            lcd.setCursor(0, 1);
            lcd.print(Rpm);
            //            lcd.print();
          }
          Serial.print("Acc = "); Serial.println(Acc);
          Serial.print("Gas = "); Serial.println(Gas);
          Serial.print("Rpm = "); Serial.println(Rpm);
          Serial.print("Flag = "); Serial.println(flag);
          gps();
          sending_to_db();
        }
}
void status_filter1(String buff)
{
  len = buff.length();
  //Serial.println(len);
  if (buff[0] == 'A')
    if (buff[2] == 'G')
      if (buff[4] == 'R')
        if (buff[7] == '#')

          //A0G0R00#
          //0123456

        {
          acc = buff.substring(1, 2);
          gas = buff.substring(3, 4);
          rpm = buff.substring(5, 7);

          Acc = acc.toInt();
          Gas = gas.toInt();
          Rpm = map(rpm.toInt(), 0, 255, 0, 100);
          if (Acc == 1)
          {
            sendsms();
            flag = 1;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Accident detected");
          }
          else if (Gas == 1)
          {
            Sendsms();
            flag = 2;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("  Gas detected  ");
          }
          else
          {
            sms = 0;
            flag = 0;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Vehicle  Speed ");
            lcd.setCursor(0, 1);
            lcd.print(Rpm);
            //            lcd.print();
          }
          Serial.print("Acc = "); Serial.println(Acc);
          Serial.print("Gas = "); Serial.println(Gas);
          Serial.print("Rpm = "); Serial.println(Rpm);
          Serial.print("Flag = "); Serial.println(flag);
          gps();
          sending_to_db();
        }
}
void status_filter2(String buff)
{
  len = buff.length();
  //Serial.println(len);
  if (buff[0] == 'A')
    if (buff[2] == 'G')
      if (buff[4] == 'R')
        if (buff[8] == '#')

          //A0G0R000#
          //0123456

        {
          acc = buff.substring(1, 2);
          gas = buff.substring(3, 4);
          rpm = buff.substring(5, 8);

          Acc = acc.toInt();
          Gas = gas.toInt();
          Rpm = map(rpm.toInt(), 0, 255, 0, 100);
          if (Acc == 1)
          {
            sendsms();
            flag = 1;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Accident detected");
          }
          else if (Gas == 1)
          {
            Sendsms();
            flag = 2;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("  Gas detected  ");
          }
          else
          {
            sms = 0;
            flag = 0;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Vehicle  Speed ");
            lcd.setCursor(0, 1);
            lcd.print(Rpm);
            //            lcd.print();
          }
          Serial.print("Acc = "); Serial.println(Acc);
          Serial.print("Gas = "); Serial.println(Gas);
          Serial.print("Rpm = "); Serial.println(Rpm);
          Serial.print("Flag = "); Serial.println(flag);
          gps();
          sending_to_db();
        }
}



void sendsms()
{
  while (sms < 2)
  {
    Serial.println("AT\r");
    delay(1000);
    Serial.println("AT+CMGF=1\r");
    delay(1000);
    Serial.println("AT+CMGS=\"+918098076074\"\r");
    //    Serial.println("AT+CMGS=\"+919600808315\"\r");
    delay(1000);
    Serial.println("EMERGENCY!!!");
    Serial.println("Your Vehicle has met with an accident");
    Serial.println("Reported to the given emergency contact number");
    Serial.println("Track your vehicle on below mentioned link");
    Serial.println("http://iotcloud22.in/vellore_vehicle_tracking");
    delay(1000);
    Serial.println((char)26);
    delay(2000);
    gpsSerial.println("AT\r");
    delay(1000);
    gpsSerial.println("AT+CMGF=1\r");
    delay(1000);
    gpsSerial.println("AT+CMGS=\"+918098076074\"\r");
    //    gpsSerial.println("AT+CMGS=\"+919600808315\"\r");
    delay(1000);
    gpsSerial.println("EMERGENCY!!!");
    gpsSerial.println("Your Vehicle has met with an accident");
    gpsSerial.println("Reported to the given emergency contact number");
    gpsSerial.println("Track your vehicle on below mentioned link");
    gpsSerial.println("http://iotcloud22.in/vellore_vehicle_tracking");
    delay(1000);
    gpsSerial.println((char)26);
    delay(2000);
    sms++;
  }
}

void Sendsms()
{
  while (sms < 2)
  {
    Serial.println("AT\r");
    delay(1000);
    Serial.println("AT+CMGF=1\r");
    delay(1000);
    Serial.println("AT+CMGS=\"+918098076074\"\r");
    //    Serial.println("AT+CMGS=\"+919600808315\"\r");
    delay(1000);
    Serial.println("Your vehicle fuel is leaking please check");
    Serial.println("Track your vehicle on below mentioned link");
    Serial.println("http://iotcloud22.in/vellore_vehicle_tracking");
    delay(1000);
    Serial.println((char)26);
    delay(2000);
    gpsSerial.println("AT\r");
    delay(1000);
    gpsSerial.println("AT+CMGF=1\r");
    delay(1000);
    gpsSerial.println("AT+CMGS=\"+918098076074\"\r");
    //    gpsSerial.println("AT+CMGS=\"+919600808315\"\r");
    delay(1000);
    gpsSerial.println("Your vehicle fuel is leaking please check");
    gpsSerial.println("Track your vehicle on below mentioned link");
    gpsSerial.println("http://iotcloud22.in/vellore_vehicle_tracking");
    delay(1000);
    gpsSerial.println((char)26);
    delay(2000);
    sms++;
  }


}


void gps()
{
  //  Serial.flush();
  while (Serial.available() > 0)
  {
    Serial.read();
  }
  if (Serial.find("$GPRMC,")) {
    String Msg = Serial.readStringUntil('\n');
    for (int i = 0; i < Msg.length(); i++) {
      if (Msg.substring(i, i + 1) == ",") {
        nmea[pos] = Msg.substring(stringplace, i);
        stringplace = i + 1;
        pos++;
      }
      if (i == Msg.length() - 1) {
        nmea[pos] = Msg.substring(stringplace, i);
      }
    }
    updates++;
    nmea[2] = ConvertLat();
    nmea[4] = ConvertLng();
    //for (int i = 0; i < 9; i++) {
    /*Serial.print(labels[0]);
      Serial.print(nmea[0]);
      Serial.print(labels[8]);
      Serial.println(nmea[8]);*/
    Serial.print("https://maps.google.com/maps?f=q&q=");
    Serial.print(nmea[2]);
    Serial.print(",");
    Serial.println(nmea[4]);

    int lat1 = nmea[2].toInt();
    if (lat1 > 0) {
      Serial.println("new data");
      lat = nmea[2];
      lon = nmea[4];
    }
    else {
      Serial.println("old data");

    }
    Serial.println("");
    //}

  }
  else {

    failedUpdates++;

  }
  stringplace = 0;
  pos = 0;

}

String ConvertLat() {
  String posneg = "";
  if (nmea[3] == "S") {
    posneg = "-";
  }
  String latfirst;
  float latsecond;
  for (int i = 0; i < nmea[2].length(); i++) {
    if (nmea[2].substring(i, i + 1) == ".") {
      latfirst = nmea[2].substring(0, i - 2);
      latsecond = nmea[2].substring(i - 2).toFloat();
    }
  }
  latsecond = latsecond / 60;
  String CalcLat = "";

  char charVal[9];
  dtostrf(latsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLat += charVal[i];
  }
  latfirst += CalcLat.substring(1);
  latfirst = posneg += latfirst;
  return latfirst;
}

String ConvertLng() {
  String posneg = "";
  if (nmea[5] == "W") {
    posneg = "-";
  }

  String lngfirst;
  float lngsecond;
  for (int i = 0; i < nmea[4].length(); i++) {
    if (nmea[4].substring(i, i + 1) == ".") {
      lngfirst = nmea[4].substring(0, i - 2);
      //Serial.println(lngfirst);
      lngsecond = nmea[4].substring(i - 2).toFloat();
      //Serial.println(lngsecond);

    }
  }
  lngsecond = lngsecond / 60;
  String CalcLng = "";
  char charVal[9];
  dtostrf(lngsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLng += charVal[i];
  }
  lngfirst += CalcLng.substring(1);
  lngfirst = posneg += lngfirst;
  return lngfirst;
}

void sending_to_db()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");


    httpRequestData = "&value1=" + String(lat) + "&value2=" + String(lon) + "&value3=" + String(Acc) + "&value4=" + String(Gas) + "&value5=" + String(Rpm) + "&value6=" + String(flag) + "";
    //    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

  delay(1000);
}
