# Black Otters Control Panel

[![version](https://img.shields.io/github/v/release/Racailloux/ControlPanel?style=for-the-badge)](https://github.com/Racailloux/ControlPanel/releases)
[![License](https://img.shields.io/github/license/Racailloux/ControlPanel?style=for-the-badge&color=blue)](https://github.com/Racailloux/ControlPanel/blob/main/LICENSE)
[![Discord](https://img.shields.io/discord/395260138493050890?style=for-the-badge)](http://discord.gg/ZF4aW5X)
[![Black Otters](https://img.shields.io/static/v1?style=for-the-badge&label=Corpo&message=BlackOtters&color=C18155)](https://www.blackotters-corporation.com/)


Here is a Arduino micro solution for custom Control Panel developped for Black Otters corporation for Star Citizen.


## Contents

### 3D printed case
* Fusion 360 3D models
* Exported files in STL

### Controls
Multiple controls optimized for Star Citizen usage (multi role : Flight, Fight & Mining)
* 1 Axis for Mining power
* 1 encoder for Throttle
* 39 Buttons :
  * Backlight with led power controlled (potentiometer on the right of the case controls the LED powers)
  * color coded lights : Red : Emergency / Yellow : Mode change / White : Reset / Green : Special buttons / Blue : Standard buttons

### Sticker
Template of sticker in SVG for front panel usage : Texts, logo, ...

### Software
Program of arduino Micro with 5 modules PCF8574
* USB Controlled Joystick mode
* Encoder & Axis management
* Analog input to control PWM lighting level of LEDs

### Star Citizen Layout
 * Layout to be added in Star Citizen (for keyboard users only, Multiple Joysticks need a parameterized installation)


## Credits
[Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary) : Arduino library to simulate a Joystick behavior.<br>
[PCF8574 Library](https://github.com/xreef/PCF8574_library) : Arduino Library to manage chipset PCF8574.<br>


