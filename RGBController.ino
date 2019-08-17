#include "FastLED.h"

FASTLED_USING_NAMESPACE

/* RGB Controller for PCs
 *  Developed at some point from some example code
 *  Don't really know what this does but hey it works
 *  
 *  Be careful
 *  
 *  William
 *  Aug 2019
 */

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif



/* In my install, LED1 is my pump and fans, LED2 is my RGB strip
 *  Keep that in mind going through the code
 */
#define DATA_PIN    5
#define DATA_PIN_2  10

#define LED_TYPE    WS2812
#define LED_TYPE_2    WS2812

#define COLOR_ORDER GRB

#define NUM_LEDS    82 
#define NUM_LEDS2    76

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];

#define BRIGHTNESS          50
#define MIN_BRIGHTNESS      26

// Tied to the input for the case buttons
#define PATTERN_BUTTON 3
#define SPEED_BUTTON 4
#define COLOR_BUTTON 2

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


int fps = 240;

CLEDController *controllers[2];

int colorCount = 0;

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

bool advPat = false; // For button cycle of modes
bool advSpd = false; // For button cycle of modes
bool advClr = false; // For button cycle of modes

bool cycleHue = true;

void setup() {
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE_2,DATA_PIN_2,COLOR_ORDER>(leds2, NUM_LEDS2);

  // Configure button input
  pinMode(PATTERN_BUTTON, INPUT);
  pinMode(SPEED_BUTTON, INPUT);
  pinMode(COLOR_BUTTON, INPUT);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
// Defined here because fuck you?
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbow2, breathing, sinelon, sinelon2, blank, sam , test };

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED[0].showLeds(255);
  FastLED[1].showLeds(BRIGHTNESS);

  // do some periodic updates
  if (cycleHue || gCurrentPatternNumber == 5) {
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  }

  if (digitalRead(PATTERN_BUTTON) && !advPat) {
    nextPattern();
    advPat = true;
  }
  else if (!digitalRead(PATTERN_BUTTON) && advPat) {
    advPat = false;
  }

  if (digitalRead(SPEED_BUTTON) && !advSpd) {
    nextSpeed();
    advSpd = true;
  }
  else if (!digitalRead(SPEED_BUTTON) && advSpd) {
    advSpd = false;
  }

  if (digitalRead(COLOR_BUTTON) && !advClr) {
    nextColor();
    advClr = true;
  }
  else if (!digitalRead(COLOR_BUTTON) && advClr) {
    advClr = false;
  }
}

void nextColor() {  
  colorCount++;
  
  if (colorCount > 9) colorCount = 0;
  
  if (colorCount == 0) {
    cycleHue = true;
  }
  else {
    cycleHue = false;
    gHue = (colorCount - 1) * 32;
  }
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

// Not actually implemented.....
void nextSpeed()
{
  // add 20 FPS and wrap it around to 20
  fps = (fps % 240) + 20;
}

void blank() {
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = CRGB::Black;
  }
  for( int i = 0; i < NUM_LEDS2; i++) { //9948
    leds2[i] = CRGB::Black;
  }
}

// In honor of a request from an old roommate, I give you "Sam"
// Please don't judge
void sam()
{
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i].r = random(0, 255);
    leds[i].g = random(0, 255);
    leds[i].b = random(0, 255);
  }
  for( int i = 0; i < NUM_LEDS2; i++) { //9948
    leds2[i].r = random(0, 255);
    leds2[i].g = random(0, 255);
    leds2[i].b = random(0, 255);
  }

  delay(5);
}

void test()
{
  fill_rainbow( leds, NUM_LEDS, gHue, 100);
  fill_rainbow( leds2, NUM_LEDS2, gHue, 50);
}

// The following functions are pretty much copied from FastLED example code

void breathing() {
  float breath = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
  if (breath < MIN_BRIGHTNESS) breath = 0;
  fill_solid(leds, ARRAY_SIZE(leds), CHSV(gHue, 187, breath));
  fill_solid(leds2, ARRAY_SIZE(leds2), CHSV(gHue, 187, breath));
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 0);
  fill_rainbow( leds2, NUM_LEDS2, gHue, 0);
}

void rainbow2() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 5);
  fill_rainbow( leds2, NUM_LEDS2, gHue, 5);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
  if( random8() < chanceOfGlitter) {
    leds2[ random16(NUM_LEDS2) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 4);
  int pos = beatsin16(13,0,NUM_LEDS - 1);
  leds[pos] += CHSV( gHue, 255, 192);
  
  fadeToBlackBy( leds2, NUM_LEDS2, 4);
  pos = beatsin16(13,0,NUM_LEDS2 - 1);
  pos = NUM_LEDS2 - pos - 1;
  leds2[pos] += CHSV( gHue, 255, 192);
}

void sinelon2()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 4);
  int pos = beatsin16(13,0,NUM_LEDS - 1);
  leds[pos] += CHSV( gHue, 255, 192);
  pos = NUM_LEDS - pos - 1;
  leds[pos] += CHSV( 255 - gHue, 255, 192);
  
  fadeToBlackBy( leds2, NUM_LEDS2, 4);
  pos = beatsin16(13,0,NUM_LEDS2 - 1);
  leds2[pos] += CHSV( gHue, 255, 192);
  pos = NUM_LEDS2 - pos - 1;
  leds2[pos] += CHSV( 255 - gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  for( int i = 0; i < NUM_LEDS2; i++) { //9948
    leds2[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS-1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }

  fadeToBlackBy( leds2, NUM_LEDS2, 20);
  dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS2-1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
