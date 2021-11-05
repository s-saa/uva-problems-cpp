#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>

#define MAX_N 1000000

// Compute greatest common divisor
// using Euclidean algorithm
int gcd(const int a, const int b) {
    if (b == 0)
        return abs(a);

    return gcd(b, a % b);
}

int main()
{
    // Vector to keep track of numbers
    // part of any triple (primitive or not),
    // with x, y, z <= n
    std::vector<bool> is_p;
    is_p.reserve(MAX_N);

    for (;;) {
        // Read input
        int n;
        std::cin >> n;

        if (!std::cin)
            break;

        // Reset and initialize is_p
        // Numbers are p unless valid triple
        // is formed with them
        is_p.clear();
        is_p.resize(n, 1);

        // Number of primitive Pythagorean triples
        // (primitives) with x, y, z <= n
        int n_prim = 0;

        // Generate relatively prime triples
        // For z to be <= n, i and j < sqrt(n) at least
        const int limit = static_cast<int>(std::sqrt(n));
        for (int i = 1; i <= limit; ++i) {
            for (int j = i + 1; j <= limit; j = j + 1) {

                // Check generated numbers are <= n
                const int z = j * j + i * i;
                if (z > n)
                    break;

                const int y = 2 * i * j;
                const int x = j * j - i * i;

                // x, y, z are part of a Pyth triple
                // Also multiples kx, ky, kz will be Pyth triples
                for (int k = 1; k * z <= n; ++k) {
                    is_p[k * x - 1] = 0;
                    is_p[k * y - 1] = 0;
                    is_p[k * z - 1] = 0;
                }

                // x, y, z are relatively prime
                // if only one of i or j is even
                if ((i + j) % 2 != 0) {
                    // and if i, j are relatively prime
                    if (gcd(i, j) == 1) {
                        ++n_prim;
                    }
                }
            }
        }

        // Sum numbers not part of a triple
        const int n_p = std::accumulate(is_p.begin(), is_p.end(), 0);

        // Write output
        std::cout << n_prim << " " << n_p << "\n";
    }

    return 0;
}
