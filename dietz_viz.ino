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
 * - Look into disabling LED backlights.
 *   See: https://learn.adafruit.com/adafruit-2-8-tft-touch-shield-v2/backlight-touch-irq
 *   
 * Notes:
 * - Reclaimed much space by switching from int[][] to char[][] for pattern data. Potentially
 *   more data could be stored by using PROGMEM but this is already sufficient to encode more
 *   patterns than I originally began with. These are really bitfields so they could be quite
 *   compact.
 * - Started with tile_size = 8, but 16 seems better when viewed from afar and makes the
 *   patterns more recognizable.
 */

#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

// OLED setup.
Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9);

// Drawing constants.
const int tile_size = 16;
const int tile_line_spacing = 4;
const int tile_drawing_delay = 10;  // ms (for each line in pattern)
const int tile_drawing_color = ILI9341_WHITE;
const int pattern_drawing_delay = (1000 * 15);  // ms (for each finished pattern)

// Pattern data.
const int num_patterns = 9;
const int max_pattern_length = 128;
const char patterns[num_patterns][max_pattern_length] = {
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
  {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 2, 2, 0, 2, 2, 0, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 0, 1, 1, 0, 1, 1, 0, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 2, 2},
  // (a + b + c + d)
  {0, 1, 2, 3},
  // (a + b + c + d) ^ 2
  {0, 0, 0, 1, 0, 1, 0, 2, 0, 2, 0, 3, 0, 3, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 3, 3, 3}
};
const int pattern_lengths[num_patterns] = {
  2,
  8,
  24,
  64,
  3,
  14,
  81,
  4,
  34
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
        } else if (patternValue == 3) {
          d(x, y);
        }

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
    delay(tile_drawing_delay);
  }
}

void b(int x, int y) {
  for (int i = 0; i < tile_size; i += tile_line_spacing) {
    tft.drawLine(x + i, y, x + i, y + tile_size, tile_drawing_color);
    delay(tile_drawing_delay);
  }
}

void c(int x, int y) {
  tft.drawLine(x, y, x + tile_size, y + tile_size, tile_drawing_color);
  delay(tile_drawing_delay);
}

void d(int x, int y) {
  tft.drawLine(x + tile_size, y, x, y + tile_size, tile_drawing_color);
  delay(tile_drawing_delay);
}
