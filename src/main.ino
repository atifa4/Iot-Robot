#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal.h>
const int rs = 5, en = 18, d4 = 19, d5 = 21, d6 = 22, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int GAS=34;
int TEMP=35;
int load1_gpio= 2;
int load2_gpio=15;
int load3_gpio= 13;
int load4_gpio= 12;
#define I2C_SDA 25
#define I2C_SCL 26
int ADXL345 = 0x53;
TwoWire I2CBME = TwoWire(0);
float X_out, Y_out;
int G, T, G1, t1, t2;

String sensor1_status;
String sensor2_status;
String sensor3_status;
String sensor4_status;
String sensor5_status;
String sensor6_status;
String sensor7_status;
String sensor8_status;
String sms_status;
 String lattitude_1 ;
 String longitude_1 ;
void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(GAS, INPUT);
  pinMode(TEMP, INPUT);
  pinMode(load1_gpio, OUTPUT);
  pinMode(load2_gpio, OUTPUT);
  pinMode(load3_gpio, OUTPUT);
  pinMode(load4_gpio, OUTPUT);
  digitalWrite(load1_gpio, LOW);
  digitalWrite(load2_gpio, LOW);
  digitalWrite(load3_gpio, LOW);
  digitalWrite(load4_gpio, LOW);
  Serial.println("iotbegin487");
  WiFi.begin("iotbegin487", "iotbegin487");  //WiFi connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    lcd.setCursor(0, 0);
    lcd.print("Connecting to  ");
    lcd.setCursor(0, 1);
    lcd.print(" iotbegin487");
    Serial.println("Waiting for Wi-Fi connection");
  }
  Serial.println("Wi-Fi connected");
  lcd.setCursor(0, 1);
  lcd.print("Wi-Fi connected");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   IOT BASED   ");
  lcd.setCursor(0, 1);
  lcd.print(" DISASTER ROBOT ");
  delay(3000);
  lcd.clear();
  Wire.begin(I2C_SDA, I2C_SCL, 100000);  // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345);  // Start communicating with the device
  Wire.write(0x2D);                 // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8);  // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  Wire.endTransmission();
  delay(10);
}
void loop()
{
  G=analogRead(GAS);
  T=analogRead(TEMP);
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);  // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);
  if(G < 3000)
  {
    G1 = 0;
  }
  else
  {
    G1 = (G - 3000)/10.95;
  }
  if(T < 400)
  {
    t1 = 25;
  }
  else
  {
    t1 = (T / 22.5);
  }
  if(t1 < 25)
  {
    t2 = 25;
  }
  else
  {
    t2 = t1;
  }
  if((X_out < 0.30)&&(Y_out > 250))
  {
    Serial.println("MEMS NORMAL");
    sensor3_status=" MEMS NORMAL";
  }
  else
  {
    Serial.println("MEMS ABNORMAL");
    sensor3_status=" MEMS ABNORMAL";
  }
  X_out = (Wire.read() | Wire.read() << 8);  // X-axis value
  X_out = X_out / 255;
  Y_out = (Wire.read() | Wire.read() << 8);  // Y-axis value
  Y_out = Y_out / 255;
  
  lcd.setCursor(0, 0);
  lcd.print("G:");
  lcd.print(G1);
  lcd.print("  ");
  lcd.setCursor(8, 0);
  lcd.print("T:");
  lcd.print(t2);
  lcd.print("  ");  
  Serial.print("Xa= ");
  Serial.print(X_out);
  Serial.print("   Ya= ");
  Serial.println(Y_out);
  delay(500);
  if(G1 > 50)
  {
    sensor1_status="G:"+String(G1)+" GAS ABNORMAL";
  }
  else if(G1 == 0)
  {
    sensor1_status="G:"+String(G1);
  }
  else
  {
    sensor1_status="G:"+String(G1)+" GAS NORMAL";
  }
  if(t2 > 33)
  {
    sensor2_status="T:"+String(t2)+" TEMPERATURE ABNORMAL";   
  }
  else
  {
    sensor2_status="T:"+String(t2)+" TEMPERATURE NORMAL";    
  }
  iot();
  location();
  HTTPClient http;    //Declare object of class HTTPClient
  http.begin("http://iotbegineer.com/api/devices");      //Specify request destination
  http.addHeader("username",  "iotbegin487"); //Specify content-type header
  int httpCode = http.GET();
  String payload = http.getString();                  //Get the response payload
  //Serial.println(httpCode);   //Print HTTP return code
  //Serial.println(payload);    //Print request response payload
  http.end();  //Close connection
 // delay(500);
  StaticJsonDocument<300> parseload;
  deserializeJson(parseload, payload);
  //Serial.println();
  JsonObject load_data  = parseload["data"];
  String device1_status = load_data["device1"];
  String device2_status = load_data["device2"];
  String device3_status = load_data["device3"];
  String device4_status = load_data["device4"];

  if((device1_status == "1")&&(device2_status == "0")&&(device3_status == "0")&&(device4_status == "0"))
  {
    digitalWrite(load1_gpio, HIGH);
    digitalWrite(load2_gpio, LOW);
    digitalWrite(load3_gpio, HIGH);
    digitalWrite(load4_gpio, LOW);
    lcd.setCursor(0, 1);
    lcd.print("    FORWARD     ");
  }
  else if((device1_status == "0")&&(device2_status == "1")&&(device3_status == "0")&&(device4_status == "0"))
  {
    digitalWrite(load1_gpio, LOW);
    digitalWrite(load2_gpio, HIGH);
    digitalWrite(load3_gpio, LOW);
    digitalWrite(load4_gpio, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("    BACKWARD    ");
  }
  else if((device1_status == "0")&&(device2_status == "0")&&(device3_status == "1")&&(device4_status == "0"))
  {
    digitalWrite(load1_gpio, LOW);
    digitalWrite(load2_gpio, HIGH);
    digitalWrite(load3_gpio, HIGH);
    digitalWrite(load4_gpio, LOW);  
    lcd.setCursor(0, 1);
    lcd.print("      LEFT      ");
  }
   else if((device1_status == "0")&&(device2_status == "0")&&(device3_status == "0")&&(device4_status == "1"))
  {
    digitalWrite(load1_gpio, HIGH);
    digitalWrite(load2_gpio, LOW);
    digitalWrite(load3_gpio, LOW);
    digitalWrite(load4_gpio, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("      RIGHT     ");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    digitalWrite(load1_gpio, LOW);
    digitalWrite(load2_gpio, LOW);
    digitalWrite(load3_gpio, LOW);
    digitalWrite(load4_gpio, LOW); 
  }
}
void iot()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    DynamicJsonDocument jsonBuffer(JSON_OBJECT_SIZE(3) + 300);
    JsonObject root = jsonBuffer.to<JsonObject>();

    root["sensor1"] = sensor1_status;
    root["sensor2"] = sensor2_status;
    root["sensor3"] = sensor3_status;
    root["sensor4"] = sensor4_status;
    root["sensor5"] = sensor5_status;
    root["sensor6"] = sensor6_status;
    root["sensor7"] = sensor7_status;
    root["sensor8"] = sensor8_status;
    root["sms"]     = sms_status;
    String json;
    serializeJson(jsonBuffer, json);
    if (sensor1_status != "null")
    {
      HTTPClient http;    //Declare object of class HTTPClient
      http.begin("http://iotbegineer.com/api/sensors");      //Specify request destination
      http.addHeader("username",  "iotbegin487"); //Specify content-type header
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(json);   //Send the request
      String payload = http.getString();   //Get the response payload
      http.end();
    }
  }
  else
  {
    Serial.println("Error in WiFi connection");
  }
}

void location() {
  HTTPClient http;
  http.begin("http://iotbegineer.com/api/locations");  //Specify request destination
  http.addHeader("username", "iotbegin487");           //Specify content-type header
  int httpCode_string = http.GET();
  String payload_string = http.getString();  //Get the response payload
  http.end();                                //Close connection
  //delay(500);
  StaticJsonDocument<300> parseserial_string;
  deserializeJson(parseserial_string, payload_string);
  JsonObject serial_string = parseserial_string["data"][0];
  String lattitude = serial_string["latitude"];
  String longitude = serial_string["longitude"];
  lattitude_1 = " lat:" + lattitude;
  longitude_1 = "long" + longitude;
  sensor4_status = lattitude_1;
  sensor5_status = longitude_1;
}
