#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoOSC.h>
#include <ESPDMX.h>

//for LED status
#include <Ticker.h>
Ticker ticker;

void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

const int OSC_RECV_PORT = 53101;

DMXESPSerial DMXu1;
DMXESPSerial DMXu2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //set led pin as output
  pinMode(BUILTIN_LED, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  Serial.println("IP ADDR: ");
  Serial.println(WiFi.localIP());

  ticker.detach();

  Serial.swap();
  DMXu1.init(&Serial1, 4, 512); // (pin d4 = tx1)
  DMXu2.init(&Serial, 8, 512); // (pin d8 = Serial swapped)

  OscWifi.subscribe(OSC_RECV_PORT, "/universe/1", [](const OscMessage& m) {
    DMXu1.write(m.arg<int>(0), m.arg<int>(1));
  });

  OscWifi.subscribe(OSC_RECV_PORT, "/universe/2", [](const OscMessage& m) {
    DMXu1.write(m.arg<int>(0), m.arg<int>(1));
  });

  //keep LED on
  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  OscWiFi.update();

}
