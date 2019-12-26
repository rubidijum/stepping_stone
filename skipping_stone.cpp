#include <iostream>
#include <vector>
#include <limits>
#include <numeric>
#include <algorithm>
 
typedef std::pair<int,int> field_coord;
typedef std::pair<field_coord, int> field;
 
bool find_path(field_coord entry, field_coord curr, std::vector<field_coord>* path, std::vector<field> possible_hops, bool direction);
int calculate_path_cost(std::vector<field_coord> path);
int find_stone(std::vector<field_coord> path, std::vector<field> initial);
void place_stone(std::vector<field_coord> path, std::vector<field>* initial, int stone);
bool stepping_stone_method(std::vector<field> initial_solution, std::vector<std::vector<int>> cost_matrix);
   
std::vector<std::vector<int>> cost_matrix{
        {20,11,15,13},
        {17,14,12,13},
        {15,12,18,18}
    };
 
int main() {
    
        
        
        std::vector<field> initial_solution{
            {{2,0},3},
            {{2,1},3},
            {{1,2},4},
            {{0,3},2},
            {{1,3},2},
            {{2,3},1}
        };
    
    
//     std::vector<field_coord> path_test{{0,2}};
//     find_path({0,2}, {0,2}, &path_test, initial_solution, true);
//     for(auto p_t : path_test){
//         std::cout << "(" << p_t.first << "," << p_t.second << std::endl;
//     }
    
        
     stepping_stone_method(initial_solution, cost_matrix);
        
    return 0;
}
    
bool stepping_stone_method(std::vector<field> initial_solution, std::vector<std::vector<int>> cost_matrix) {
 
        bool solution_found = false;
        
        while(!solution_found){
            std::vector<std::pair<int,int>> free_fields;
            auto row{cost_matrix.size()}, col{cost_matrix.at(0).size()};
    
            // Free fields initialization
            for(int i = 0; i < row; i++){
                for(int j = 0; j < col; j++){
                    std::pair<int,int> needle{i,j};
                    auto iter = std::find_if(initial_solution.cbegin(), initial_solution.cend(), [=] (field f) {return f.first == needle; });        
                    if(iter == initial_solution.cend()){
                        free_fields.push_back({i,j});
                    }
                }
            }
    
            // For each free node compute closed path
            std::vector<field_coord> path_{};    
            std::vector<std::pair<std::vector<field_coord>, int >> searchme;
        
            for(auto ff: free_fields){
                path_.push_back({ff.first, ff.second});
                find_path(ff, ff, &path_, initial_solution, true);
                for(auto pp : path_){
                    std::cout << "P(" << pp.first <<  "." << pp.second << ")P" << std::endl;
                }
                searchme.push_back({path_, calculate_path_cost(path_)});
                path_.clear();
            }
        
            // Find path with the most negative cost
            auto min = std::min_element(searchme.cbegin(),
                                                    searchme.cend(),
                                            [] (std::pair<std::vector<std::pair<int, int> >, int> a, std::pair<std::vector<std::pair<int, int> >, int> b) {
                                                return a.second < b.second;
                                            });
            
            if((*min).second >= 0){
                std::cout << "Optimal solution found" << std::endl;
                solution_found = true;
                break;
            }
        
            // Find all paths that have most negative cost
            int min_cost = (*min).second;
            std::vector<std::pair<std::vector<field_coord>, int >> minimums;
            std::copy_if(searchme.cbegin(), searchme.cend(), std::back_inserter(minimums), 
                        [min_cost] (std::pair<std::vector<field_coord>, int> path) { return path.second == min_cost; });
            
            // Choose the path with the largest stone
            int max_stone = minimums[0].second;
            std::cout << "INITIAL MAX STONE" << max_stone << std::endl;
            std::pair<std::vector<field_coord>, int > chosen_path = minimums[0];
            for(auto m : minimums){
                int stone = find_stone(m.first, initial_solution);
                std::cout << "FOUND STONE " << stone << std::endl;
                if (stone > max_stone){
                    max_stone = stone;
                    chosen_path = m;
                }
            }
            std::cout << "END MAX STONE" << max_stone << std::endl;
            
            std::cout << "CHOSEN PATH " << std::endl;
            for(auto cp : chosen_path.first){
                std::cout << cp.first << "," << cp.second << " - ";
            }
            std::cout << std::endl;
            
            
            // Update solution
            place_stone(chosen_path.first, &initial_solution, max_stone);
            std::cout << "Updated" << std::endl;
            for(auto is : initial_solution)
                std::cout << "[" << is.first.first << "," << is.first.second << "] ->" << is.second << std::endl;
            
        }
    }
 
// Find closed path for given field
// If it's found, it's the only one
bool find_path(field_coord entry, field_coord curr, std::vector<field_coord>* path, std::vector<field> possible_hops, bool direction){
 
    int row{curr.first};
    int col{curr.second};
   
    std::cout << "row " << row << "," << "col " << col << std::endl;
    
    if(direction){
        std::cout << "<->" << std::endl;
        if(row != entry.first || col != entry.second){
            // Loop can be closed
            if(row == entry.first){
                std::cout << "Closing loop horizontal" << std::endl;
                return true;
            }
        }
       
        std::vector<field> next_in_row_vec;
        std::copy_if(possible_hops.cbegin(), possible_hops.cend(), std::back_inserter(next_in_row_vec), [row](field f) { return f.first.first == row; });
       
        if(next_in_row_vec.size() == 0){
            // Loop cannot be closed => backtrack
            std::cout << "backtracking ... <->" << std::endl;
            std::cout << "pop <> " << path->back().first << "," << path->back().second << std::endl;
            path->pop_back();
            return false;
        }
       
       
       // Try building the path with the next field in the same row
        for(auto nirv : next_in_row_vec){
            field_coord next{nirv.first.first, nirv.first.second};
            path->push_back(nirv.first);
            auto rm = std::remove_if(possible_hops.begin(), possible_hops.end(), [=](auto k){ return k.first == next; });
            possible_hops.erase(rm, possible_hops.end());
            std::cout << "next: " << next.first << "," << next.second << std::endl;
            if(find_path(entry, next, path, possible_hops, !direction)){
                return true;
            }
        }
        std::cout << "popme ^v " << path->back().first << "," << path->back().second << std::endl;
        path->pop_back();
        return false;
    } else {
        std::cout << "^v" << std::endl;
        if(row != entry.first || col != entry.second){
            // Loop can be closed    
            if(col == entry.second){
                std::cout << "Closing loop vertical" << std::endl;
                return true;
            }          
        }
       
        std::vector<field> next_in_col_vec;
        std::copy_if(possible_hops.cbegin(), possible_hops.cend(), std::back_inserter(next_in_col_vec), [col](field f) { return f.first.second == col; });
 
        if(next_in_col_vec.size() == 0){
            // Loop cannot be closed => backtrack
            std::cout << "backtracking ... ^v" << std::endl;
            std::cout << "pop ^v " << path->back().first << "," << path->back().second << std::endl;
            path->pop_back();
            return false;
        }
       
        for(auto nirv : next_in_col_vec){
            field_coord next{nirv.first.first, nirv.first.second};
            path->push_back(nirv.first);
            auto rm = std::remove_if(possible_hops.begin(), possible_hops.end(), [=](auto k){ return k.first == next; });
            possible_hops.erase(rm, possible_hops.end());
            if(find_path(entry, next, path, possible_hops, !direction)){
                return true;
            }
        }
        std::cout << "popme ^v " << path->back().first << "," << path->back().second << std::endl;
        path->pop_back();
        return false;
    }
    std::cout << "pop end " << path->back().first << "," << path->back().second << std::endl;
    path->pop_back();
    return false;
}
 
int calculate_path_cost(std::vector<field_coord> path){
    std::cout << "Calculating path cost for " << path[0].first << "," << path[0].second << std::endl;
    
    std::vector<int> v(path.size());
    std::generate(v.begin(), v.end(), [n=-1] () mutable { return n*=-1; });
    std::vector<int> costs;
   
    for(auto p : path){
        costs.push_back(cost_matrix[p.first][p.second]);
        std::cout << "DSADASDAS" << p.first << "," << p.second << " = " << cost_matrix[p.first][p.second] << std::endl;
    }
   
    std::cout << std::inner_product(v.begin(), v.end(), costs.begin(), 0) << std::endl;
    return std::inner_product(v.begin(), v.end(), costs.begin(), 0);
}
 
// Finds minimum weight on a given path
int find_stone(std::vector<field_coord> path, std::vector<field> initial){
    char sign = 1;
    std::vector<field> costs;
    int minimum = 100000;
    std::cout << "MINIMUM " << minimum << std::endl;
    for(auto p : path){
        // find value of this field
        auto min = std::find_if(initial.cbegin(), initial.cend(), [=] (field f) { return p.first == f.first.first && p.second == f.first.second; });
        if((*min).second < minimum && sign == -1)
            minimum = (*min).second;
        sign *= -1;
    }
    return minimum;
}
 
// Update weights
void place_stone(std::vector<field_coord> path, std::vector<field>* initial, int stone){
    std::cout << "Placing stone " << stone << " on " << path[0].first << "," << path[0].second << std::endl;
    char sign = 1;
    for(auto p : path){
        auto init = std::find_if(initial->begin(), initial->end(), [p] (field f) { return f.first == p; } );
        if(init != initial->cend()){
            (*init).second += sign*stone;
            if((*init).second == 0)
                initial->erase(init);
        } else {
            initial->push_back({{p.first, p.second}, stone});
        }
        sign*=-1;
    }
}
