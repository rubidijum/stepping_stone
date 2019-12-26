#include <iostream>
#include <vector>
#include <numeric>

#include "main.hpp"

int main() {
    unsigned row;
    unsigned col;
    std::cin >> row >> col;

    std::vector< std::vector<unsigned>> cost;
    scan_matrix(cost, row, col);
    print_matrix(cost, row, col);
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
    
    std::cout << "North-west method: " << NW_method(cost, row, col, offer, demand) << std::endl;
    std::cout << "Minimum method: " << min_method(cost, row, col, offer, demand) << std::endl;
    std::cout << "Fogel's method: " << fogels_method(cost, row, col, offer, demand) << std::endl;
    
    return 0;
}

