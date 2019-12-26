#ifndef __TRANSPORTATION_PROBLEM_HPP
#define __TRANSPORTATION_PROBLEM_HPP

#include <vector>
#include <tuple>
#include <algorithm>
#include <climits>

#include "../stepping_stone.hpp"

/*  
    Utility functions
*/
void scan_vector(std::vector<unsigned>& vector, const unsigned size);
void scan_matrix( std::vector<std::vector<unsigned>> &cost
                  , const unsigned row
                  , const unsigned col);
void print_vector(const std::vector<unsigned>& vector, const unsigned size);
void print_matrix( const std::vector<std::vector<unsigned>> &cost
                   , const unsigned row
                   , const unsigned col);
// Input: matrix, number od rows and columns
// Output: array where a single element is an element from the given matrix and its position
std::vector<std::tuple<unsigned, unsigned, unsigned>> matrix2array
                    (const std::vector<std::vector<unsigned>> &matrix
                    , const unsigned row
                    , const unsigned col);
std::vector<std::vector<unsigned>> copy_matrix(const std::vector<std::vector<unsigned>> &cost_given);

/*
    Initial solution solvers
*/
unsigned NW_method( const std::vector<std::vector<unsigned>> &cost
                    , const unsigned row
                    , const unsigned col
                    , const std::vector<unsigned> &offer_given
                    , const std::vector<unsigned> &demand_given
                    , std::vector<field> *solution);

unsigned min_method( const std::vector<std::vector<unsigned>> &cost
                    , const unsigned row
                    , const unsigned col
                    , const std::vector<unsigned> &offer_given
                    , const std::vector<unsigned> &demand_given
                    , std::vector<field> *solution);

unsigned fogels_method(  const std::vector<std::vector<unsigned>> &cost_given
                , const unsigned row
                , const unsigned col
                , const std::vector<unsigned> &offer_given
                , const std::vector<unsigned> &demand_given
                , std::vector<field> *solution);

#endif
