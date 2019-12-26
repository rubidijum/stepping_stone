#include "stepping_stone.hpp"
    
std::vector<field> stepping_stone_method(std::vector<field> initial_solution, std::vector<std::vector<unsigned>> cost_matrix) {
 
        bool solution_found = false;
        std::vector<field> result;
        
        while(!solution_found){
            std::vector<std::pair<int,int>> free_fields;
            auto row{cost_matrix.size()}, col{cost_matrix.at(0).size()};
    
            // Free fields initialization
            for(unsigned i = 0; i < row; i++){
                for(unsigned j = 0; j < col; j++){
                    std::pair<unsigned,unsigned> needle{i,j};
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
                searchme.push_back({path_, calculate_path_cost(path_, cost_matrix)});
                path_.clear();
            }
        
            // Find path with the most negative cost
            auto min = std::min_element(searchme.cbegin(),
                                                    searchme.cend(),
                                            [] (std::pair<std::vector<std::pair<unsigned, unsigned> >, int> a, std::pair<std::vector<std::pair<unsigned, unsigned> >, int> b) {
                                                return a.second < b.second;
                                            });
            
            if((*min).second >= 0){
                std::cout << "Optimal solution found" << std::endl;
                return initial_solution;
            }
        
            // Find all paths that have most negative cost
            int min_cost = (*min).second;
            std::vector<std::pair<std::vector<field_coord>, int >> minimums;
            std::copy_if(searchme.cbegin(), searchme.cend(), std::back_inserter(minimums), 
                        [min_cost] (std::pair<std::vector<field_coord>, int> path) { return path.second == min_cost; });
            
            // Choose the path with the largest stone
            int max_stone = 0;
            std::pair<std::vector<field_coord>, int > chosen_path = minimums[0];
            for(auto m : minimums){
                int stone = find_stone(m.first, initial_solution);
                if (stone > max_stone){
                    max_stone = stone;
                    chosen_path = m;
                }
            }           

            // Update solution
            place_stone(chosen_path.first, &initial_solution, max_stone);
            
        }
        //TODO: safety
        return result;
    }
 
// Find closed path for given field
// If it's found, it's the only one
bool find_path(field_coord entry, field_coord curr, std::vector<field_coord>* path, std::vector<field> possible_hops, bool direction){
 
    auto row{curr.first};
    auto col{curr.second};
    
    if(direction){
        if(row != entry.first || col != entry.second){
            // Loop can be closed
            if(row == entry.first){
                return true;
            }
        }
       
        std::vector<field> next_in_row_vec;
        std::copy_if(possible_hops.cbegin(), possible_hops.cend(), std::back_inserter(next_in_row_vec), [row](field f) { return f.first.first == row; });
       
        if(next_in_row_vec.size() == 0){
            // Loop cannot be closed => backtrack
            path->pop_back();
            return false;
        }
       
       
       // Try building the path with the next field in the same row
        for(auto nirv : next_in_row_vec){
            field_coord next{nirv.first.first, nirv.first.second};
            path->push_back(nirv.first);
            auto rm = std::remove_if(possible_hops.begin(), possible_hops.end(), [=](auto k){ return k.first == next; });
            possible_hops.erase(rm, possible_hops.end());
            if(find_path(entry, next, path, possible_hops, !direction)){
                return true;
            }
        }
        path->pop_back();
        return false;
    } else {
        if(row != entry.first || col != entry.second){
            // Loop can be closed    
            if(col == entry.second){
                return true;
            }          
        }
       
        std::vector<field> next_in_col_vec;
        std::copy_if(possible_hops.cbegin(), possible_hops.cend(), std::back_inserter(next_in_col_vec), [col](field f) { return f.first.second == col; });
 
        if(next_in_col_vec.size() == 0){
            // Loop cannot be closed => backtrack
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
        path->pop_back();
        return false;
    }
    path->pop_back();
    return false;
}
 
int calculate_path_cost(std::vector<field_coord> path, std::vector<std::vector<unsigned>> cost_matrix){
    std::vector<int> v(path.size());
    std::generate(v.begin(), v.end(), [n=-1] () mutable { return n*=-1; });
    std::vector<int> costs;
   
    for(auto p : path)
        costs.push_back(cost_matrix[p.first][p.second]);
   
    return std::inner_product(v.begin(), v.end(), costs.begin(), 0);
}
 
// Finds minimum weight on a given path
int find_stone(std::vector<field_coord> path, std::vector<field> initial){
    char sign = 1;
    std::vector<field> costs;
    unsigned minimum = 100000;
    for(auto p : path){
        if(sign == -1){
            // find value of this field
            auto min_ = std::find_if(initial.cbegin(), initial.cend(), [=] (field f) { return p.first == f.first.first && p.second == f.first.second; });
            if(min_ != initial.cend()){
                if((*min_).second < minimum)
                    minimum = (*min_).second;
            }
        }
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
