# NeTimer

## What is NeTimer?
NeTimer is an cube timer made by Arduino,trying let the price of the cube timer get lower,let the cube timer be more popular

## I want a NeTimer,what should I prepare?
* An Arduino develope board
* An LCD1602
* A tact switch
* Two touch sensor
* lots of resistor between 10K to 100K
* A toggle switch
* A breadboard
* A bundle of Dupont Line
* 40 Pin Header
* Three section AA battery box
You may also need an electric iron and some solder wire to weld the Pin Header of LCD1602 and Arduino,and an USB-TTL line to upload progran for Arduino.

## I have prepared these things so how to wire?
The wiring tutorial please view the [Wiring Tutorial](https://github.com/Nerlci/NeTimer/blob/master/wiring.md)。

## What does the timer look like
Just like this!<(￣ˇ￣)/

## OK,all done,can you tell me how to use?
Just like other timers,after turning it on，put your hands on the touch sensor,release the touch sensor when the green LED on the Arduino turned on,and the timer started to time.Put your both hands on the sensor again to stop timing.Click the tact switch to clean the time.

## How can I record my time?
Now NeTimer doesn't have the function to record the time now,there's two ways to record:
1. Use the manual input function of DCTimer,csTimer etc. to record
2. When stop timing the timer will send the time by serial with port baud rate 9600,you can write an program to record the time.