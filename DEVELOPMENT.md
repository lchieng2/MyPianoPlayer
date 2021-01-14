# Development

---
**4/20/20** 
-
Linked necessary libraries to begin project

**4/27/20** Began work on drawing basic components
-
created a drawn piano with black and white keys placed correctly

**4/28/20** Matched each piano key to a key on a computer keyboard
-
printed out a letter or number corresponding each piano key to a keyboard key

**4/28/20** Need to complete the following next:
-
[] Link an additional library to facilitate audio output (realized my original design would be too repetitive)
[] Map each keyboard key to its corresponding sound

**4/28/20** Added .wav files for each key and mapped
-
Was able to assign each .wav file to its matching key so that when KeyDown recognizes
a key has been pressed, the sound is played. The code is very repetitive so I hope
to create helper functions to make it look less cluttered.

**5/3/20** Finished work on highighting keys as they are pressed
-
after figuring out how to use the KeyUp() function, was able to track when keys were pressed and when
they were released. Used the tracking to draw a key red if its corresponding computer key was pressed.

**5/3/20** Need to complete the following next:
-
Implementation of nlohmann json:
[] Generating a file of all names of keysignatures with their correct sharps
[] Reading the json file in so that the data can be displayed

**5/5/20** Completed writing of JSON to a file and simple reading of the file
-
The json correctly writes to the file (used unit testing to ensure accuracy) using a pattern that
music signatures follow. This allowed for the code to not be hard coded and rather systematic.
Was able to print out a simple "key" name with its corresponding "sharps" following in the app.

**5/6/20** Allowed for user interaction when displaying the key signature
-
Created a function that whenever the user hits the SPACE bar the key signature displayed changes,
looping over and over again through the possibilities given in the JSON file.