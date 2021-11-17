#include <iostream>
#include <iomanip>
#include <ios>
#include <vector>
#include <algorithm>

#define MAX_N_KINGDOMS 20

struct Coord {
    int x;
    int y;

    Coord operator-(const Coord & other) const {
        Coord result;

        result.x = x - other.x;
        result.y = y - other.y;

        return result;
    }
};

using CoordIter = std::vector<Coord>::const_iterator;

// Check orientation of three points, i.e.:
// Returns:
//      >0 if they are clockwise oriented (i.e. they turn to the right)
//      <0 if they are counterclockwise oriented (i.e. they turn to the left)
//      0 if they are collinear
int orientation(const Coord & p_a, const Coord & p_b, const Coord & p_c) {
    // Move origin to p_a
    const Coord b = p_b - p_a;
    const Coord c = p_c - p_a;

    // The `orientation` quantity is built from the
    // comparison between the slopes of the
    // oriented lines between a and b, and a and c
    return b.y * c.x - c.y * b.x;
}

// Function to advance to the left in a vector of coordinates
// avoiding points already selected
// Caller has to ensure origin is not leftmost
CoordIter advance_left(
    const CoordIter & origin,
    const CoordIter & leftmost,
    const std::vector<bool> & is_selected
) {
    auto current = origin - 1;
    std::size_t index = current - leftmost;

    for (;;) {

        // Break if best_guess is leftmost
        if (current == leftmost)
            break;

        // Break once a `valid` point is found
        if (is_selected[index] == 0)
            break;

        // Try next point
        --current;
        --index;
    }

    return current;
}

void read_input(
    std::vector<std::vector<Coord>> & kingdoms,
    std::vector<Coord> & missiles
) {
    // Unknown number of kingdoms
    for (;;) {
        int n;
        std::cin >> n;

        if (!std::cin)
            break;

        // It is a kingdom
        if (n != -1) {
            // Vector to store kingdom sites
            std::vector<Coord> sites(n);

            // There are n lines with coordinates
            for (int i = 0; i < n; ++i) {
                Coord coord;
                std::cin >> coord.x >> coord.y;

                sites[i] = coord;
            }

            // Order points from left to right
            // and bottom to top
            std::sort(
                sites.begin(),
                sites.end(),
                [](const Coord & a, const Coord & b) {
                    if (a.x == b.x)
                        return a.y < b.y;
                    else
                        return a.x < b.x;
                }
            );

            kingdoms.push_back(sites);
        }
        else {
            // Unknown number of lines
            // with missile coordinates
            for (;;) {
                Coord coord;
                std::cin >> coord.x >> coord.y;

                if (!std::cin)
                    break;

                missiles.push_back(coord);
            }
        }
    }
}

void compute_upper_hull(
    const CoordIter & leftmost,
    const CoordIter & rightmost,
    std::vector<bool> & is_hull,
    std::vector<Coord> & hull
) {
    // First point of upper hull is the leftmost-bottom  
    hull.push_back(*leftmost);
    is_hull[0] = true;

    // Position of last point found to be in the hull
    auto last = leftmost;

    // End iterator of sites vector
    const auto end = rightmost + 1;

    for (;;) {
        // There are at least three points in the kingdom
        // Next point to the right is the initial best guess in this iteration
        auto best_guess = last + 1;

        // Point to be checked at the iteration
        // Current points will always be more to the right
        // than in previous iterations
        auto current = best_guess + 1;

        // Check all other sites
        for (;;) {
            // Break when end iterator is reached
            if (current == end)
                break;

            // Update best_guess if a point is found such that
            // the line last->current is to the LEFT of last->best_guess
            // or if they are collinear
            if (orientation(*last, *best_guess, *current) <= 0)
                best_guess = current;

            // Advance
            ++current;
        }

        // Check if rightmost point is reached
        // This point is not pushed back to upper hull,
        // since it will be the first point of lower hull
        if (best_guess == rightmost)
            break;

        // Otherwise introduce the point found
        last = best_guess;
        const std::size_t index = best_guess - leftmost;
        is_hull[index] = true;
        hull.push_back(*best_guess);
    }
}

void compute_lower_hull(
    const CoordIter & leftmost,
    const CoordIter & rightmost,
    std::vector<bool> & is_hull,
    std::vector<Coord> & hull
) {
    // First point will be the rightmost site
    hull.push_back(*rightmost);
    is_hull.back() = true;

    // Position of last point found to be in the hull
    auto last = rightmost;

    for (;;) {
        // Next point to the left is the initial best guess in this iteration
        // that is not already in the hull
        // `last` is never leftmost
        auto best_guess = advance_left(last, leftmost, is_hull);

        // Break if best_guess is leftmost
        if (best_guess == leftmost)
            break;

        // Point to be checked at the iteration
        // Current points will always be more to the left
        // than in previous iterations
        // `best_guess` is not leftmost
        auto current = advance_left(best_guess, leftmost, is_hull);

        // Check all other sites
        for (;;) {
            // Update best_guess if a point is found such that
            // the line last->current is to the LEFT of last->best_guess
            // or collinear
            if (orientation(*last, *best_guess, *current) <= 0)
                best_guess = current;

            // Break when leftmost point is reached
            // This is checked after the orientation to avoid
            // keeping as "best_guess" the initial point, which
            // would not be checked otherwise
            if (current == leftmost)
                break;

            // Advance until new current point is not already on the hull
            // `current` is not leftmost
            current = advance_left(current, leftmost, is_hull);
        }

        // Check if leftmost point is reached
        // and so the hull is closed
        if (best_guess == leftmost)
            break;

        // Otherwise introduce the point found
        last = best_guess;
        const std::size_t index = best_guess - leftmost;
        is_hull[index] = true;
        hull.push_back(*best_guess);
    }
}

int main()
{
    // Vector of vectors with kingdom's points
    std::vector<std::vector<Coord>> kingdoms;
    kingdoms.reserve(MAX_N_KINGDOMS);

    // Vector with missile coordinates
    std::vector<Coord> missiles;

    read_input(kingdoms, missiles);

    // Number of missiles
    const std::size_t n_missiles = missiles.size();

    // Number of kingdoms
    const std::size_t n_kingdoms = kingdoms.size();

    // Compute border of each kingdom
    std::vector<std::vector<Coord>> borders(n_kingdoms);

    for (std::size_t k = 0; k < n_kingdoms; ++k) {
        // `sites` vector is const,
        // thus iterators usage below is safe
        const std::vector<Coord> sites = kingdoms[k];
        const std::size_t n_sites = sites.size();

        // Find border (convex hull) around each kingdom
        std::vector<Coord> hull;
        hull.reserve(n_sites);

        // Vector to indicate if a point is on the hull
        std::vector<bool> is_hull(n_sites, false);

        // Leftmost(-bottom) and rightmost(-top) points
        // (Sites vector is ordered)
        const auto leftmost = sites.begin();
        const auto rightmost = sites.end() - 1;

        compute_upper_hull(
            leftmost, rightmost,
            is_hull, hull
        );

        compute_lower_hull(
            leftmost, rightmost,
            is_hull, hull
        );

        borders[k] = hull;
    }

    // Two times the total area of the kingdoms without power
    int twice_total_area = 0;

    // Find which kingdoms have missile impacts
    for (std::size_t i = 0; i < n_missiles; ++i) {
        const Coord missile = missiles[i];

        for (std::size_t k = 0; k < borders.size(); ++k) {
            const std::vector<Coord> hull = borders[k];

            // Number of vertices in the hull
            const std::size_t n_hull = hull.size();

            // Missile has to be at least 
            // between leftmost and rightmost sites
            const Coord leftmost_point = hull[0];
            const Coord rightmost_point = kingdoms[k].back();

            // This will count missiles that impact on border
            if (missile.x < leftmost_point.x || missile.x > rightmost_point.x)
                continue;

            // If missile is between leftmost and rightmost points,
            // go over border edges to check if missile
            // is inside kingdom and compute (2x)area
            int twice_area = 0;

            // Area computation is based on shoelace formula,
            // optimized to have fewer multiplications
            // Sum from j = 1 to j = n - 2
            Coord previous = hull[0];
            Coord current = hull[1];
            Coord next;

            // Bool to track if missile is found for this kingdom
            // Missile is assumed to be found at this point,
            // looping over the hull edges will check it
            bool is_inside = true;

            // Loop over the first n - 2 edges
            std::size_t j = 1;
            for (; j < n_hull - 1;) {
                // Hull is ordered clockwise, so a missile
                // is inside if it is to the right of all the edges
                // Set is_inside to 0 and break as soon as
                // the missile is found to be to the left (i.e. it is outside)
                if (orientation(hull[j - 1], hull[j], missile) < 0) {
                    is_inside = false;
                    break;
                }

                // Compute next area term
                next = hull[j + 1];

                // Hull is clockwise oriented by construction
                // Signs are chosen to provide a positive value
                twice_area += current.x * (previous.y - next.y);

                previous = current;
                current = next;
                ++j;
            }

            // Go to next kingdom if it was found to be outside
            if (!is_inside)
                continue;

            // Check the last two edges

            // j = n - 1 (nth vertex is hull[0])
            // Check edge between vertices n - 2 and n - 1
            if (orientation(hull[j - 1], hull[j], missile) < 0) {
                // If outside, look in next kingdom
                continue;
            }
            else {
                next = hull[0];
                twice_area += current.x * (previous.y - next.y);

                previous = current;
                current = next;
                ++j;
            }

            // j = n (nth vertex is hull[0], n+1th vertex is hull[1])
            // Check edge between vertices n - 2 and n - 1
            // (Last edge: condition is inverted to finish area computation)
            if (orientation(hull[j - 1], hull[0], missile) >= 0) {

                next = hull[1];
                twice_area += current.x * (previous.y - next.y);

                twice_total_area += twice_area;

                // Remove kingdom data if it already has a missile impact
                // (Last element of the vectors will occupy the kingdom's position)
                kingdoms[k] = kingdoms.back();
                kingdoms.pop_back();

                borders[k] = borders.back();
                borders.pop_back();

                // Stop looking in the kingdoms if missile 
                // has already been found (kingdoms do not overlap)
                break;
            }
        }
    }

    // Write output
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    std::cout << 1.0 / 2.0 * twice_total_area << "\n";

    return 0;
}