## June 29

Ran into an issue where there was an audible buzzing, and no motor movement when expected with input. After some tinkering, I discovered that the PWM (pulse-width modulation) set at 90 was too low to overcome static friction, and the default frequency of 1kHz was causing the motor windings to vibrate and a frequency audible to the human ear. I adjusted the PWM frequency to 140 and this solved the issue.

I now have a bug where one motor functions perfectly as expected, but the other is not responding to any inputs. I have two dual motor driver boards, so i tested between the two. I got slightly different results, but neither perfectly drove both motors. I still have to go through some more debugging, but at this point I am suspicious of the motor driver board.
