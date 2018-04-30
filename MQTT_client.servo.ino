#include <ESP8266WiFi.h>
#include <PubSubClient.h>



#include <Servo.h> 
Servo myservo; 



const char* ssid = "LDV-CPP";
const char* password = "LDV-LK-C++";
const char* mqtt_server = "ajax-der-kleine.clients.ldv.ei.tum.de";    ///broker MQTT
 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {

  Serial.begin(115200);
   myservo.attach(2);  // attach your servo to pin D4 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
   String string;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
   // Serial.print((char)payload[i]);
     string+=((char)payload[i]);  // converting the message received to a string 
  }
  
    Serial.print(string);

if (topic ="servo")   
    Serial.print(" ");
   int resultado = string.toInt();   // converting the string to integer 
   int pos = map(resultado, 1, 100, 32, 165); // converting the integer to an Angle for the servo motor 
   Serial.println(pos);
    myservo.write(pos);
    delay(15); 
   
 }


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {

      Serial.println("connected");
      client.subscribe("servo");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {



   
  if (!client.connected()) {
    reconnect();

  }
  client.loop();


 delay(100);
  
}
