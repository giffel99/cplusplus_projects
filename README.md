# This repo acts as an experimentation repo for me to practice my C++ skills

## A simple worlde you run in the terminal.

This program is meant to be played in the terminal. It is also meant to have internet connection as the words chosen are coming from an open API.

To run the program run this you need to compile first using: `g++ ordel.cpp -o ordel.exe` and to run it run: `ordel.exe` in the terminal.

This program contains a list of all five letter swedish words and randomly selects one of them, for each game.

## A simple hangman you run in the terminal.

This program is meant to be played in the terminal. It is also meant to have internet connection as the words chosen are coming from an open API.

To run the program run `hangman.exe` in the terminal. If needed to recompile, run: `g++ hangman.cpp -o hangman.exe`

The reason for calling an API and not storing a list of words here was to learn how to make API calls in C++.
The api used originates from here: [https://random-word-api.herokuapp.com](https://random-word-api.herokuapp.com)

Docs originated from code, copied here.

```
// #include <curlpp/Options.hpp>
/*
 Structure of the game
     Ask player how long the word should be (returns an int word_len)
     Go through the english wordbook to find a word with that length.
     Start the game
     While game is not over:
       print the board
       ask user to type a letter
       check if valid: if true then highlight it
       If invalid: Make player loose a life
       If word is complete: Then victory
       If no life left then game over
*/

/*
GAME UI

_____Hangman_____
b a n a n a
Guessed letters: abn
 __
/  o
|  ^
|  ^
/\
_________________

*/
```
