# USB-Whisperer
Identify which values are affected by an HID's buttons


     -- Using irrlicht and hidapi  --
This is a little project that lists the connected HIDs and prompts the user to select one
by inputting its VID and PID in the following format: "vid pid" (separated by a space).
(The user could also type "q" and end the program)
If the user has entered a valid VID and PID that both match a currently connected HID,
then the selected HID will be monitored and a set of values will be displayed. If the controller
changes any value (such as by button press), then the change will be reflected in the text on-screen.

Controls:
[PAGE_UP] / [mouse wheel scrolled up] --> change current "page" of displayed values to lesser indices.
[PAGE_DOWN] / [mouse wheel scrolled down] --> change page to greater indices.
[ESC] --> terminate program
[SPACE] --> print all 65 buffers to console; good for comparing momentary snapshots of values


How to use:
Enter a matching VID and PID from the list of devices and look on the right side for a change in
value after pressing/releasing a button or moving an analog control. It will display which bits have
changed.



What it's good for:
It's great for identifying which values correlate with the state of which button on a given connected
device. Then, you know which values to check for when using that device via hidapi.



Using this program, I have successfully been able to map out the values corresponding to the buttons
of several controllers. The resulting mappings are in the ressystem/ subdirectory.

The controllers I mapped are:

PS2 wired controller (connected via USB-to-PS2_controller connector)
(the rest of the HIDs were connected directly via USB)
PS3 wired controller
PS4 wiresless controller connected via USB charging cable
PC/PS3 steering wheel controller (+ pedals)
Logitech 3D Extreme flight stick controller
Logitech G Saitek Pro Flight Switch Panel (i managed to find all switch/knob/lever values but not LEDs)
Xbox 360 wired controller


*conversion from integer to string representing binary value of that integer may be inefficient

**Some HIDs (keyboard, mouse, tablet) will be listed but, if selected, will not show any change
in any values. I think this is because the operating system uses drivers that first intercept and
interpret these devices... but the real, technical reason is out of the scope of my current knowledge.
