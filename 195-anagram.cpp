#include <iostream>
#include <string>
#include <algorithm>

// Map characters to an integer in AaBb... series
int map_char(const char & character) {
    const int integer = static_cast<int>(character);
    const int a_int = static_cast<int>('a');

    // If lowercase
    if (integer >= a_int)
        return (2 * (integer - a_int) + 1);

    // If uppercase
    return 2 * (integer - static_cast<int>('A'));
}

int main()
{
    // Read the number of words
    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        // Read each word
        std::string word;
        std::cin >> word;

        // First permutation are the characters sorted
        // by custom comparison
        std::sort(
            word.begin(),
            word.end(),
            [](const char & a, const char & b) {
                return map_char(a) < map_char(b);
            }
        );

        std::cout << word << "\n";

        // Keep printing the next circular permutation
        // until finished, using custom comparison
        for (;;) {
            const bool b_perm =
                std::next_permutation(
                    word.begin(),
                    word.end(),
                    [](const char & a, const char & b) {
                        return map_char(a) < map_char(b);
                    }
            );

            if (!b_perm)
                break;

            std::cout << word << "\n";
        }
    }

    return 0;
}
