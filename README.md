# RGBController
Controller for RGB Strips and Corsair Fans

Based on an Arduino Pro Micro (cheap chinese clone) 
Uses the FastLED library
Pin assignments can be found in the code

Recommended to have a resistor on the data output pins to the LED strips and fans, between 220Ω and 470Ω. I use 330Ω.

# Off-The-Shelf components and LED Type
This is meant to use a variety of standard PC components as well as cheap RGB strips to produce the RGB effect.
A small list of various verified parts and their LED type are listed below:

- Corsair HD1x0 fans
    - WS2812B-based custom RGB modules
	- HD120: 12 LEDS
- Corsair XD5 Pump and Res Combo
    - WS2812B-based custom RGB modules
	- 10 LEDS
- Corsair XG7 GPU Block
	- WS2812B-based custom RGB modules
	- 16 LEDS
