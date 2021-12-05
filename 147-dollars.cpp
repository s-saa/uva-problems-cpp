#include <iostream>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <array>

// Money is assumed always valid:
// even if multiplied by 100, 
// int of 2 bytes can be used
using Money = std::uint16_t;

// Assuming output can be as large as 16 digits,
// 8 bytes are used for the number of combinations
// A negative value will be used to indicate a result has
// not yet be computed
using Result = std::int64_t;

using Iter = std::array<Money, 11>::const_iterator;

template <class T>
Result compute_subproblem(
    const Money subsum,
    const Iter begin_it,
    const Iter values_end,
    const std::size_t subresult_num_cols,
    T & subresults_array
);

template <class T>
Result compute_combinations(
    const Money money,
    const Iter values_begin,
    const Iter values_end,
    const std::size_t subresult_num_cols,
    T & subresults_array
) {
    // The return value is the number of combinations 
    // that sum to money
    // using values from values_begin (inclusive) to values_end
    Result num_combinations = 0;

    for (Iter it = values_begin; it != values_end; ++it) {
        const Money value = *it;

        // Skip values that are larger than target
        if (value > money)
            continue;

        // There is an infinite amount of any copy of those values
        // The max amount of coins of the value that can be used
        // is provided by the integer division
        const Money quotient = money / value;

        // The new addend to the sum of money
        const Money addend = quotient * value;

        // If the value is the last one, there is only
        // a possible combination if quotient * value
        // sum to the target
        if (it == values_end - 1) {
            if (money == addend)
                ++num_combinations;

            break;
        }

        // Otherwise, there is the situation that adding quotient * value
        // already sums to the target, but also more combinations with smaller
        // multiples (see below)
        if (money == addend) {
            ++num_combinations;
        }
        else {
            // Try to find combinations with smaller values
            // for the remainder
            num_combinations += compute_subproblem(
                money - addend,
                it + 1,
                values_end,
                subresult_num_cols,
                subresults_array
            );
        }
        
        // Continue with the algorithm for smaller multiples
        for (Money i = 1; i < quotient; ++i) {
            // At this point remainder cannot be negative nor equal to money
            num_combinations += compute_subproblem(
                money - i * value,
                it + 1,
                values_end,
                subresult_num_cols,
                subresults_array
            );
        }
    }

    return num_combinations;
}

template <class T>
Result compute_subproblem(
    const Money subsum,
    const Iter begin_it,
    const Iter values_end,
    const std::size_t subresult_num_cols,
    T & subresults_array
) {
    // Number of values used in the subproblem
    const std::size_t num_values = values_end - begin_it;

    const std::size_t row = num_values - 1;
    const std::size_t col = subsum - 1;
    Result & subproblem_combinations =
        subresults_array[row * subresult_num_cols + col];

    // Recompute if not previously computed
    if (subproblem_combinations == -1)
        subproblem_combinations =
            compute_combinations(
                subsum,
                begin_it,
                values_end,
                subresult_num_cols,
                subresults_array
            );

    return subproblem_combinations;
}

int main()
{
    // Values are multiplied by * 20:
    //  - multiplied by 100 to have int values
    //  - divided by 5 (lowest available coin) to decrease size
    constexpr Money c_resize = 20;

    // Number of available banknotes/coins
    constexpr std::size_t num_values = 11;

    // Array with available banknotes/coins values
    // (already taking into account c_resize)
    constexpr std::array<Money, num_values> values = {
        2000, 1000, 400, 200, 100,
        40, 20, 10, 4, 2, 1
    };

    // Max required target
    constexpr Money max_target = c_resize * 300;

    // In order to compute the number of combinations
    // a recursive algorithm is used
    // An array of subresults is used to avoid computing
    // the same `subresult` more than once
    // The array contains the following matrix:
    // - a row per number of values used (ordered from lowest to largest)
    //      i.e. row_index = 0 means (1) values = {1} used
    //           row_index = 1 means (2) values = {1, 2} used
    //  - a column per possible subresult (i.e. partial sum)
    //      i.e. col_index = 0 means the sum in that case is 1
    //      The total number of columns is then the maximum value for the required sum
    // E.g.: subresult[4][99] = number of combinations that sum up
    //          to 100 using the 4 lowest available banknotes/coins
    constexpr std::size_t num_cols = static_cast<std::size_t>(max_target);
    std::array<Result, num_values * num_cols> subresult;

    // A value of -1 indicates the subresult has not yet been computed
    subresult.fill(static_cast<Result>(-1));

    for (;;) {
        // Read money
        float m;
        std::cin >> m;

        if (m == 0)
            break;

        const Money money = static_cast<Money>(m * c_resize);

        const Result n =
            compute_combinations(
                money,
                values.begin(),
                values.end(),
                num_cols,
                subresult);

        // Write output
        std::cout << std::fixed;
        std::cout << std::setprecision(2);
        std::cout << std::right;

        std::cout << std::setw(6) << m ;
        std::cout << std::setw(17) << n << "\n";
    }

    return 0;
}
