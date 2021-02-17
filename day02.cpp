#include <iostream>
#include <string>
#include <algorithm>

struct password_t {
  int low, high, letter;
  std::string pw;
};


password_t read() {
    int low, high;
    char _, letter;
    std::string pw;
    if (!(std::cin >> low)) {
        return password_t {};
    }
    std::cin >> _ >> high >> letter >> _ >> pw;
    return password_t { low, high, letter, pw };
}

bool validate_1(const password_t& pw) {
  auto match = std::count(pw.pw.begin(), pw.pw.end(), pw.letter);
  return pw.low <= match && match <= pw.high;
}

bool validate_2(const password_t& pw) {
  int pair[] = { pw.pw[pw.low-1], pw.pw[pw.high-1] };
  return std::count(pair, pair + 2, pw.letter) == 1;
}

int main() {
  int p1 = 0, p2 = 0;
  while(std::cin.good()) {
    const auto& pw = read();
    if (pw.pw.size() == 0) {
      break;
    }
    if (validate_1(pw)) {
      p1++;
    }
    if (validate_2(pw)) {
      p2++;
    }
  }
  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;
}
