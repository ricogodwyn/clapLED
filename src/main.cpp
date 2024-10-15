#include <FastLED.h>
#include <OneButton.h>
#include <vector>

#define LED_PIN 15        // Pin for WS2812B data
#define NUM_LEDS 60      // Number of LEDs on the strip
#define BUTTON_PIN 2     // Pin for the button

CRGB leds[NUM_LEDS];
OneButton button(BUTTON_PIN, true);  // Button setup (true = active low)
std::vector<int> dotPositions;       // Vector to store positions of running dots
unsigned long lastUpdate = 0;        // Time of last update
int delayTime = 10;                 // Delay between each LED update

void addNewDot() {
  // Add a new dot at position 0 (starting point)
  dotPositions.push_back(0);
}

void updateDots() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= delayTime) {
    // Clear the LEDs
    FastLED.clear();

    // Loop through all active dots and update their positions
    for (int i = 0; i < dotPositions.size(); i++) {
      if (dotPositions[i] < NUM_LEDS) {
        leds[dotPositions[i]] = CHSV(0, 255, 255); // Set the dot color (e.g., red)
        
        // Move the dot forward
        dotPositions[i]++;
      }
    }

    // Remove any dots that have reached the end of the strip
    dotPositions.erase(std::remove_if(dotPositions.begin(), dotPositions.end(),
                                      [](int pos){ return pos >= NUM_LEDS; }), dotPositions.end());

    // Show the updated LED positions
    FastLED.show();

    // Update the last update time
    lastUpdate = currentTime;
  }
}


void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  button.attachClick(addNewDot);     // Button click adds a new dot
  FastLED.clear();                   // Start with all LEDs off
  FastLED.show();
  Serial.begin(9600);
}

void loop() {
  button.tick();  // Check for button presses
  updateDots();   // Update positions of all running dots
}