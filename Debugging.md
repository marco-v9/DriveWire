## June 29: Buzzing noise and no motor movement

Ran into an issue where there was an audible buzzing, and no motor movement when expected with input. After some tinkering, I discovered that the PWM (pulse-width modulation) set at 90 was too low to overcome static friction, and the default frequency of 1kHz was causing the motor windings to vibrate and a frequency audible to the human ear. I adjusted the PWM frequency to 140 and this solved the issue.

I now have a bug where one motor functions perfectly as expected, but the other is not responding to any inputs. I have two dual motor driver boards, so i tested between the two. I got slightly different results, but neither perfectly drove both motors. I still have to go through some more debugging, but at this point I am suspicious of the motor driver board.

## July 7: Only one motor receiving power

Debugging extended video: [DriveWire V1 Debugging - Motor Power Issues](https://youtu.be/DzpZYJx54m0?si=vVjL2RTWoly1Th7c)

During previous tests, I confirmed that both DC motors function correctly, and that the 22 AWG automotive wire and soldered motor terminal connections are not the source of the issue.

The suspected failure point was the dual motor driver board. To isolate the issue, I tested whether the fault followed the ESP32 control inputs or stayed with the motor driver output channel.

Before debugging, only the left motor was receiving power. However, its polarity was reversed relative to the firmware command.

First, I swapped the motor driver input signals. After this change, the left motor still ran, but its polarity changed. The right motor remained off. This suggested that the ESP32 control signals were not the main issue, since changing the input mapping affected the working motor but did not restore the inactive motor.

Next, I swapped the motor outputs. After swapping the outputs, the left motor no longer worked, while the right motor began working with reversed polarity. This confirmed that the fault stayed with one output channel of the motor driver board, rather than following the motor, wiring, or ESP32 control signals.

I inspected the driver board visually, but did not find any obvious solder bridges or damaged connections. The issue may be internal to the H-bridge or motor driver IC.

As a temporary workaround, I added the backup dual motor driver board. This second board also appears to have only one fully functional output channel, so each board is now driving one motor using its working channel. Both boards are powered from the same motor supply rail, which was approximately 6.24 V during testing, and both boards share a common ground with the ESP32.

With this setup, both wheels now spin correctly, and the forward, reverse, and pulse commands function as intended.

### New bug discovered: 
The turning commands are currently producing incorrect behavior.

For the right turn command, R, the left wheel spins backward, which suggests its polarity or logic is inverted. However, the right wheel also spins backward.

For the left turn command, L, the right wheel still spins backward, while the left wheel spins forward. The left wheel behavior appears to be an inverted polarity issue, but the right wheel not changing direction between L and R suggests that there may also be a firmware mapping or command logic issue.

The next debugging step is to verify the motor direction mapping in firmware and create a simple truth table for each command, showing the expected and actual direction of each wheel.

| Command | Expected Left | Expected Right | Actual Left | Actual Right |
| ------- | ------------- | -------------- | ----------- | ------------ |
| F       | forward       | forward        | forward     | forward      |
| B       | backward      | backward       | backward    | backward     |
| L       | backward      | forward        | forward     | backward     |
| R       | forward       | backward       | backward    | backward     |

