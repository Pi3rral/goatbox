# GoatRoller And GoatFather

## Install Arduino-cli

```
brew install arduino-cli
```

## Build

Set Environment For `libraries` To Be Found

```
export ARDUINO_DIRECTORIES_USER=~/Documents/goatbox
```

Then build with

```
arduino-cli compile --fqbn arduino:avr:pro  midi-controller
```

## Upload

Find port where board is connected

```
arduino-cli board list
```

Upload

```
arduino-cli upload -p /dev/cu.usbserial-1420 --fqbn arduino:avr:pro  midi-controller
```
