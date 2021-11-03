#include <iostream>
#include <vector>
#include <string>

// Returns blocks on top of element to original pos
void return_above(
    const int element, 
    const int pos,
    std::vector<std::vector<int>> & world,
    std::vector<int> & pos_world,
    std::vector<int> & pos_stack
) {
    // Vector containing element
    const std::vector<int> & v = world[pos];
    const int pos_stack_element = pos_stack[element];

    // Return blocks (!= element) on top of a to original pos
    for (int i = pos_stack_element + 1; i < static_cast<int>(v.size()); ++i) {
        const int block = v[i];
        world[block].push_back(block);
        pos_world[block] = block;
        pos_stack[block] = world[block].size() - 1;
    }
    world[pos].resize(pos_stack_element + 1);
}

int main()
{
    // First line of input is number of blocks
    int n;
    std::cin >> n;

    // Vector of vectors to store world
    std::vector<std::vector<int>> world(n);

    // Vector with blocks positions in world
    std::vector<int> pos_world(n);

    // Vector with blocks positions in stack
    std::vector<int> pos_stack(n, 0);

    // Initialize world and positions
    for (int i = 0; i < n; ++i) {
        world[i].push_back(i);
        pos_world[i] = i;
    }

    // Parse commands
    for (;;) {
        std::string verb;
        std::cin >> verb;

        if (!verb.compare("quit"))
            break;

        int a;
        std::cin >> a;

        std::string prep;
        std::cin >> prep;

        int b;
        std::cin >> b;

        // Ignore invalid commands
        // Same blocks
        if (a == b)
            continue;

        // Blocks are in same stack
        const int pos_a = pos_world[a];
        const int pos_b = pos_world[b];
        if (pos_a == pos_b)
            continue;

        if (!verb.compare("move")) {
            if (!prep.compare("onto")) {
                // Return blocks on top of a and b to original pos
                return_above(a, pos_a, world, pos_world, pos_stack);
                return_above(b, pos_b, world, pos_world, pos_stack);

                // Move a on top of b
                world[pos_b].push_back(a);
                world[pos_a].pop_back();
                pos_world[a] = pos_b;
                pos_stack[a] = world[pos_b].size() - 1;
            } else {
                // Only option here is "over"
                return_above(a, pos_a, world, pos_world, pos_stack);

                // Move a on top of stack containing b
                world[pos_b].push_back(a);
                world[pos_a].pop_back();
                pos_world[a] = pos_b;
                pos_stack[a] = world[pos_b].size() - 1;
            }
        } else {
            // Only option here is "pile"
            if (!prep.compare("onto")) {
                // Return blocks on top of b to original pos
                return_above(b, pos_b, world, pos_world, pos_stack);
                
                // Move (a and blocks above) on top of b
                const std::vector<int> & v_a = world[pos_a];
                const int pos_stack_a = pos_stack[a];
                // (This should be pos_stack[b])
                const int final_stack_b = world[pos_b].size() - 1;
                int j = 0;
                for (int i = pos_stack[a]; i < static_cast<int>(v_a.size()); ++i) {
                    const int block = v_a[i];
                    world[pos_b].push_back(block);
                    pos_world[block] = pos_b;
                    pos_stack[block] = final_stack_b + 1 + j;
                    ++j;
                }
                world[pos_a].resize(pos_stack_a);
            } else {
                // Only option here is "over"
                // Move (a and blocks above) on top of stack containing b
                const std::vector<int> & v_a = world[pos_a];
                const int pos_stack_a = pos_stack[a];
                const int final_stack_b = world[pos_b].size() - 1;
                int j = 0;
                for (int i = pos_stack[a]; i < static_cast<int>(v_a.size()); ++i) {
                    const int block = v_a[i];
                    world[pos_b].push_back(block);
                    pos_world[block] = pos_b;
                    pos_stack[block] = final_stack_b + 1 + j;
                    ++j;
                }
                world[pos_a].resize(pos_stack_a);
            }
        }
    }

    // Write output
    for (int i = 0; i < n; ++i) {
        std::cout << i << ":";
        const std::vector<int> & blocks = world[i];

        // If blocks is empty no space is written
        for (std::size_t j = 0; j < blocks.size(); ++j)
                std::cout << " " << blocks[j];

        std::cout << "\n";
    }
    
    return 0;
}
