#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(4);

const char* ssid = "";
const char* password = "22082001";
const int ledPin = 2;

AsyncWebServer server(80);

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Turn off LED initially

  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  IPAddress localIP = WiFi.localIP();
  Serial.print("ESP32 IP Address: ");
  Serial.println(localIP.toString());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = R"(
      <html>
      <head>
        <style>
          body {
            background-color: #f0f0f0;
            color: #333;
            font-family: 'Arial', sans-serif;
            text-align: center;
          }
          h1 {
            color: blue;
          }
          .container {
            max-width: 600px;
            margin: 0 auto;
            padding: 20px;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
          }
          input[type="text"] {
            width: 100%;
            padding: 15px;
            margin: 10px 0;
            box-sizing: border-box;
            border: 2px solid blue;
            border-radius: 4px;
            background-color: green;
            color: white;
          }
          input[type="submit"], button {
            background-color: red;
            color: white;
            padding: 14px 20px;
            margin: 8px 0;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-weight: bold;
          }
          button {
            background-color: yellow;
          }
        </style>
      </head>
      <body>
        <h1>IoT Projects</h1>
        <div class="container">
          <form action="/submit" method="get">
            <label for="text">Enter your text:</label>
            <input type="text" id="text" name="text" maxlength="5">
            <input type="submit" value="Submit">
          </form>
          <button onclick="location.href='/refresh'">Refresh OLED</button>
        </div>
      </body>
      </html>
    )";
    request->send(200, "text/html", html);
  });

  server.on("/submit", HTTP_GET, [](AsyncWebServerRequest *request) {
    String text = request->getParam("text")->value();
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(5, 3);
    display.println(text);
    display.display();

    if (text == "AMINA") {
      digitalWrite(ledPin, HIGH); // Turn on LED if text is "AMINA"
    }

    request->redirect("/");
  });

  server.on("/refresh", HTTP_GET, [](AsyncWebServerRequest *request) {
    display.clearDisplay();
    display.display();
    digitalWrite(ledPin, LOW); // Turn off LED when refreshing
    request->redirect("/");
  });

  server.begin();
}

void loop() {
  // Nothing here
}
