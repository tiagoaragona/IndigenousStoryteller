/**
* @file tutorial.cpp
* This code is provided for educational and non-commercial use only.
* It should not be used for commercial purposes.
* 
* Created by Tiago Aragona. For more information, visit:
* - Website: tiagoaragona.com
* - LinkedIn: https://www.linkedin.com/in/tiagoaragona/
*
* This Arduino sketch interfaces with WiFi, an LCD display, and the ChatGPT API
* to display geolocated inigenous stories.
*/

// Include required libraries
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include "ChatGPT.hpp"

// WiFi credentials
const char* ssid = "PLACE_WIFI_NAME_HERE"; // SSID of the WiFi network
const char* password = "PLACE_WIFI_PASSWORD_HERE"; // Password for the WiFi network

// OpenAI API key
const char* apiKey = "PLACE_OPEN_AI_KEY_HERE"; // API key for OpenAI ChatGPT

// LCD settings
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize LCD with I2C address 0x27 and size 16x2

// Button and LEDs
const int buttonPin = 18; // GPIO pin connected to the button
const int ledRed = 19;    // GPIO pin connected to the red LED
const int ledGreen = 23;  // GPIO pin connected to the green LED

WiFiClientSecure client;
ChatGPT<WiFiClientSecure> chat_gpt(&client, "v1", apiKey); // ChatGPT object for making API calls

String globalCountry = "";
String globalRegion = "";
String globalCity = "";

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight of the LCD
  
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with internal pull-up
  pinMode(ledRed, OUTPUT); // Set the red LED pin as output
  pinMode(ledGreen, OUTPUT); // Set the green LED pin as output

  digitalWrite(ledRed, LOW); // Initialize red LED as OFF
  digitalWrite(ledGreen, LOW); // Initialize green LED as OFF

  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // Connect to the specified WiFi network

  while (WiFi.status() != WL_CONNECTED) { // Wait until connected
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");

  client.setInsecure(); // Disable SSL/TLS certificate validation

  printGeolocation(); // Fetch and display geolocation data
}

void loop() {
  if (digitalRead(buttonPin) == LOW) { // Check if button is pressed
    digitalWrite(ledRed, HIGH); // Turn on the red LED
    delay(100); // Debounce delay
    while (digitalRead(buttonPin) == LOW); // Wait for button release

    fetchAndDisplayStory(); // Fetch and display the story
    digitalWrite(ledRed, LOW); // Turn off the red LED after fetching
  }
}

void fetchAndDisplayStory() {
  lcd.clear(); // Clear the LCD
  lcd.setCursor(0, 0); // Set cursor to the first line
  lcd.print(globalCity); // Display the city
  lcd.setCursor(0, 1); // Set cursor to the second line
  lcd.print(globalRegion + ", " + globalCountry); // Display the region and country
  delay(5000); // Display for 5 seconds

  lcd.clear(); // Clear the LCD for new message
  lcd.setCursor(0, 0); // Set cursor to first line
  lcd.print("Loading story..."); // Display loading message

  // Blink red LED while fetching the story
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledRed, LOW);
    delay(250);
    digitalWrite(ledRed, HIGH);
    delay(250);
  }

  String prompt = "Tell me a traditional story from the aborigin culture that inhabited " + globalCity + ", " + globalRegion + ", " + globalCountry + " before colonization. Tell it as they used to tell it but in English.";
  String story = askChatGPT(prompt); // Get the story from ChatGPT
  
  digitalWrite(ledRed, LOW); // Turn off the red LED
  digitalWrite(ledGreen, HIGH); // Turn on the green LED
  displayTextOnLCD(story); // Display the story on LCD
  delay(5000); // Display the story for 5 seconds
  digitalWrite(ledGreen, LOW); // Turn off the green LED
}

String askChatGPT(String prompt) {
  String result;
  Serial.println("[ChatGPT] Asking for a story..."); // Log the query
  if (chat_gpt.simple_message("gpt-3.5-turbo", "user", prompt, result)) { // Send the prompt to ChatGPT
    Serial.println("Story received successfully:");
    Serial.println(result);
    return result; // Return the received story
  } else {
    Serial.println("Failed to fetch story from ChatGPT");
    return "Error fetching story."; // Return error message if failed
  }
}

void displayTextOnLCD(const String& story) {
  int startPos = 0;
  const int charsPerLine = 16; // Max characters per line on LCD
  const int totalChars = story.length(); // Total characters in the story

  Serial.println("Begin Story Display");

  while (startPos < totalChars) { // Loop until end of story
    lcd.clear(); // Clear LCD
    String line1 = getNextLine(story, startPos, charsPerLine); // Get the first line of text
    lcd.setCursor(0, 0); // Set cursor to first line
    lcd.print(line1); // Display first line

    if (startPos < totalChars) {
      String line2 = getNextLine(story, startPos, charsPerLine); // Get the second line of text
      lcd.setCursor(0, 1); // Set cursor to second line
      lcd.print(line2); // Display second line
    }

    delay(3000); // Wait 3 seconds before showing the next two lines
  }
  
  Serial.println("End Story Display"); // Log end of story display
}

void printGeolocation() {
  HTTPClient http; // Create HTTP client
  http.begin("http://ip-api.com/json/?fields=country,regionName,city,lat,lon,query"); // HTTP request to fetch geolocation data
  int httpCode = http.GET(); // Make the GET request

  if (httpCode == HTTP_CODE_OK) { // Check the response status
    String payload = http.getString(); // Get the response payload
    DynamicJsonDocument doc(1024); // Create JSON document
    deserializeJson(doc, payload); // Deserialize JSON data
    globalCountry = doc["country"].as<String>(); // Extract country
    globalRegion = doc["regionName"].as<String>(); // Extract region
    globalCity = doc["city"].as<String>(); // Extract city

    Serial.print("Location: ");
    Serial.print(globalCity);
    Serial.print(", ");
    Serial.print(globalRegion);
    Serial.print(", ");
    Serial.println(globalCountry); // Log location data
  } else {
    Serial.println("Failed to retrieve geolocation data."); // Log failure
  }
  http.end(); // End HTTP connection
}

String getNextLine(const String& text, int& start, int maxLen) {
  if (start >= text.length()) return ""; // Check if all text has been processed

  int end = start + maxLen; // Determine endpoint based on maximum length
  if (end > text.length()) end = text.length(); // Adjust endpoint if it exceeds text length

  if (end < text.length() && text.charAt(end) != ' ') { // Check if endpoint is not at a space
    int lastSpace = text.lastIndexOf(' ', end - 1); // Find last space before endpoint
    if (lastSpace >= start) {
      end = lastSpace + 1; // Adjust endpoint to last space
    } else {
      // If no space, find next space after current endpoint
      int nextSpace = text.indexOf(' ', end);
      if (nextSpace != -1 && nextSpace - start <= maxLen) {
        end = nextSpace + 1; // Adjust endpoint to next space
      }
      // If no suitable space is found, endpoint remains unchanged
    }
  }

  String line = text.substring(start, end); // Extract the line of text
  start = end; // Update start position for next iteration
  line.trim(); // Trim whitespace from the line
  return line; // Return the trimmed line
}
