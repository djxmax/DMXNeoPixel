#include <WS2812FX.h>
#include <DMXSerial.h>

#define DMXLENGTH 6

#define LED_COUNT 120
#define LED_PIN 12

#define TIMER_MS 5000

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

int dipPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
int transAddress;


void setup() {
  DMXSerial.init(DMXProbe);
  DMXSerial.maxChannel(DMXLENGTH);

  ws2812fx.init();
  ws2812fx.setBrightness(30);
  ws2812fx.setSpeed(100);
  ws2812fx.setColor(0x0000FF);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();
}

void loop() {
  ws2812fx.service();
  //ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
  
  //transAddress = address();
  if (DMXSerial.receive()) {
    changeEffect(DMXSerial.read(1));
    changeDimmer(DMXSerial.read(2));
    changeSpeed(DMXSerial.read(3));
    changeColor(DMXSerial.read(4),DMXSerial.read(5),DMXSerial.read(6));
  }
}

//Read state from DIP Switch (4 positions used)
byte address() {
  int i, j = 0;

  //Get the switches state
  for (i = 0; i <= 7; i++) {
    j = (j << 1) | digitalRead(dipPins[i]);   // read each input pin
  }
  return 255-j;


  /*byte binary = 0;
  int x;
  for (byte x = 0; x < 8; x++)
  {
    byte value = digitalRead(dipPins[x]);
    binary = binary + (value << x);
  }
  return binary; //return address*/
}

void changeEffect(uint8_t value) {
  if (value >= 0 && value < 3 && (ws2812fx.getMode() != FX_MODE_STATIC)) {
    ws2812fx.setMode(FX_MODE_STATIC);
  } else if (value >= 4 && value < 7 && (ws2812fx.getMode() != FX_MODE_BLINK)) {
    ws2812fx.setMode(FX_MODE_BLINK);
  } else if (value >= 8 && value < 11 && (ws2812fx.getMode() != FX_MODE_BREATH)) {
    ws2812fx.setMode(FX_MODE_BREATH);
  } else if (value >= 12 && value < 15 && (ws2812fx.getMode() != FX_MODE_COLOR_WIPE)) {
    ws2812fx.setMode(FX_MODE_COLOR_WIPE);
  } else if (value >= 16 && value < 19 && (ws2812fx.getMode() != FX_MODE_COLOR_WIPE_INV)) {
    ws2812fx.setMode(FX_MODE_COLOR_WIPE_INV);
  } else if (value >= 20 && value < 23 && (ws2812fx.getMode() != FX_MODE_COLOR_WIPE_REV)) {
    ws2812fx.setMode(FX_MODE_COLOR_WIPE_REV);
  } else if (value >= 24 && value < 27 && (ws2812fx.getMode() != FX_MODE_COLOR_WIPE_REV_INV)) {
    ws2812fx.setMode(FX_MODE_COLOR_WIPE_REV_INV);
  } else if (value >= 28 && value < 31 && (ws2812fx.getMode() != FX_MODE_COLOR_WIPE_RANDOM)) {
    ws2812fx.setMode(FX_MODE_COLOR_WIPE_RANDOM);
  } else if (value >= 32 && value < 35 && (ws2812fx.getMode() != FX_MODE_RANDOM_COLOR)) {
    ws2812fx.setMode(FX_MODE_RANDOM_COLOR);
  } else if (value >= 36 && value < 39 && (ws2812fx.getMode() != FX_MODE_SINGLE_DYNAMIC)) {
    ws2812fx.setMode(FX_MODE_SINGLE_DYNAMIC);
  } else if (value >= 40 && value < 43 && (ws2812fx.getMode() != FX_MODE_MULTI_DYNAMIC)) {
    ws2812fx.setMode(FX_MODE_MULTI_DYNAMIC);
  } else if (value >= 44 && value < 47 && (ws2812fx.getMode() != FX_MODE_RAINBOW)) {
    ws2812fx.setMode(FX_MODE_RAINBOW);
  } else if (value >= 48 && value < 51 && (ws2812fx.getMode() != FX_MODE_RAINBOW_CYCLE)) {
    ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
  } else if (value >= 52 && value < 55 && (ws2812fx.getMode() != FX_MODE_SCAN)) {
    ws2812fx.setMode(FX_MODE_SCAN);
  } else if (value >= 56 && value < 59 && (ws2812fx.getMode() != FX_MODE_DUAL_SCAN)) {
    ws2812fx.setMode(FX_MODE_DUAL_SCAN);
  } else if (value >= 60 && value < 63 && (ws2812fx.getMode() != FX_MODE_FADE)) {
    ws2812fx.setMode(FX_MODE_FADE);
  } else if (value >= 64 && value < 67 && (ws2812fx.getMode() != FX_MODE_THEATER_CHASE)) {
    ws2812fx.setMode(FX_MODE_THEATER_CHASE);
  } else if (value >= 68 && value < 71 && (ws2812fx.getMode() != FX_MODE_THEATER_CHASE_RAINBOW)) {
    ws2812fx.setMode(FX_MODE_THEATER_CHASE_RAINBOW);
  } else if (value >= 72 && value < 75 && (ws2812fx.getMode() != FX_MODE_RUNNING_LIGHTS)) {
    ws2812fx.setMode(FX_MODE_RUNNING_LIGHTS);
  } else if (value >= 76 && value < 79 && (ws2812fx.getMode() != FX_MODE_TWINKLE)) {
    ws2812fx.setMode(FX_MODE_TWINKLE);
  } else if (value >= 80 && value < 83 && (ws2812fx.getMode() != FX_MODE_TWINKLE_RANDOM)) {
    ws2812fx.setMode(FX_MODE_TWINKLE_RANDOM);
  } else if (value >= 84 && value < 87 && (ws2812fx.getMode() != FX_MODE_TWINKLE_FADE)) {
    ws2812fx.setMode(FX_MODE_TWINKLE_FADE);
  } else if (value >= 88 && value < 91 && (ws2812fx.getMode() != FX_MODE_TWINKLE_FADE_RANDOM)) {
    ws2812fx.setMode(FX_MODE_TWINKLE_FADE_RANDOM);
  } else if (value >= 92 && value < 95 && (ws2812fx.getMode() != FX_MODE_SPARKLE)) {
    ws2812fx.setMode(FX_MODE_SPARKLE);
  } else if (value >= 96 && value < 99 && (ws2812fx.getMode() != FX_MODE_FLASH_SPARKLE)) {
    ws2812fx.setMode(FX_MODE_FLASH_SPARKLE);
  } else if (value >= 100 && value < 103 && (ws2812fx.getMode() != FX_MODE_HYPER_SPARKLE)) {
    ws2812fx.setMode(FX_MODE_HYPER_SPARKLE);
  } else if (value >= 104 && value < 107 && (ws2812fx.getMode() != FX_MODE_STROBE)) {
    ws2812fx.setMode(FX_MODE_STROBE);
  } else if (value >= 108 && value < 111 && (ws2812fx.getMode() != FX_MODE_STROBE_RAINBOW)) {
    ws2812fx.setMode(FX_MODE_STROBE_RAINBOW);
  } else if (value >= 112 && value < 115 && (ws2812fx.getMode() != FX_MODE_MULTI_STROBE)) {
    ws2812fx.setMode(FX_MODE_MULTI_STROBE);
  } else if (value >= 116 && value < 119 && (ws2812fx.getMode() != FX_MODE_BLINK_RAINBOW)) {
    ws2812fx.setMode(FX_MODE_BLINK_RAINBOW);
  } else if (value >= 120 && value < 123 && (ws2812fx.getMode() != FX_MODE_CHASE_WHITE)) {
    ws2812fx.setMode(FX_MODE_CHASE_WHITE);
  } else if (value >= 124 && value < 127 && (ws2812fx.getMode() != FX_MODE_CHASE_COLOR)) {
    ws2812fx.setMode(FX_MODE_CHASE_COLOR);
  }
}

void changeDimmer(uint8_t value) {
  if (ws2812fx.getBrightness() != value) {
    ws2812fx.setBrightness(value);
  }
}

void changeSpeed(uint8_t value) {
  if (value >= 0 && value < 49 && ws2812fx.getSpeed() != 10000) {
    ws2812fx.setSpeed(10000);
  } else if (value >= 50 && value < 99 && ws2812fx.getSpeed() != 1000) {
    ws2812fx.setSpeed(1000);
  } else if (value >= 100 && value < 149 && ws2812fx.getSpeed() != 100) {
    ws2812fx.setSpeed(100);
  } else if (value >= 150 && value < 255 && ws2812fx.getSpeed() != 10) {
    ws2812fx.setSpeed(10);
  }
}

void changeColor(uint8_t r, uint8_t g, uint8_t b) {
  //ws2812fx.setColor(r,g,b);
}

