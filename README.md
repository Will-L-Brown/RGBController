# RGBController
Controller for RGB Strips and RGB Fans

Based on an Arduino Pro Micro (cheap chinese clone) 

Uses the FastLED library

Pin assignments can be found in the code

Recommended to have a resistor on the data output pins to the LED strips and fans, between 220Ω and 470Ω. I use 330Ω.

# 10-channel RGB Controller Hardware

This board is a custom board designed for hooking up to 10 aRGB devices with individual control using an Arduino Pro Micro. 

The Board features 9 3-pin aRGB headers and 1 Corsair RGB header. Idea is to have up to 8 RGB fans that are each individually addressable but do not pass the line back to have a combined effect, 
while also having a chain of Corsair RGB products and an aRGB strip.

There is a 3-button header for up to 3 standard case buttons, such as the unused reset switch!

This board is in a standard 2.5 inch drive format with mounting holes for easy case mounting.

# Known Hardware issues

- Rev 0.1A 
	- Incorrect pinout on the Corsair header
	
- Rev 0.1B
	- Corsair header does not have the best routing on VCC and GND to local filter capacitors
		- SMD stencil is compatible with Rev 0.1A hardware
	- BOM P/N for aRGB headers is incorrect. Correct part should be a 2.54mm spaced, 0.6mm diameter, right angle "Swiss machine pin" header to be compatible with standard aRGB connectors
		- I don't know of a good part number or source for this part. Contact me if you have a part number, or submit a pull request
	- BOM P/N for Molex power connect is likely not optimal. 
	- Spacing for aRGB headers may not be optimal for certain devices and may have clearance issues when completely populated
	- 0.1uF caps C12-16, C27-31 should not be populated. These destroy the signal from the ATMega and were intended for 5V-GND, not Din-GND

# Future Hardware To-Do

- Rev 0.1C
	- Re-routing filter caps for the Corsair header for better performace
- Future Variants
	- Change one aRGB header to a second Corsair header
	- Variant with 3 aRGB, 2 Cosair on one side, temp probes and fan headers on the other


# Off-The-Shelf components and LED Type
This is meant to use a variety of standard PC components as well as cheap RGB strips to produce the RGB effect.

A small list of various verified parts and their LED type are listed below:

- Corsair HD1x0 fans
    - WS2812B-based custom RGB modules
	- HD120: 12 LEDS
- Corsair XD5 Pump and Res Combo
    - WS2812B-based custom RGB modules
	- 10 LEDS
- Corsair XG7 RTX 2080 GPU Block
	- WS2812B-based custom RGB modules
	- 16 LEDS
