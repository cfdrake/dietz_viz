/**
 * Ada Dietz polynomial (a + b + ...) ^ N pattern visualizer
 * by: Colin Drake
 * version: 1.0
 * 
 * Requires:
 * - Arduino
 * - ILI9341 OLED screen (Adafruit shield available)
 * 
 * Todos:
 * - Adding (a + b + c) ^ 3 brings system to 99% of dynamic memory, and unable to deploy.
 *   Need to look into PROGMEM potentially to store more data.
 *   See: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
 * - Look into disabling LED backlights.
 *   See: https://learn.adafruit.com/adafruit-2-8-tft-touch-shield-v2/backlight-touch-irq
 */

#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

// OLED setup.
Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9);

// Drawing constants.
const int tile_size = 8;
const int tile_line_spacing = 4;
const int tile_drawing_delay = 10;  // ms
const int tile_drawing_color = ILI9341_WHITE;
const int pattern_drawing_delay = (1000 * 10);  // ms

// Pattern data.
const int num_patterns = 6;
const int max_pattern_length = 128;
const int patterns[num_patterns][max_pattern_length] = {
  // (a + b)
  {0, 1},
  // (a + b) ^ 2
  {0, 0, 0, 1, 0, 1, 1, 1},
  // (a + b) ^ 3
  {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1},
  // (a + b) ^ 4
  {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
  // (a + b + c)
  {0, 1, 2},
  // (a + b + c) ^ 2
  {0, 0, 1, 0, 2, 0, 1, 1, 1, 2, 1, 2, 2, 2},
  // (a + b + c) ^ 3
  // {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 2, 2, 0, 2, 2, 0, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 0, 1, 1, 0, 1, 1, 0, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 2, 2}
};
const int pattern_lengths[num_patterns] = {
  2,
  8,
  24,
  64,
  3,
  14,
  //81
};

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  // For each pattern...
  for (int i = 0; i < num_patterns; i++) {
    long pattern_index = 0;
    
    // For each tile...
    for (long y = 0; y < tft.height(); y += tile_size) {
      for (long x = 0; x < tft.width(); x += tile_size) {
        // Grab current value of current pattern.
        int patternValue = patterns[i][pattern_index % pattern_lengths[i]];
  
        if (patternValue == 0) {
          a(x, y);
        } else if (patternValue == 1) {
          b(x, y);
        } else if (patternValue == 2) {
          c(x, y);
        }

        delay(tile_drawing_delay);  // Note: per tile.

        // Increment pattern index.
        pattern_index++;
      }
    }

    // Hold current finished pattern, clear screen, move to next pattern...
    delay(pattern_drawing_delay);
    tft.fillScreen(ILI9341_BLACK);
  }
}

void a(int x, int y) {
  for (int i = 0; i < tile_size; i += tile_line_spacing) {
    tft.drawLine(x, y + i, x + tile_size, y + i, tile_drawing_color);
  }
}

void b(int x, int y) {
  for (int i = 0; i < tile_size; i += tile_line_spacing) {
    tft.drawLine(x + i, y, x + i, y + tile_size, tile_drawing_color);
  }
}

void c(int x, int y) {
  tft.drawLine(x, y, x + tile_size, y + tile_size, tile_drawing_color);
}
