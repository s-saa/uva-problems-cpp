#include <iostream>
#include <string>
#include <istream>
#include <vector>
#include <algorithm>
#include <utility>

#define MAX_N_STATIONS 10000
#define MAX_N_IMPORTANT 1000
#define MAX_N_LINES 100

int main()
{
    // Data structures to be used in tests

    // Vector of vectors to store lines of stations
    // (graph will be constant, no need to use list)
    std::vector<std::vector<int>> lines;
    lines.reserve(MAX_N_LINES);

    // Vector to store number of times 
    // each station is found
    // (if count > 1 the station is important)
    // (lines never repeat stations)
    std::vector<int> counts;
    counts.reserve(MAX_N_STATIONS);
    // Vector of important stations
    std::vector<int> v_important;
    v_important.reserve(MAX_N_IMPORTANT);

    // Vector to store order of important station 
    // (important_station = v_important[order_important - 1])          
    // For non-important stations it will store 0
    std::vector<int> order_important;
    order_important.reserve(MAX_N_STATIONS);

    // Vector of vectors storing distances
    // between important stations
    // (dist[s])[t]
    // s (t) index corresponds to v_important[s]
    std::vector<std::vector<int>> dist;

    // Read input

    // Read first line: number of tests
    int n_tests;
    std::cin >> n_tests;

    // Read parameters for each test
    for (int i = 0; i < n_tests; ++i) {

        // Read total number of stations 
        // and number of lines
        int n_stations;
        int n_lines;
        std::cin >> n_stations >> n_lines;

        // Clear data
        lines.clear();
        counts.clear();
        v_important.clear();
        order_important.clear();
        dist.clear();
        
        // Vectors initialized with 0
        counts.resize(n_stations, 0);
        order_important.resize(n_stations, 0);

        // Read stations in each line
        for (int l = 0; l < n_lines; ++l) {
            std::vector<int> stations_read;

            for (;;) {
                int station;
                std::cin >> station;

                if (!station)
                    // When 0 is read it stops reading line
                    // 0 is not introduced in the line vector
                    break;
                stations_read.push_back(station);

                // Update count value for the station
                ++counts[station - 1];
            }

            lines.push_back(stations_read);
        }

        int order = 0;
        // Simplify graph by retaining only the important stations as nodes
        for (int s = 0; s < n_stations; ++s) {
            if (counts[s] > 1) {
                const int station = s + 1;
                v_important.push_back(station);
                
                // Update `order count`
                ++order;
                
                // Store number for this station
                order_important[s] = order;

                // Create a vector of distances
                dist.push_back({});
            }
        }
        
        // Number of important stations
        const std::size_t n_important = v_important.size();

        // In order to keep always a minimum value
        // distances will be initialized to a 
        // maximum path (n_stations - 1)
        // (edges in full graph count always as 1)
        for (std::size_t s = 0; s < n_important; ++s) {
            (dist[s]).resize(n_important, n_stations - 1);

            // Distance between the same station is zero
            (dist[s])[s] = 0;
        }

        // Look for pairs of `consecutive` important stations in each line
        for (std::size_t l = 0; l < lines.size(); ++l) {
            // Count of edges between `consecutive` important stations
            int steps = 0;

            // Pair to store `consecutive` important stations
            std::pair<int, int> found = {0, 0};

            // Vector of stations in the line
            // (Lines will remain constant till end of problem)
            std::vector<int> & stations = lines[l];

            // Loop over stations in each line
            for (std::size_t s = 0; s < stations.size(); ++s) {
                // Station number
                const int station = stations[s];

                // Check if station is important
                if (order_important[station - 1] > 0) {
                    // If the station is the first of the first pair
                    if (found.first == 0) {
                        found.first = station;
                    }
                    else {
                        // The station is not the first one found,
                        // thus it will be the second of a pair
                        found.second = station;

                        // Equivalent indeces in the `important` vector
                        const int a = order_important[found.first - 1] - 1;
                        const int b = order_important[found.second - 1] - 1;

                        // Store distance for the completed pair
                        // If a pair is repeated in a different line, 
                        // the minimum path is kept
                        if (steps < (dist[a])[b]) {
                            // Graph is unordered
                            (dist[a])[b] = steps;
                            (dist[b])[a] = steps;
                        }

                        // The station found will be the first of a pair
                        // (if it can be completed before ending the line)
                        found.first = station;

                        // Reinitialize steps for the next pair
                        steps = 0;
                    }
                }

                // If a pair has already been started
                // update steps before advancing to next station
                if (found.first != 0)
                    ++steps;
            }
        }

        // Compute minimum paths between
        // pairs of important stations
        // Floyd is applied on simplified graph
        for (std::size_t k = 0; k < n_important; ++k) {
            for (std::size_t s = 0; s < n_important; ++s) {
                for (std::size_t t = 0; t < n_important; ++t) {
                    // Compare distance_st with sum of distances
                    // to `intermediate` node k   
                    const int distance_st = (dist[s])[t];

                    // Skip when distance_st is already minimal
                    if (distance_st > 1) {
                        const int distance_skt = (dist[s])[k] + (dist[k])[t];
                        if (distance_st > distance_skt)
                            (dist[s])[t] = distance_skt;
                    }
                }
            }
        }

        // Look for important station with minimum sum of 
        // minimum paths with all other important stations
        // (There is no need to compute average)
        // Value of minimum sum of minimum paths 
        // is initialized with largest value,
        // (n_stations - 1) for each path
        int min_sum = (n_stations - 1) * n_important;

        // Int to store important station with minimum average
        int min_station = 0;
  
        // Compute sum of minimum paths for each important station
        // This loops in order of increasing station number,
        // thus in the case two stations can be solution,
        // it will keep the smaller one
        for (std::size_t s = 0; s < n_important; ++s) {
            int sum = 0;

            for (std::size_t t = 0; t < n_important; ++t) {
                sum += (dist[s])[t];
            }

            // Update minimum sum if needed
            // and update minimum station accordingly
            if (sum < min_sum) {
                min_sum = sum;
                min_station = v_important[s];
            }
        }

        // Write output
        std::cout << "Krochanska is in: " << min_station << "\n";
    } // End of loop over tests

    return 0;
}
