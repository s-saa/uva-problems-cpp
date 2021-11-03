#include <iostream>

// Input range
#define MIN_NUM 0
#define MAX_NUM 10000

int main()
{
    // Read input
    // Number of lines is unknown
    for (;;) {
        // Read input line
        // Format: n_a n_b
        // Store lower bound (n_a)
        int n_a;
        std::cin >> n_a;

        // Check if end of file has been reached
        if (!std::cin)
            break;

        // Store upper bound (n_b)
        int n_b;
        std::cin >> n_b;

        int n_ini = n_a;
        int n_end = n_b;

        // Swap the bounding numbers if needed
        if (n_end < n_ini) {
            n_ini = n_b;
            n_end = n_a;
        }

        // Algorithm for line
        // Maximum cycle-length computed
        // There is always at least the initial number
        int max = 1;
        for (int i = n_ini; i <= n_end; ++i) {
            // For each i, compute cycle
            int n = i;

            // Count numbers generated in the cycle
            int count = 0;
            for (;;) {
                // Update counter
                ++count;

                // Stop when 1 is reached
                if (n == 1) {
                    // Update maximum if needed
                    if (count > max)
                        max = count;
                    break;
                }

                // Compute next number in the cycle
                if (n % 2 == 0)
                    n = n / 2;
                else
                    n = 3 * n + 1;                  
            }
        }

        // Write output
        std::cout << n_a << " " << n_b << " " << max << "\n";
    }

    return 0;
}