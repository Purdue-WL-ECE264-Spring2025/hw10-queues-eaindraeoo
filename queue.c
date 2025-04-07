#include "queue.h"
#include "tile_game.h"

static bool is_solved (struct game_state state) {
    // Check if the game state is solved (i.e., the tiles are in order)
    int expected = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == 3 && j == 3) {
                // The last tile should be zero
                if (state.tiles[i][j] != 0) {
                    return false;
                }
            } else {
                if (state.tiles[i][j] != expected++) {
                    return false;
                }
            }
        }
    }
    return true;
}

void enqueue(struct queue *q, struct game_state state) {
    uint64_t serialized = serialize(state);
    insert_at_tail(&q->data, serialized);
}

struct game_state dequeue(struct queue *q) { 
    uint64_t serialized = remove_from_head(&q->data);
    return deserialize(serialized); 
}

static bool is_visited(struct linked_list *visited_list, uint64_t state) {
    struct list_node *curr = visited_list->head;
    while (curr != NULL) {
        if (curr->value == state) {
            return true; // State has already been visited
        }
        curr = curr->next;
    }
    return false; // State has not been visited
}

static void add_visited(struct linked_list *visited_list, uint64_t state) {
    insert_at_tail(visited_list, state);
}

int number_of_moves(struct game_state start) { 
   if (is_solved(start))
   {
    return start.num_steps; // Already solved, no moves needed
   }

   struct queue q;
   q.data.head = NULL;

   struct linked_list visited_list = { .head = NULL };

   uint64_t start_serialized = serialize(start);
   enqueue(&q, start);
   add_visited(&visited_list, start_serialized);

    typedef void (*move_func)(struct game_state *);
    move_func move[] = {move_up, move_down, move_left, move_right};

    while(q.data.head != NULL) {
        struct game_state curve = dequeue(&q);
        for (int i = 0; i < 4; i++) {
            // Apply each possible move
            struct game_state next = curve;
            uint64_t bef = serialize(next);

            move[i](&next);
            uint64_t aft = serialize(next);

            if(bef == aft) {
                // If the state hasn't changed, skip this move
                continue;
            }

            if (is_visited(&visited_list, aft)) {
                // If this state has already been visited, skip it
                continue;
            }

            add_visited(&visited_list, aft);
            
            if (is_solved(next)) {
                free_list(q.data);
                free_list(visited_list);
                return next.num_steps; // Return the number of moves
            }

            enqueue(&q, next);
        }
    }

    free_list(visited_list);
    return -1;
}
