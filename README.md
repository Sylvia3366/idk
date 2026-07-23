# P1. PITCHPLAYER
Perfect pitch or utterly tone deaf? Find out with the Pitch Player :)!

There are three difficulties consisting of pitches ranging from C3 (~131Hz) to C7 (~2093Hz). Adjust the potentiometer to match that pitch after the buzzer plays a demo. The LEDs will let you know how well you’ve done. 
<img width="4032" height="3024" alt="anotherimage" src="https://github.com/user-attachments/assets/89e7746a-6649-4664-bf93-217eb41e9123" />

FEATURES
-
- Arduino UNO R3
- 4 result LEDs (red, yellow, green, blue)
- 3 countdown/difficulty LEDs
- 3 lives LEDs
- Submit button
- Difficulty change button
- Potentiometer

HOW TO PLAY
-
When starting the game, it will ask you to choose a difficulty level. Use the red button to toggle between the three difficulties. 
- D1: range c3 to c5, 6 seconds, demo plays 2 times
- D2: range c3 to c6, 6 seconds, demo plays 1 time
- D3: range c3 to c7, 3 seconds, demo plays 1 time

After selecting a difficulty, press the submit button (black) to confirm. The game will start with a 3 second countdown before the demo note plays. After the note finishes playing, you can start adjusting the potentiometer to try and match the demo note. The coundown LEDs will display a timer. 

Once you're sure of a choice, click the black button again to submit. A result LED will light up depending on how well you did.
- Blue: 15 allowed error
- Green: 50 allowed error
- Yellow: 150 allowed error
- Red: Over 150 away from answer

At anytime during selection, hold the red button for 1 second to reselect the difficulty level.

# P2. SPACE SHOOTING
Navigate your ship through space! Avoid or destroy astroids along the way and try to survive as long as possible! Use the potentiometer to adjust the speed of the game.
<img width="4032" height="3024" alt="pewpew" src="https://github.com/user-attachments/assets/1d99bda3-ac72-455c-aa6f-2d7c1f95d720" />

FEATURES
-
- Arduino UNO R3
- 4 LEDs (red, yellow, green, blue)
- 2-axis joystick
- Potentiometer
- LCD Screen
- 1 laser button

HOW TO PLAY
-
Navigate the setting by steering the ship (>) using the joystick. Press a red button to shoot a laser to destroy astroids.

# P3. DINO JUMP
An auto dino jump mechanism using LDRs and a servo so you can sit back and watch :)

FEATURES
-
- Arduino UNO R3
- 2 LDRs
- Servo
