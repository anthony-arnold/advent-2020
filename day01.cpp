#include <iostream>
#include <unordered_set>

int main() {
    std::unordered_set<int> nums;
    int in;
    while (std::cin >> in) {
        nums.insert(in);
    }
    
    // Find the two inputs which sum to 2020
    for (auto n : nums) {
        auto m = 2020 - n;
        if (nums.find(m) != nums.end()) {
            std::cout << "Part 1: " << n * m << std::endl;
            break;
        }
    }
    
    // Find the input pairs which sum to <2020
    for (auto n : nums) {
        for (auto m : nums) {
            auto p = 2020 - (n + m);
            if (nums.find(p) != nums.end()) {
                std::cout << "Part 2: " << n * m * p << std::endl;
                return 0;
            }
        }
    }
}
