#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdbool.h>

// To check if two game states are equal by comparing all tile positions
bool states_equal(struct game_state s1, struct game_state s2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (s1.tiles[i][j] != s2.tiles[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// To determine if a serialized game state has already been visited
bool is_visited(uint64_t serialized_state, uint64_t *visited, int visited_count) {
    for (int i = 0; i < visited_count; i++) {
        if (visited[i] == serialized_state) {
            return true;
        }
    }
    return false;
}

// To enqueue a game state into the BFS queue after serializing it
void enqueue(struct queue *q, struct game_state state) {
    uint64_t serialized = serialize(state); // To convert the state to a compact format
    insert_at_tail(&q->data, serialized);   // To insert it at the end of the queue
}

// To dequeue the next game state from the BFS queue and deserialize it
struct game_state dequeue(struct queue *q) {
    uint64_t serialized = remove_from_head(&q->data); // To get the next state from the queue
    return deserialize(serialized);                   // To convert it back to game state format
}

// To compute the minimum number of moves required to solve the tile puzzle from a given start state
int number_of_moves(struct game_state start) {
    // To define the goal state (solved puzzle configuration)
    struct game_state target = {
        .tiles = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}
        },
        .empty_row = 3,
        .empty_col = 3,
        .num_steps = 0
    };
    
    // To immediately return if the puzzle is already solved
    if (states_equal(start, target)) {
        return 0;
    }
    
    // To initialize the BFS queue and enqueue the starting state
    struct queue q = {.data = {.head = NULL}};
    enqueue(&q, start);
    
    // To keep track of visited states to prevent reprocessing
    uint64_t *visited = malloc(1000000 * sizeof(uint64_t));
    if (!visited) {
        return -1; // Memory allocation failure
    }
    int visited_count = 0;
    visited[visited_count++] = serialize(start); // To mark the starting state as visited
    
    // To define movement directions: up, right, down, left
    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};
    
    // To begin BFS traversal
    while (q.data.head != NULL) {
        struct game_state current = dequeue(&q); // To get the next state to explore
        
        // To explore all valid moves from the current state
        for (int dir = 0; dir < 4; dir++) {
            int new_row = current.empty_row + dr[dir];
            int new_col = current.empty_col + dc[dir];
            
            // To ensure move is within bounds
            if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) {
                struct game_state next_state = current; // To create a copy for modification
                
                // To move the tile into the empty space
                next_state.tiles[current.empty_row][current.empty_col] = 
                    current.tiles[new_row][new_col];
                next_state.tiles[new_row][new_col] = 0;
                
                // To update empty tile's coordinates
                next_state.empty_row = new_row;
                next_state.empty_col = new_col;
                
                // To increment the number of steps taken
                next_state.num_steps++;
                
                // To check if this new state solves the puzzle
                if (states_equal(next_state, target)) {
                    free(visited);        // To clean up allocated memory
                    free_list(q.data);    // To deallocate queue memory
                    return next_state.num_steps;
                }
                
                // To serialize the new state for tracking
                uint64_t serialized_next = serialize(next_state);
                
                // To enqueue the state only if it hasn't been visited before
                if (!is_visited(serialized_next, visited, visited_count)) {
                    visited[visited_count++] = serialized_next; // Mark as visited
                    enqueue(&q, next_state);                    // Enqueue for future exploration
                }
            }
        }
    }
    
    // To free memory after search completes without success
    free(visited);
    free_list(q.data);
    
    // To indicate that no solution was found
    return -1;
}
