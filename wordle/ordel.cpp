#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <tuple>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <random>
#include <unordered_set>
#include <cstdlib>
using namespace std;
/*
 GAME IDEA

Get a 5 letter word.
Prompt user to enter a valid word. => we check if this is a valid word
=> If not valid word, then we say "Not a word"
=> If valid word, run ValidateInput(), and print the likeness
=> If word is correct => you win
=> if 5 guesses done => game over
*/
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string DEFAULT = "\033[0m";
const std::string BOX = "\u25A2";

vector<string> previousGuesses;
string wordToGuess;
string inputWord;
int numGuesses;
int guessesLeft;
/*
COLORS:
GREEN:      \033[32m
YELLOW:     \033[33m
DEFAULT:    \033[0m

UNICODE:

BOX: \u25A2

*/
bool isTwoByteUtf8Char(const std::string &str, size_t pos)
{
    if (pos >= str.size())
    {
        return false; // Out of bounds
    }

    unsigned char firstByte = str[pos];

    // Check if the first byte indicates a 2-byte character
    if ((firstByte & 0xE0) == 0xC0)
    {
        // Make sure the second byte is within the valid range for UTF-8
        if (pos + 1 < str.size() && (str[pos + 1] & 0xC0) == 0x80)
        {
            return true;
        }
    }

    return false;
}

size_t utf8_strlen(const std::string &str)
{
    size_t length = 0;
    for (size_t i = 0; i < str.size();)
    {
        if ((str[i] & 0x80) == 0)
        { // 1-byte character (ASCII)
            i += 1;
        }
        else if ((str[i] & 0xE0) == 0xC0)
        { // 2-byte character
            i += 2;
        }
        else if ((str[i] & 0xF0) == 0xE0)
        { // 3-byte character (common for å, ä, ö in UTF-8)
            i += 3;
        }
        else if ((str[i] & 0xF8) == 0xF0)
        { // 4-byte character
            i += 4;
        }
        length++;
    }
    return length;
}
// Function to merge vector of strings into a single string
std::string mergeVectorToString(const std::vector<std::string> &vec)
{
    std::ostringstream oss;
    for (const auto &str : vec)
    {
        oss << str;
    }
    return oss.str();
}
std::string getMissingLetters(const std::string &input)
{
    // Define the full alphabet including Swedish characters
    const std::string fullAlphabet = "abcdefghijklmnopqrstuvwxyzåäö";

    // Use a set to track characters in the input string
    std::unordered_set<char> inputChars(input.begin(), input.end());

    // Create a result string to store missing characters
    std::string result;

    // Iterate through the full alphabet and add missing characters to result
    for (int i = 0; i < fullAlphabet.length(); i++)
    {
        if (
            isTwoByteUtf8Char(fullAlphabet, i))
        {
            std::string s = std::string(1, fullAlphabet[i]) + fullAlphabet[i + 1];

            if (input.find(s) == string::npos)
            {
                result += s;
                i++;
            }
        }

        else if (inputChars.find(fullAlphabet[i]) == inputChars.end())
        {

            result += fullAlphabet[i];
        }
    }

    return result;
}

void PrintBoard(string language, int wordLen, int numGuesses, int guessesLeft, vector<string> previousGuesses)
{

    system("clear");
    cout << "Wordle in: " << language << endl;

    // Print the guessed letters
    for (int i = 0; i < numGuesses - guessesLeft; i++)
    {
        cout << previousGuesses[i] << endl;
    }
    // Print lives left
    for (int i = 0; i < guessesLeft; i++)
    {
        for (int j = 0; j < wordLen; j++)
        {
            cout << BOX;
        }
        cout << endl;
    }

    // Print the used letters
    // QWERTYUIOPÅ
    // ASDFGHJKLÖÄ
    // ZXCVBNM
    string mergedGuesses = mergeVectorToString(previousGuesses);
    cout << "Used letters: " << mergedGuesses << endl;
    cout << "Not used letters: " << getMissingLetters(mergedGuesses) << endl;
}

void ValidateWord(string inputWord, string wordToGuess, vector<string> &previousGuesses)
{
    string coloredGuess = "";
    int j = 0;
    for (int i = 0; i < wordToGuess.length(); i++)
    {
        // Ifall det är 2 bytes => ÅÄÖ
        if (isTwoByteUtf8Char(inputWord, i))
        {

            // Check if they are correct position. COLOR GREEN
            if (wordToGuess[i] + wordToGuess[i + 1] == inputWord[j] + inputWord[j + 1])
            {
                coloredGuess += GREEN + inputWord[j] + inputWord[j + 1];
            }
            // Check if the character is in wrong place. COLOR YELLOW
            else if (wordToGuess.find(inputWord[j] + inputWord[j + 1]) != std::string::npos)
            {
                coloredGuess += YELLOW + inputWord[j] + inputWord[j + 1];
            }
            else
            {
                coloredGuess += DEFAULT + inputWord[j] + inputWord[j + 1];
            }
            j += 2;
            if (isTwoByteUtf8Char(wordToGuess, i))
            {
                i++;
            }
            continue;
        }

        // Check if they are correct position. COLOR GREEN
        if (wordToGuess[i] == inputWord[j])
        {
            coloredGuess += GREEN + inputWord[j];
        }
        // Check if the character is in wrong place. COLOR YELLOW
        else if (wordToGuess.find(inputWord[j]) != std::string::npos)
        {
            coloredGuess += YELLOW + inputWord[j];
        }
        else
        {
            coloredGuess += DEFAULT + inputWord[j];
        }
        j++;

        if (isTwoByteUtf8Char(wordToGuess, i))
        {
            i++;
        }
    }
    coloredGuess += DEFAULT;
    previousGuesses.push_back(coloredGuess);
}

string GetSwedishWord(int wordLen)
{

    int randomInt;
    int threshold = 90;
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist100(0, 100);

    ifstream file("swedish_curated.dic");

    if (!file.is_open())
    {
        cerr << "Could not open the file!" << endl;
    }
    string line;
    while (getline(file, line))
    {
        istringstream stream(line);
        randomInt = dist100(rng);

        line[0] = tolower(line[0]);

        if (utf8_strlen(line) == wordLen)
        {
            if (randomInt >= threshold) // threshold = 100
            {

                file.close();
                return line;
            }
            threshold--;
        }
    }
    file.close();

    return "fritt";
}

string GetEnglishWord(int wordLen)
{
    return "hinge";
}

bool CheckIfValidWord(string inputWord, int wordLen)
{
    int inputLen = utf8_strlen(inputWord);
    // Kolla om längden är över 5 chars och om det finns ett åäö i texten
    if (inputLen > wordLen)
    {
        cout << "Word is too long." << endl;
        return false;
    }
    else if (inputLen < wordLen)
    {
        cout << "Word is too short." << endl;
        return false;
    }

    ifstream file("swedish_curated.dic");

    if (!file.is_open())
    {
        cerr << "Could not open the file!" << endl;
    }
    string line;
    while (getline(file, line))
    {
        line[0] = tolower(line[0]);

        if (inputWord == line)
        {

            return true;
        }
    }
    // Check if a valid word
    cout << "Not a valid word!!!!" << endl;
    return false;
}

void DEBUG()
{
    cout << "DEBUG" << endl;
    cout << wordToGuess << endl
         << guessesLeft << endl;
    for (string guess : previousGuesses)
    {
        cout << guess << endl;
    }
}

tuple<bool, string> Runwordle(string language, int wordLen)
{

    // Ask how many lives you want
    numGuesses = 9;
    guessesLeft = 9;
    // Word to guess
    if (language == "swedish")
    {
        wordToGuess = GetSwedishWord(wordLen);
    }
    else
    {
        wordToGuess = GetEnglishWord(wordLen);
    }
    // Init board
    PrintBoard(language, wordLen, numGuesses, guessesLeft, previousGuesses);

    // Game Loop
    for (int i = 0; i < numGuesses; i++)
    {
        guessesLeft--;
        cin >> inputWord;

        while (!CheckIfValidWord(inputWord, wordLen))
        {
            cin >> inputWord;
        }

        // We check the potential errors and add the new word to the list.
        ValidateWord(inputWord, wordToGuess, previousGuesses);

        // Print a new board with the new guess applied
        PrintBoard(language, wordLen, numGuesses, guessesLeft, previousGuesses);

        // After we calculate we check if it was the right word. If so we end the game.
        if (inputWord == wordToGuess)
        {
            return {true, wordToGuess};
        }
        // print statements
        // DEBUG();
    }
    return {false, wordToGuess};
}

int main(int argc, char **argv)
{

    string language = argv[1];   // First argument is "english" or "swedish"
    int wordLen = atoi(argv[2]); // Second argument is a number that is cast to an int

    setlocale(LC_ALL, "sv_SE.UTF-8"); // Uses the system locale

    auto [wasGameWon, wordToGuess] = Runwordle(language, wordLen);

    if (wasGameWon)
    {
        cout << "You won!" << endl;
    }
    else
    {
        cout << "You lost. The word was: " << wordToGuess << endl;
    }

    return 0;
}