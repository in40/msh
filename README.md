# msh
Moonshine Automation

Requires filesystem based temperature sensor(s) and switch(es). Currently tested with 1wire OWFS implementation.

You will require to update your temperature sensor file location in settings.txt file, and set your relay/switch file location.
There is also some other settings , like PID regulator integration period - it set to 10 seconds, what I found is fine for 1wire thermometers. If your thermometers is slower/faster - you may need to increas/decrease this. 
MSH now logs all values at the end of each cycle into local SQLite3 database for future reference.

Problems resolution:
If it crashing with Segmentation fault, it means in 99% of times that thermometer file is not accessible, so double check.
If it comlaining about ncurses during compilation, try to install libncurses-devel package, as it requires ncurses headers.

Shortcuts:
There are some shortcuts to update temperature settings during runtime
Arrows Up, Down, Left, Right - increase/decrease set temperature
Numbers from 1 to 9 set some preret head, body anbd tail temperatures.
D - disable/enable delay after each cycle. Without delay MSH will try to read temperatures and write switch sequence as quickly as possible (limited only by speed of your sensors and switches), but will take more CPU resources. 

Any questions - feel free to ask :)
Samogon
