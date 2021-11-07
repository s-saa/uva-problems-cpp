#include <iostream>
#include <vector>

int main()
{
    // Read input
    // Size of numbers square
    int size;
    std::cin >> size;

    // Vector to store numbers
    std::vector<int> v(size * size);

    // Read and store numbers
    for (int i = 0;; ++i) {
        int n;
        std::cin >> n;

        if (!std::cin)
            break;

        v[i] = n;
    }

    // Compute max subrectangle sum
    int max_sum = 0;

    std::vector<int> v_col(size * size);
    for (int i = 0; i < size; ++i) {
        int sum = 0;
        int sub_sum = 0;
        const int row = size * i;

        for (int j = 0; j < size; ++j) {         
            // Store vector with values by columns
            // (In v numbers are stored by rows)
            const int new_value = v[i * size + j];
            v_col[i + j * size] = new_value;

            // Apply Kadane to each row in v
            // This computes all the w x 1 subrectangles
            sum += v[row + j];

            if (sum < 0)
                sum = 0;

            else if (sum > sub_sum)
                sub_sum = sum;
        }

        if (sub_sum > max_sum)
            max_sum = sub_sum;
    }

    // Compute all the w x h subrectangles with w >= 1, h > 1
    for (int h = 2; h <= size; ++h) {
        // Number of `subrectangle` rows for this size h
        const int n_sub_rows = size - h + 1;

        // Rewrite v with the sums of subrectangles 1 x h
        // Each subrectangle result is stored starting from the last row
        const int last_row = size * (size - 1);

        // Previous sums and original data in v_c
        // are reused to compute larger subrectangles
        // i is used to go up the column
        for (int i = 0; i < n_sub_rows; ++i) {
            const int row = last_row - size * i;
             
            int sum = 0;
            int sub_sum = 0;

            // j moves to the next subrectangle to the right        
            for (int j = 0; j < size; ++j) {
                v[row + j] += v_col[size - h - i + size * j];

                // Apply Kadane to the row of `subrectangles`
                sum += v[row + j];

                if (sum < 0)
                    sum = 0;

                else if (sum > sub_sum)
                    sub_sum = sum;
            }

            if (sub_sum > max_sum)
                max_sum = sub_sum;
        }
    }

    // Write output
    std::cout << max_sum << "\n";

    return 0;
}
