## June 12th

All parts came in. Assembled mechanical [chassis](https://github.com/marco-v9/DriveWire/blob/main/Chassis.md). 

## June 13th

Ran basic code on ESP32 board to show it is communicating properly with my computer. 

## June 14th

Soldered dual motor driver board, and wired basic powertrain circuitry. Ran code to spin a motor, and touched two wires against contacts. Confirmed by doing so that the motor will go forward, and reverse based on the code written. 

## June 29th

Soldered 22 AWG automotive wire to the motor terminals, and wrapped joints in heat shrink wrap. Zip tied cables as needed to reduce strain on joints and keep good organization. 

Also soldered the current sensor board and time of flight sensor board. Soldered jumper wires to the battery pack with heat shrink at the joint for easy connection to power source. Battery pack secured to underside of chassis for lower centre of gravity, and breadboard mounted to top of chassis with adhesive. 

Wired up the full circuit minus the ToF sensor, and created much more in depth code that allows for user input in order to test circuit function. 

Ran into an issue where there was an audible buzzing, and no motor movement when expected with input. After some tinkering, I discovered that the PWM (pulse-width modulation) set at 90 was too low to overcome static friction, and the default frequency of 1kHz was causing the motor windings to vibrate and a frequency audible to the human ear. I adjusted the PWM duty value to 140 and this solved the issue. 

I now have a bug where one motor functions perfectly as expected, but the other is not responding to any inputs. I have two dual motor driver boards, so i tested between the two. I got slightly different results, but neither perfectly drove both motors. I still have to go through some more debugging, but at this point I am suspicious of the motor driver board. 

<p align="center">
  <img src="Images/DriveWireStatus06.29.jpg" width="300">
</p>

# July 1 - July 3
I learned SolidWorks with the goal of 3D printing a professional demo [support stand](https://github.com/marco-v9/DriveWire/blob/main/Mechanical/Support-Stand.md) for DriveWire. I need the wheels to be suspended off the ground so that I can debug and demo properly. By July 3rd, using reference planes, sketch tools, and the rib feature, along with basic structural analysis, this is the stand that I produced: 

<p align="center">
  <img src="Images/DriveWireStandFinal.png" width="500">
</p>

Also sliced design, and started 3D printing process using black PLA on the Raise3D E2 printer. Supports off to avoid scarring small details, and a 9 hour estimated print time. 

# July 6
3D printer error occured, and print stopped early. I restarted the print and it is going well so far and is 83.5% complete. Here is an image of the progress: 

<p align="center">
  <img src="Images/stand-printing.gif" width="500" alt="DriveWire support stand 3D printing">
</p>

I noticed the bottom corners lifting slightly due to shrinkage during cooling. The part will likely be functional, but for future iterations I will explore solutions such as increased temperature on the heating bed (was 65 degrees for this print), use of brim, and other print settings to improve first layer adhesion. 

# July 7th
I was able to pick up the successful print for the stand: 

<p align="center">
  <img src="Images/DWStandPrintOutside.jpg" width="600" alt="DriveWire printed stand product">
</p>

After testing, the chassis sits perfectly mounted on the stand and seems extremely stable. This will become the testbench where I can continue the debugging and bring-up process. Next step is to debug one motor not recieving power. 

# July 8th
Began debugging the issue where one motor isn't recieving power. In previous tests, I confirmed that both motors work, and that the 22 AWG automotive wire and soldered joints to the motor terminals also work. 

I believed the issue was related to the dual motor driver board. To isolate the issue, I decided to first swap the imputs to the motor driver, and see what behavior changes. Then swap the outputs and see what behavior changes. This will determine if the issue is with the signals getting to the board from the ESP32, or if the issue happens within the board, affecting the output. 

Before beginning the debugging process, only the left motor was working. But it's polarity was backwards to what the firmware was commanding. Upon swapping the inputs, all that changed was the polarity of the left motor. The right motor remained off. This is expected behavior if the inputs are working. So now the focus falls on the output. 

I swapped the output, and the left motor would no longer work, but the right motor now started working with reversed polarity. This confirms the issue is within the board. 

I inspected the board carefully, but I can't see a reason for this to be happening. Maybe an issue with the H bridge. There are no solder bridges or anything like that. 

For now, I added in the backup dual motor driver board which also only has one fully functional output, and wired it in parallel to the other motor board. They both share the same VCC battery voltage (~6.24 V currently) in parallel. And common grounds are shared. One board now drives one motor, and the other drives the other. 

Both wheels are now spinning, and the forward, reverse and pulse commands all function exactly as intended. 

The turning commands on the other hand are producing strange results. The right turn command, R, spins the left wheel backward (seems polarity is flipped with this one) but the right wheel also goes backward. Then for a left turn, the right wheel goes backward still (this is correct, but no change in direction between L and R commands is very odd), and the left wheel goes forward, which again indicates the left wheel logic is simply wrong polarity, but otherwise working. This is the next thing to debug. 
