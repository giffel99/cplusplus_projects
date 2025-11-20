#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>

using namespace std;

// #include <curlpp/Options.hpp>
/*
 Structure of the game
     Ask player how long the word should be (returns an int word_len)
     Go through the english wordbook to find a word with that length. (using an api call maybe?)
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

int word_len;
int lives = 9;
string incompleteWord;
string wordToGuess;
string guessedLetters;
bool isGameOver;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *userp)
{
    userp->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void PrintLives(int lives)
{
    /*
     __
    /  o
    |  ^
    |  ^
    /\
    */

    string tower_0;
    string tower_1;
    string tower_2;
    string tower_3;
    string tower_4;

    if (lives <= 8)
    {
        tower_4 = "/";
    }
    if (lives <= 7)
    {
        tower_4 = "/\\";
    }

    if (lives <= 6)
    {
        tower_3 = "|";
    }
    if (lives <= 5)
    {
        tower_2 = "|";
    }

    if (lives <= 4)
    {
        tower_1 = "/";
    }

    if (lives <= 3)
    {
        tower_0 = " __";
    }
    if (lives <= 2)
    {
        tower_1 = "/  o";
    }

    if (lives <= 1)
    {
        tower_2 = "|  ^";
    }
    if (lives <= 0)
    {
        tower_3 = "|  ^";
    }

    cout << tower_0 << "\n";
    cout << tower_1 << "\n";
    cout << tower_2 << "\n";
    cout << tower_3 << "\n";
    cout << tower_4 << "\n";
}

void PrintOutboard(string incompleteWord, string guessedLetters, int lives)
{
    system("clear");
    // print the words
    cout << "\n_____Hangman_____\n";
    for (int i = 0; i < incompleteWord.size(); i++)
    {
        cout << incompleteWord[i] << " ";
    }

    cout << "\nGuessed letters: " << guessedLetters << "\n";
    PrintLives(lives);
    cout << "\n_________________\n";
}

string UpdateIncompleteWord(string wordToGuess, string incompleteWord, char letter)
{
    for (int i = 0; i < wordToGuess.size(); i++)
    {
        if (letter == wordToGuess[i])
        {
            incompleteWord[i] = letter;
        }
    }
    return incompleteWord;
}

void Debug()
{
    cout << lives << "\n"
         << wordToGuess << "\n"
         << incompleteWord << "\n"
         << guessedLetters << "\n";
}
string GetRandomWord(int word_len)
{
    CURL *curl;
    CURLcode res;
    string readBuffer;

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {

        string url = "https://random-word-api.herokuapp.com/word?number=1&length=" + to_string(word_len);
        // Set the URL to the API
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to store the response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Pass a pointer to the string to save the response
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the GET request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        else
        {
            if (!readBuffer.empty())
            {
                readBuffer.erase(0, 2);                  // Remove the first character '['
                readBuffer.erase(readBuffer.size() - 2); // Remove the last character ']'
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }
    else
    {
        cout << "Unable to fetch word.";
        return "banana";
    }

    // Clean up global curl environment
    curl_global_cleanup();


    // https://random-word-api.herokuapp.com/word?number=1&length=4
    return readBuffer;
}
void Hangman()
{

    char letter;
    bool wasCorrectGuess;
    isGameOver = false;

    // Ask player how long the word should be:
    cout << "\nWelcome to hangman.\nHow long do you want you word to be between 4-10?\n";
    cin >> word_len;

    // Find a word with that length
    wordToGuess = GetRandomWord(word_len);
    incompleteWord = string(word_len, '_');
    // Run game loop
    while (!isGameOver)
    {
    game_loop:

        // start the game
        PrintOutboard(incompleteWord, guessedLetters, lives);

        letter = ' ';

        // Debug
        // Debug();

        // Ask user for a letter
        cout << "Enter a letter: ";
        cin >> letter;
        if (guessedLetters.find(letter) != string::npos)
        {
            cout << "\nYou have already guessed: " << letter << ". Pick another one\n";
            // run loop again
            goto game_loop;
        }

        // Check if letter is valid
        wasCorrectGuess = (wordToGuess.find(letter) != string::npos);
        // If guess was correct then we update the incomplete word
        if (wasCorrectGuess)
        {
            cout << "CORRECT GUESS\n";
            incompleteWord = UpdateIncompleteWord(wordToGuess, incompleteWord, letter);
        }
        // The letter was incorrect, we loose a life
        else
        {
            cout << "INCORRECT GUESS\n";

            lives--;
        }
        // We add the letter to list of guessed letters
        guessedLetters += letter;

        // We check if the game is over
        if (lives <= 0 || incompleteWord == wordToGuess)
        {
            cout << "CODE: Game is over";
            isGameOver = true;
        }
    }

    // Game is over we check if we won.
    if (lives <= 0)
    {
        PrintOutboard(incompleteWord, guessedLetters, lives);

        cout << "Game over, you lost.\n The correct word was: " << wordToGuess << "\n";
    }
    else
    {
        PrintOutboard(incompleteWord, guessedLetters, lives);

        cout << "You won! You guessed the right word: " << wordToGuess << "\n";
    }
}

int main()
{
    // cout << "The word is: " << GetRandomWord(5) << "\n";
    Hangman();
    return 0;
}