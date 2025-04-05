#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdbool.h>

// Function to check if two game states are exactly the same
bool states_equal(struct game_state s1, struct game_state s2) {
    // Loop through each row
    for (int i = 0; i < 4; i++) {
        // Loop through each column
        for (int j = 0; j < 4; j++) {
            // If the value in any tile does not match, return false
            if (s1.tiles[i][j] != s2.tiles[i][j]) {
                return false;
            }
        }
    }
    // If we didn't find any mismatches, the states are equal
    return true;
}

// Function to check if a state has already been visited
bool is_visited(uint64_t serialized_state, uint64_t *visited, int visited_count) {
    // Iterate through every visited state
    for (int i = 0; i < visited_count; i++) {
        // If we find a match, return true
        if (visited[i] == serialized_state) {
            return true;
        }
    }
    // If no match was found, return false
    return false;
}

// Function to add a state to the queue
void enqueue(struct queue *q, struct game_state state) {
    // Convert the state into a compact 64-bit format
    uint64_t serialized = serialize(state);
    // Insert the serialized state at the end of the queue
    insert_at_tail(&q->data, serialized);
}

// Function to remove a state from the front of the queue and deserialize it
struct game_state dequeue(struct queue *q) {
    // Remove the front element of the queue
    uint64_t serialized = remove_from_head(&q->data);
    // Convert it back to game_state format
    struct game_state state = deserialize(serialized);
    return state;
}

// Main function to determine the minimum number of moves to solve the puzzle
int number_of_moves(struct game_state start) {
    // Define what the solved puzzle looks like
    struct game_state target;
    target.tiles[0][0] = 1;  target.tiles[0][1] = 2;  target.tiles[0][2] = 3;  target.tiles[0][3] = 4;
    target.tiles[1][0] = 5;  target.tiles[1][1] = 6;  target.tiles[1][2] = 7;  target.tiles[1][3] = 8;
    target.tiles[2][0] = 9;  target.tiles[2][1] = 10; target.tiles[2][2] = 11; target.tiles[2][3] = 12;
    target.tiles[3][0] = 13; target.tiles[3][1] = 14; target.tiles[3][2] = 15; target.tiles[3][3] = 0;
    target.empty_row = 3;
    target.empty_col = 3;
    target.num_steps = 0;

    // If the start is already the goal, return 0
    if (states_equal(start, target)) {
        return 0;
    }

    // Initialize a queue with an empty linked list
    struct queue q;
    q.data.head = NULL;

    // Add the initial state to the queue
    enqueue(&q, start);

    // Create a visited array to store serialized versions of states we have seen
    uint64_t *visited = malloc(1000000 * sizeof(uint64_t));
    if (visited == NULL) {
        return -1; // If malloc failed
    }

    // Mark the start state as visited
    uint64_t start_serialized = serialize(start);
    visited[0] = start_serialized;
    int visited_count = 1;

    // Define movement directions (row and column offsets)
    const int dr[4] = {-1, 0, 1, 0}; // row change: up, right, down, left
    const int dc[4] = {0, 1, 0, -1}; // column change: up, right, down, left

    // Begin the BFS loop
    while (q.data.head != NULL) {
        // Remove the front state from the queue
        struct game_state current_state = dequeue(&q);

        // Loop over each of the 4 possible directions
        for (int direction = 0; direction < 4; direction++) {
            // Calculate the new position of the empty tile after the move
            int row_change = dr[direction];
            int col_change = dc[direction];
            int new_empty_row = current_state.empty_row + row_change;
            int new_empty_col = current_state.empty_col + col_change;

            // Check if the move stays within bounds of the 4x4 board
            if (new_empty_row >= 0 && new_empty_row < 4 && new_empty_col >= 0 && new_empty_col < 4) {
                // Make a deep copy of the current state
                struct game_state new_state = current_state;

                // Move the tile into the empty space
                int value_to_move = current_state.tiles[new_empty_row][new_empty_col];
                new_state.tiles[current_state.empty_row][current_state.empty_col] = value_to_move;
                new_state.tiles[new_empty_row][new_empty_col] = 0;

                // Update the empty tile’s location in the new state
                new_state.empty_row = new_empty_row;
                new_state.empty_col = new_empty_col;

                // Increment the number of steps taken to reach this state
                new_state.num_steps = current_state.num_steps + 1;

                // If this state matches the target, we're done
                if (states_equal(new_state, target)) {
                    free(visited);     // Free the visited memory
                    free_list(q.data); // Free any remaining nodes in the queue
                    return new_state.num_steps;
                }

                // Serialize the new state
                uint64_t serialized_new_state = serialize(new_state);

                // If we haven't seen this state before, mark as visited and enqueue it
                if (!is_visited(serialized_new_state, visited, visited_count)) {
                    visited[visited_count] = serialized_new_state;
                    visited_count++;

                    // Add to queue for BFS
                    enqueue(&q, new_state);
                }
            }
        }
    }

    // If we exit the loop, it means we couldn’t solve the puzzle
    free(visited);     // Clean up the visited array
    free_list(q.data); // Clean up any remaining queue items
    return -1;         // Return failure
}
