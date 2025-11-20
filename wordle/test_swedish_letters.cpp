#include <iostream>
#include <locale>
#include <iomanip>
int main()
{
    // Set the locale to UTF-8 for Swedish
    if (std::setlocale(LC_ALL, "sv_SE.UTF-8") == NULL)
    {
        std::cerr << "Locale not supported!" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Locale set successfully!" << std::endl;
    }

    // Now, take input and print it
    std::string word;
    std::cout << "Enter a word: ";
    std::cin >> word;

    std::cout << "You entered: " << std::hex << word << "\\" << std::hex << word[1] << word.length() << std::endl;
    std::cout << std::hex << (int)word[0] << std::hex << (int)word[1] << std::endl;
    return 0;
}