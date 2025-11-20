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

using namespace std;
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
int main()
{

    ifstream inputFile("Swedish.dic");
    ofstream outputFile("swedish_curated.dic");

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cerr << "Could not open the input or output file!" << std::endl;
        return 1;
    }

    std::string line;

    while (getline(inputFile, line))
    {
        string word, details;
        istringstream stream(line);

        if (std::getline(stream, word, '/') && std::getline(stream, details))
        {
            int count = 0;
            for (int i = 0; word[i]; i++)
                if (word[i] != ' ')
                    word[count++] = word[i];
            // Check if the word has exactly 5 letters

            // Write the word and details to the new file
            outputFile << word << std::endl;
        }
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Filtering complete. Check 'new_words.dic' for the result." << std::endl;

    return 1;
}