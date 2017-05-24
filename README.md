# GoButton

GoButton is a simple Arduino sketch that uses the Open Sound Control protocol to turn a physical switch into a GO button for an Eos family lighting console.

This project was created for Actors Theatre of Louisville's [2016 production](https://actorstheatre.org/shows/4000-miles-2015-2016/ "4000 Miles") of _4000 Miles_ by Amy Herzog. It allowed actors to use physical switches on a prop lamp and a scenic wall to trigger lighting cues.

## Dependencies

The sketch uses the OSC implementation for Arduino available at [https://github.com/CNMAT/OSC](https://github.com/CNMAT/OSC).

## Usage

This version of the sketch allows up to 4 physical switches. Every switch sends a simple "GO" command to the Eos console. The command is currently triggered any time the switch changes state (so switch 'on' and switch 'off' both just send `/eos/key/go 0`).

This sketch also assumed the use of an RGB LED for visual feedback.

---

John Newman, 2017.05.24
