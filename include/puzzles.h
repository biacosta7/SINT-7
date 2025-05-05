#ifndef PUZZLES_H
#define PUZZLES_H
#include <stdbool.h>

void init_puzzles();
void update_puzzle();
void draw_puzzle();
void start_memory_fragment(int id);
bool is_puzzle_solved();
void end_puzzle();

#endif // PUZZLES_H
