## June 29: Buzzing noise and no motor movement

Ran into an issue where there was an audible buzzing, and no motor movement when expected with input. After some tinkering, I discovered that the PWM (pulse-width modulation) set at 90 was too low to overcome static friction, and the default frequency of 1kHz was causing the motor windings to vibrate and a frequency audible to the human ear. I adjusted the PWM frequency to 140 and this solved the issue.

I now have a bug where one motor functions perfectly as expected, but the other is not responding to any inputs. I have two dual motor driver boards, so i tested between the two. I got slightly different results, but neither perfectly drove both motors. I still have to go through some more debugging, but at this point I am suspicious of the motor driver board.

## July 7: Only one motor recieving power

Debugging extended video: [DriveWire V1 Debugging - Motor Power Issues](https://youtu.be/DzpZYJx54m0?si=vVjL2RTWoly1Th7c)

In previous tests, I confirmed that both motors work, and that the 22 AWG automotive wire and soldered joints to the motor terminals also work.

I believed the issue was related to the dual motor driver board. To isolate the issue, I decided to first swap the imputs to the motor driver, and see what behavior changes. Then swap the outputs and see what behavior changes. This will determine if the issue is with the signals getting to the board from the ESP32, or if the issue happens within the board, affecting the output.

Before beginning the debugging process, only the left motor was working. But it's polarity was backwards to what the firmware was commanding. Upon swapping the inputs, all that changed was the polarity of the left motor. The right motor remained off. This is expected behavior if the inputs are working. So now the focus falls on the output.

I swapped the output, and the left motor would no longer work, but the right motor now started working with reversed polarity. This confirms the issue is within the board.

I inspected the board carefully, but I can't see a reason for this to be happening. Maybe an issue with the H bridge. There are no solder bridges or anything like that.

For now, I added in the backup dual motor driver board which also only has one fully functional output, and wired it in parallel to the other motor board. They both share the same VCC battery voltage (~6.24 V currently) in parallel. And common grounds are shared. One board now drives one motor, and the other drives the other.

Both wheels are now spinning, and the forward, reverse and pulse commands all function exactly as intended.

### New bug discovered: 
The turning commands on the other hand are producing strange results. The right turn command, R, spins the left wheel backward (seems polarity is flipped with this one) but the right wheel also goes backward. Then for a left turn, the right wheel goes backward still (this is correct, but no change in direction between L and R commands is very odd), and the left wheel goes forward, which again indicates the left wheel logic is simply wrong polarity, but otherwise working. This is the next thing to debug.
