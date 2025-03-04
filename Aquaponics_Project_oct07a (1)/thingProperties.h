// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "34a815ce-84e2-4997-83cd-b5bcbb3a0786";

const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onPumpChange();
void onFeederChange();

float ph;
CloudTemperatureSensor temperature;
int pump;
CloudPercentage water_level;
bool feeder;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(ph, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(pump, READWRITE, ON_CHANGE, onPumpChange);
  ArduinoCloud.addProperty(water_level, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(feeder, READWRITE, ON_CHANGE, onFeederChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
