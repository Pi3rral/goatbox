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

Install MIDI Library https://github.com/FortySevenEffects/arduino_midi_library
```
arduino-cli lib install "MIDI Library"
```


Then build with

```
arduino-cli compile --fqbn arduino:avr:pro  goatroller
```

## Upload

Find port where board is connected

```
arduino-cli board list
```

Upload

```
arduino-cli upload -p /dev/cu.usbserial-1420 --fqbn arduino:avr:pro  goatroller
```

## GoatRoller MIDI Implementation


### MIDI Channel

Channel: 3

### MIDI Messages

#### Program Change (PC)

|  PC Number  |  Loop 1  |  Loop 2  |  Loop 3  |   Loop 4 |
|:-----------:|:--------:|:--------:|:--------:|:--------:|
|     0       |    :x:   |    :x:   |    :x:   |    :x:   |
|     1       |    :white_check_mark:    |    :x:   |    :x:   |    :x:   |
|     2       |    :x:   |    :white_check_mark:    |    :x:   |    :x:   |
|     3       |    :white_check_mark:    |    :white_check_mark:    |    :x:   |    :x:   |
|     4       |    :x:   |    :x:   |    :white_check_mark:    |    :x:   |
|     5       |    :white_check_mark:    |    :x:   |    :white_check_mark:    |    :x:   |
|     6       |    :x:   |    :white_check_mark:    |    :white_check_mark:    |    :x:   |
|     7       |    :white_check_mark:    |    :white_check_mark:    |    :white_check_mark:    |    :x:   |
|     8       |    :x:   |    :x:   |    :x:   |    :white_check_mark:    |
|     9       |    :white_check_mark:    |    :x:   |    :x:   |    :white_check_mark:    |
|    10       |    :x:   |    :white_check_mark:    |    :x:   |    :white_check_mark:    |
|    11       |    :white_check_mark:    |    :white_check_mark:    |    :x:   |    :white_check_mark:    |
|    12       |    :x:   |    :x:   |    :white_check_mark:    |    :white_check_mark:    |
|    13       |    :white_check_mark:    |    :x:   |    :white_check_mark:    |    :white_check_mark:    |
|    14       |    :x:   |    :white_check_mark:    |    :white_check_mark:    |    :white_check_mark:    |
|    15       |    :white_check_mark:    |    :white_check_mark:    |    :white_check_mark:    |    :white_check_mark:    |

#### Control Change (CC)

CC Number needs to be 1,2,3,4 to action on loop 1,2,3,4.

Loop Action:
- Toggle: CC Value = 64
- Turn Off: CC Value < 64
- Turn On: CC Value > 64

Examples:
- CC Number = 2 and CC Value = 64: Toggle loop 2 state
- CC Number = 3 and CC Value = 0: Turn OFF loop 3
- CC Number = 3 and CC Value = 100: Turn ON loop 3
