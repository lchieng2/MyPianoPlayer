# My Piano Player

My Piano Player is a virtual piano that you can play using the keys of your computer.

## Installation

My Piano Player uses the CMake build. Simply load in this project
and make sure to link any necessary libraries.

### Usage

To use My Piano Player, there is a given set of 17 keys (10 white and 7 black) that correspond
to piano keys displayed in the app. Hitting any of these specified keys will prompt a
sound that matches the tone of the piano note, as well as highlighting the note played on the screen.

### Modifications

If you choose to alter what computer keys are used to play the notes, follow these steps:

-Change the global variable arrays white_keys_used and black_keys_used to your desired
computer keys. For example, if you want to use the "A" key instead of the "Q" key, all you
have to do is change the "A" into "Q". 
-Next, in your KeyDown() and KeyUp() functions, change the KeyEvent cases for Q into A. This
would look something like KeyEvent::KEY_Q ---> KeyEvent::KEY_A

Sorry if this is a bit of work, this was just the easiest way for me to organize the code.

Note: The provided keys are designed to perfectly match a piano orientation so it is advised
to not even have to change it at all!

Have fun using the piano!


