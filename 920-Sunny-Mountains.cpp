#include <iostream>
#include <iomanip>
#include <ios>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

#define MAX_N_COORD 100

// Pair of coordinates struct
struct Coord{
    int x;
    int y;
};

int main()
{
    // First line of input is the number of tests
    int n_tests;
    std::cin >> n_tests;

    // Data structures
    std::vector<Coord> v_coord;
    v_coord.reserve(MAX_N_COORD);

    // Loop over the tests
    for (int i = 0; i < n_tests; ++i) {

        // Read number of coordinates
        int n_coord;
        std::cin >> n_coord;

        // Clear previous data
        v_coord.clear();

        // Read coordinates
        for (int j = 0; j < n_coord; ++j) {
            int x, y;
            std::cin >> x >> y;
            
            v_coord.push_back({x, y});
        }

        // Order points by increasing x
        std::sort(
            v_coord.begin(),
            v_coord.end(),
            [](const Coord & a, const Coord & b) {
                return a.x < b.x;
            }
        );

        // Compute sum of illuminated segments
        double sum = 0;

        // Sun comes from the right horizontally
        // Variable to keep track of maximum y coordinate
        // to the right of the current peak
        int p_right_max_y = 0;

        // Go through ridge from right to left
        int j = 0;
        for (;;) {
            // Current peak
            // (v_coord last point is the last col)
            const int p_current_index = n_coord - 2 * (j + 1);
            if (p_current_index < 0)
                break;

            const Coord p_current = v_coord[p_current_index];
            
            // `Previous` col (to the right of the peak)
            const Coord c_right = v_coord[p_current_index + 1];

            // Length of slope that is illuminated
            double length = 0;

            // Compute length if peak is not shadowed by another at its right
            if (p_current.y > p_right_max_y) {
                const double y = p_current.y - p_right_max_y;

                // Horizontal size obtained using Thales theorem
                const double x =
                    (c_right.x - p_current.x) * y / (p_current.y - c_right.y);

                // If the peak is the rightmost (c_right.y)
                // expressions reduce to triangle 
                // where full slope is illuminated

                // Pythagoras theorem
                length = std::sqrt(x * x + y * y);

                // Update max height peak
                p_right_max_y = p_current.y;
            }

            sum = sum + length;
            ++j;
        }

        // Write output for each test
        std::cout << std::fixed;
        std::cout << std::setprecision(2);
        std::cout << sum << "\n";
    }

    return 0;
}
