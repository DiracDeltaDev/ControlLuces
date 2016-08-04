#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define SSID        "MY HOME"
#define PASSPHRASE  "ZOCOZOPAGUA"
#define DEVICE_NAME "esp-01-dev1"
#define mqtt_server "iot.eclipse.org"

char* WELCOME_TOPIC = "welcome";
char* LIGHT_TOPIC = "light";
void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(mqtt_server,1883,callback,espClient);

void setup(){
  delay(1000);
  Serial.println();
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(SSID,PASSPHRASE);
  Serial.println("Dios es Grande!");
  Serial.println(":D:D");
  Serial.print("connecting ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Serial.println("En un segundo se realizará el envío de información");
  delay(1000);

  /** Client connect to mqtt broker **/
  if (client.connect(DEVICE_NAME)){
    client.subscribe(LIGHT_TOPIC);
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(WELCOME_TOPIC);
    if (client.publish(WELCOME_TOPIC, "Se inicializó con éxito")) {
      Serial.println("Publish ok");
    }else {
      Serial.println("Publish failed");
    }
  }else{
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}

void loop(){
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  char p[20];
  for(i = 0;i<length;i++){
    p[i] = payload[i];
  }
  p[i] = '\0';
  String _payload = p;
  String _topic = topic;
  Serial.print("received: {");
  Serial.print(_topic);
  Serial.print(":");
  Serial.print(_payload);
  Serial.println("}");
  if(_payload.equals("1")) digitalWrite(2, HIGH);
  else if(_payload.equals("0")) digitalWrite(2, LOW);
}
