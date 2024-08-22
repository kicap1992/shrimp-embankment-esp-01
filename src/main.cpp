#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// WiFi credentials
// const char *ssid = "KARAN";
// const char *password = "12345679";
const char *ssid = "RIZALDY";
const char *password = "capputo322";


WiFiClient client;

void setup()
{
  Serial.begin(115200);
  // pinMode(2, OUTPUT);
  // digitalWrite(2, HIGH);
  Serial.println("ESP-01 ready");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  Serial.println("Starting up...");

  delay(2000);
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    // Serial.println("Wifi connected");
    // digitalWrite(2, LOW);
    String IncomingStr = "";

    while (Serial.available() > 0)
    {
      char in = Serial.read();
      IncomingStr += in;
      delay(2);
    }

    if (IncomingStr.length() > 0)
    {
      Serial.print("Wifi connected,");
      IncomingStr = IncomingStr.substring(0, IncomingStr.indexOf('\n'));

      String jsonSent = "{\"value\": \"" + IncomingStr + "\"}";

      // Initialize HTTP client
      HTTPClient http;
      http.begin(client, "http://192.168.20.45:3006");

      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(jsonSent);

      if (httpResponseCode > 0)
      {
        // Serial.print("HTTP Response code: ");
        // Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.print(payload);
      }
      else
      {
        Serial.print("Error code: ");
        Serial.print(httpResponseCode);
      }

      // Close connection
      http.end();
      delay(1000);
    }

    // Send data to Arduino
    // Serial.println("Hello from ESP-01!");
  }
  else
  {
    // Reconnect to WiFi if disconnected
    WiFi.begin(ssid, password);
    Serial.println("Wifi disconnected");
    // digitalWrite(2, HIGH);

    // Wait for connection
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20)
    { // wait for 10 seconds max
      delay(500);
      // Serial.print(".");
      timeout++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      // Serial.println("");
      // Serial.println("Reconnected to WiFi");
      // Serial.println("IP address: ");
      // Serial.println(WiFi.localIP());
    }
    else
    {
      // Serial.println("");
      // Serial.println("Failed to reconnect to WiFi");
    }
  }
  delay(2000);
}
