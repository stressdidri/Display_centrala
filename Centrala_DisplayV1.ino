
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 256 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

int heatPin = D6; // incalzire 
int coldPin = D7; // racire
 
/* Set these to your desired credentials. */
const char *ssid = "SSID";  //ENTER YOUR WIFI SETTINGS
const char *password = "XXXXXX";

 
//Web/Server address to read/write from 
const char *host = "http://yourdomain.com";   //https://circuits4you.com website or IP address of server
 
//=======================================================================
//                    Power on setup
//=======================================================================
 
void setup() {

  Serial.begin(115200);
  delay(10);

  WiFi.setAutoReconnect(true);
  
 
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Boot...");
  delay(4000);
  display.display();
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Conectare...");
  delay(100);

  pinMode(LED_BUILTIN, OUTPUT); 
  
  pinMode(heatPin, OUTPUT);
  digitalWrite(heatPin, LOW);

  pinMode(coldPin, OUTPUT);
  digitalWrite(coldPin, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Pornesc...");
  delay(4000);
  display.display();
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
 HTTPClient http;    //Declare object of class HTTPClient
 
  String ADCData, station, postData;
  int adcvalue=analogRead(A0);  //Read Analog value of LDR
  ADCData = String(adcvalue);   //String to interger conversion
  station = "A";
 
  //Post Data
  postData = "status=" + ADCData + "&station=" + station ;
  
  http.begin("http://yourdomain.com/page.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  /// iau in ram
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(payload);
  display.display();
  delay(10000);

  // versiune

  //display.clearDisplay();
  //display.setTextSize(1);
  //display.setTextColor(WHITE);
  //display.setCursor(0,0);
  //display.println("Versiune Soft: ");
  //display.println("Beta 1.0");
  //display.println("ADRIAN TUDORACHE");
  //display.display();
  //delay(8000);

  /// debug

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Salut sunt la:");
  display.println(WiFi.localIP());
  display.println("Retea Locala: OK...");
  display.println("Semnal WIFI BUN... ");
  display.display();
  delay(5000);

  /// comunicare

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Stare ... OK");
  display.println("Centrala : ONLINE");
  display.println("Verific Temperaturi");
  display.println("Ambient / Setata");
  display.display();
  delay(9000);

  /// ce urmeaza sa arat din ram
    
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Ambient...");
  display.println("Setata...");
  display.display();
  delay(8000);


  
  http.end();  //Close connection
  delay(5000);  //Post Data at every 5 seconds

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("Client Conectat");
  while(!client.available()){
    delay(10);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  delay(100);
 
  // incepe controlul pinilor

  /// int coldValue = digitalRead(D5); /// citire si afisare status pin
  /// int hotValue = digitalRead(D6); /// citire si afisare status pin
 
  int hot = LOW;
  if (request.indexOf("/HOT=ON") != -1) {
    digitalWrite(heatPin, HIGH);
    hot = HIGH;
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
    delay(20); 
    digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level 
  } 
  if (request.indexOf("/HOT=OFF") != -1){
    digitalWrite(heatPin, LOW);
    hot = LOW;
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
    delay(20); 
    digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level 
  }


  int cold = LOW;
  if (request.indexOf("/COLD=ON") != -1) {
    digitalWrite(coldPin, HIGH);
    cold = HIGH;
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
    delay(20); 
    digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level 
  } 
  if (request.indexOf("/COLD=OFF") != -1){
    digitalWrite(coldPin, LOW);
    cold = LOW;
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
    delay(20); 
    digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level 
  }



    if (client) {
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      delay(100);
      if (client.available()) {
        char c = client.read();

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println();
        break;
        }

      }
    }  
    // closing the client connection
    delay(10);
    client.stop();
    Serial.println("Client disconnected.");
}
