# Satisfactory efficiency terminal

![Screen image](screnn.jpg)
_*this image was taken with the french version of the code_

## The screen
This screen is a arduino all in one board called Wio terminal. If you are interested click [here](https://wiki.seeedstudio.com/Wio-Terminal-Getting-Started/).  
The arduino code is uploaded directly on it with a USB-C cable.

## The story
I wanted a screen for monitoring satisfactory using the [Ficsit Remote Monitoring](https://ficsit.app/mod/FicsitRemoteMonitoring) mod.
Unfortunately the mod wouldn't connect to the board (idk why) so I used a python script that take the data out of the server and send it to the Arduino via the Serial port.

## How to run the project
- Setup the arduino:
    - Install the required libraries:  
[LCD library](https://wiki.seeedstudio.com/Wio-Terminal-LCD-Overview/)  
    - Once done open the screen.ino file and upload it to the board (remember the port of the board)
- Setup the python script:  
    - You need to modify the port at the line 16 by the port on which is connected the arduino  
    - If you modified the mod webserver port, also change the port at line 28  
    - Line 54 is the list of the items that will be displayed. To get the name of the items, go on [your local webserver page](http://localhost:8080/) and copy some item you want. (if your game is in french you can leave it for testing, for other language this will not work)
    - Launch the python script (`python main.py` on a tyerminal)
- Install the Ficsit Remote Monitoring mod
- Launch Satisfactory and join a game (this is not optional)
- Start the web server by entering `/frmweb start`  

This should work

## Functionalities
- Display up to 8 items
- Customizable refresh rate (modify on python code)
- Efficiency percentage is colored (green when 100, more and more red as it aproach 0)
- Total item production count and color (if the item is more consumed than created, it is showed in red or else in blue)
- Almost Ficsit approved

