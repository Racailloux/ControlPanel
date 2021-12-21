# Black Otters Control Panel

[![version](https://img.shields.io/github/v/release/Racailloux/ControlPanel?style=for-the-badge)](https://github.com/Racailloux/ControlPanel/releases)
[![License](https://img.shields.io/github/license/Racailloux/ControlPanel?style=for-the-badge&color=blue)](https://github.com/Racailloux/ControlPanel/blob/main/LICENSE)
[![Discord](https://img.shields.io/discord/395260138493050890?style=for-the-badge)](http://discord.gg/ZF4aW5X)
[![Black Otters](https://img.shields.io/static/v1?style=for-the-badge&label=Corpo&message=BlackOtters&color=C18155)](https://www.blackotters-corporation.com/)

<br><br>
[![French](https://raw.githubusercontent.com/yammadev/flag-icons/master/png/FR.png) Section Française](#french-section) - 
[![English](https://raw.githubusercontent.com/yammadev/flag-icons/master/png/GB.png) English Section](#english-section)
<br><br>


<p align="center"><img src="https://cloud.sheepy.fr/index.php/apps/files_sharing/publicpreview/8WFDLJjTbtgygz2?x=1920&y=588&a=true&file=ControlPanel.jpg"  alt="Control Panel" /></p>

# French section

## Introduction

Ceci est un projet de panneau de contrôle personnalisé développé pour la corporation de la [Black Otters](https://www.blackotters-corporation.com/) du jeu [Star Citizen](https://robertsspaceindustries.com/star-citizen).

## Contenu

### Boitier imprimé 3D
* Modèles en 3D au format Fusion 360
* Fichiers STL exportés en STL

### Contrôles
Multiples contrôles optimisés pour l'utilisation dans Star Citizen (multi rôles : Vol, Combat & Minage)
* 1 Axe pour la puissance du minage
* 1 codeur pour encoder for le niveau des gaz
* 39 Boutons :
  * Boutons rétroéclairés par LEDs avec la luminosité ajustable (le potentiomètre sur la droite du boitier contrôle la luminosité)
  * Code couleur des boutons : Rouge : Urgencesy / Jaune : Changement de mode / Blanc : Réinitialisation / Vert : Boutons spéciaux / Bleu : Boutons standards

### Autocollant
Un autocollant modèle au format SVG pour placer sous les contrôles sur la façade avant : Textes, logo, ...

### Logiciel
Programme pour Arduino Micro relié à 5 modules d'extension PCF8574
* Mode Joystick contrôlé par USB
* Gestion de l'axe et du codeur
* Entrée analogique pour contrôler l'éclairage  des LEDs en PWM

### Association des contrôles pour Star Citizen
 * Fichier d'association (Layout) des contrôles pour l'importation dans Star Citizen (Utile pour les utilisateurs au clavier/souris, La gestion de multiple joysticks nécessitant une configuration adaptée)


## Credits

### Libraires Arduino

[Libraire Joystick](https://github.com/MHeironimus/ArduinoJoystickLibrary) : Libraire Arduino pour simuler le comportement d'un Joystick .<br>
[Libraire PCF8574](https://github.com/xreef/PCF8574_library) : Libraire Arduino pour gérer les puces PCF8574.<br>

### Sources d'inspiration

[Custom Starcitizen control panels](https://robertsspaceindustries.com/community/citizen-spotlight/3330-Custom-Starcitizen-Control-Panels)<br>
[Star Citizen Custom Controller](https://shinythinks.com/project/star-citizen-custom-controller/)<br>
[MAXED - Custom Star Citizen hardware. mk1 prototype](https://www.reddit.com/r/starcitizen/comments/9jc5qt/maxed_custom_star_citizen_hardware_mk1_prototype/)<br>


# English section

## Introduction

This is a project for custom Control Panel developped for [Black Otters](https://www.blackotters-corporation.com/) corporation in the [Star Citizen](https://robertsspaceindustries.com/star-citizen) game.

## Contents

### 3D printed case
* Fusion 360 3D models
* Exported files in STL

### Controls
Multiple controls optimized for Star Citizen usage (multi roles : Flight, Fight & Mining)
* 1 Axis for mining power
* 1 encoder for Throttle
* 39 Buttons :
  * Backlight with led power adjustable (potentiometer on the right of the case controls the LEDs power)
  * color coded buttons : Red : Emergency / Yellow : Mode change / White : Reset / Green : Special buttons / Blue : Standard buttons

### Sticker
Template sticker in SVG to be placed below front panel controls : Texts, logo, ...

### Software
Program of arduino Micro with 5 extension PCF8574 modules
* USB Controlled Joystick mode
* Encoder & Axis management
* Analog input to control PWM lighting level of LEDs

### Star Citizen Layout
 * Layout of the control panel to be imported in Star Citizen (for keyboard/mouse users only, multiple joysticks need a specific configuration)

## Credits

### Arduino Libraries

[Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary) : Arduino library to simulate a Joystick behavior.<br>
[PCF8574 Library](https://github.com/xreef/PCF8574_library) : Arduino Library to manage PCF8574 chipset.<br>
[Tools40](https://github.com/IndustrialShields/arduino-Tools40) : Arduino libraries used for debounce features.<br>

### Inspiration sources

[Custom Starcitizen control panels](https://robertsspaceindustries.com/community/citizen-spotlight/3330-Custom-Starcitizen-Control-Panels)<br>
[Star Citizen Custom Controller](https://shinythinks.com/project/star-citizen-custom-controller/)<br>
[MAXED - Custom Star Citizen hardware. mk1 prototype](https://www.reddit.com/r/starcitizen/comments/9jc5qt/maxed_custom_star_citizen_hardware_mk1_prototype/)<br>
