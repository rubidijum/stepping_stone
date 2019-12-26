#include <iostream>
#include <vector>
#include <numeric>

#include "./transportation-problem/transportation_problem.hpp"

int main() {
    unsigned row;
    unsigned col;
    std::cin >> row >> col;

    std::vector< std::vector<unsigned>> cost_matrix;
    scan_matrix(cost_matrix, row, col);
    print_matrix(cost_matrix, row, col);
    std::vector<unsigned> offer;       
    std::vector<unsigned> demand;
    scan_vector(offer, row);
    scan_vector(demand, col);
    std::cout << "Offer: " << std::endl;
    print_vector(offer, row);
    std::cout << "Demand: " << std::endl;
    print_vector(demand, col);

    if (std::accumulate(offer.cbegin(), offer.cend(), 0) == std::accumulate(demand.cbegin(), demand.cend(), 0)) {
        std::cout << "Problem is saturated." << std::endl;
    }
    
    std::vector<field> NW_initial;
    std::cout << "North-west method: " << NW_method(cost_matrix, row, col, offer, demand, &NW_initial) << std::endl;

    std::vector<field> r = stepping_stone_method(NW_initial, cost_matrix);
    std::cout << "Stepping stone method:" << std::endl;
    int res = 0;
    for(auto i : r){        
        std::cout << "x" << i.first.first << "_" << i.first.second << " = " << i.second << std::endl;
        res += cost_matrix[i.first.first][i.first.second] * i.second; 
    }
    std::cout << res << std::endl;
    
    
    std::vector<field> Minimum_initial;
    std::cout << "Minimum method: " << min_method(cost_matrix, row, col, offer, demand, &Minimum_initial) << std::endl;
    
    r = stepping_stone_method(Minimum_initial, cost_matrix);
    std::cout << "Stepping stone method:" << std::endl;
    res = 0;
    for(auto i : r){        
        std::cout << "x" << i.first.first << "_" << i.first.second << " = " << i.second << std::endl;
        res += cost_matrix[i.first.first][i.first.second] * i.second; 
    }
    std::cout << res << std::endl;
    
    std::vector<field> Fogel_initial;
    std::cout << "Fogel's method: " << fogels_method(cost_matrix, row, col, offer, demand, &Fogel_initial) << std::endl;
    stepping_stone_method(Fogel_initial, cost_matrix);
    r = stepping_stone_method(Minimum_initial, cost_matrix);
    std::cout << "Stepping stone method:" << std::endl;
    res = 0;
    for(auto i : r){        
        std::cout << "x" << i.first.first << "_" << i.first.second << " = " << i.second << std::endl;
        res += cost_matrix[i.first.first][i.first.second] * i.second; 
    }
    std::cout << res << std::endl;
    
    return 0;
}

