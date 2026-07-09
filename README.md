# PITCHPLAYER
Perfect pitch or utterly tone deaf? Find out with the Pitch Player :)!

There are three difficulties consisting of pitches ranging from C3 (~131Hz) to C7 (~2093Hz). Adjust the potentiometer to match that pitch after the buzzer plays a demo. The LEDs will let you know how well you’ve done. 
<img width="4032" height="3024" alt="pitchgame" src="https://github.com/user-attachments/assets/d49672c8-6ec7-41b0-a315-caeb3287b774" />

# FEATURES
- Arduino UNO R3
- 4 result LEDs (red, yellow, green, blue)
- 1 progress LED
- 3 countdown/difficulty LEDs
- Submit button
- Difficulty change button
- Potentiometer

# HOW TO PLAY
When starting the game, it will ask you to choose a difficulty level. Use the red button to toggle between the three difficulties. 
D1: range c3 to c5, 6 seconds, demo plays 2 times
D2: range c3 to c6, 6 seconds, demo plays 1 time
D3: range c3 to c7, 3 seconds, demo plays 1 time

After selecting a difficulty, press the submit button (black) to confirm. The game will start with a 3 second countdown before the demo note plays. After the note finishes playing, you can start adjusting the potentiometer to try and match the demo note. The coundown LEDs will display a timer. 

Once you're sure of a choice, click the black button again to submit. A result LED will light up depending on how well you did.
Blue: 15 allowed error
Green: 50 allowed error
Yellow: 150 allowed error
Red: Over 150 away from answer

At anytime during selection, hold the red button for 1 second to reselect the difficulty level.
