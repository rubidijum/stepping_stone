#ifndef __STEPPING_STONE_HPP
#define __STEPPING_STONE_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <numeric>
#include <algorithm>
 
typedef std::pair<unsigned,unsigned> field_coord;
typedef std::pair<field_coord, unsigned> field;

bool find_path(field_coord entry, field_coord curr, std::vector<field_coord>* path, std::vector<field> possible_hops, bool direction);
int calculate_path_cost(std::vector<field_coord> path, std::vector<std::vector<unsigned>> cost_matrix);
int find_stone(std::vector<field_coord> path, std::vector<field> initial);
void place_stone(std::vector<field_coord> path, std::vector<field>* initial, int stone);
std::vector<field> stepping_stone_method(std::vector<field> initial_solution, std::vector<std::vector<unsigned>> cost_matrix);

#endif
