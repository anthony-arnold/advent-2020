#include <array>
#include <set>
#include <iostream>
#include <cmath>

namespace {
  const char* START = 
    "#.##....\n.#.#.##.\n###.....\n....##.#\n#....###\n.#.#.#..\n.##...##\n#..#.###\n";
  
  template <size_t Dim>
  using coord_t = std::array<int, Dim>;
    
  template <size_t X, size_t Dim>
  struct neighbours {
    template <typename F>
    static void apply(const coord_t<Dim>& from, F func) {
      auto look = from;
      for (int i = -1; i < 2; i++) {
        look[X] = from[X] + i;
        neighbours<X + 1, Dim>::apply(look, func);
      }
    }
  };
  
  template <size_t Dim>
  struct neighbours<Dim, Dim> {
    template <typename F>
    static void apply(const coord_t<Dim>& from, F func) {
      func(from);
    }
  };
  
  template <size_t Dim, typename S>
  auto find_off(const coord_t<Dim>& from, const S& on) {
    auto off = S();
    neighbours<0, Dim>::apply(from, [&on, &off](const auto& look) {
      if (on.find(look) == on.end()) {
        off.insert(look);
      }
    });
    return off;
  }
  
  template <size_t Dim, typename S>
  auto count_on(const coord_t<Dim>& from, const S& on) {
    auto count = 0;
    neighbours<0, Dim>::apply(from, [&on, &count](const auto& look) {
      count += on.find(look) != on.end();
    });
    return count;
  }
  
  template <size_t Dim>
  constexpr auto calc_on(size_t off) {
    return static_cast<size_t>(std::pow(3, Dim)) - 1 - off;
  }
  
  template <size_t Dim>
  auto cycle(const std::set<coord_t<Dim>>& on) {
    auto next = on;
    for (const auto& coord : on) {
      auto off = find_off(coord, on);
      auto n_on = calc_on<Dim>(off.size());
      if (n_on < 2 || n_on > 3) {
        next.erase(coord);
      }
      for (const auto& o : off) {
        if (count_on(o, on) == 3) {
          next.insert(o);
        }
      }
    }
    return next;
  }
  
  template <size_t Dim>
  auto read() {
    std::set<coord_t<Dim>> on;
    
    coord_t<Dim> x = { 0 };
    for (auto c = START; *c; c++) {
      if (*c == '\n') {
        x[0] = 0;
        x[1]++;
      }
      else {
        if (*c == '#') {
          on.insert(x);
        }
        x[0]++;
      }
    }
    
    return on;
  }
  
  template <size_t Dim>
  auto boot() {
    auto on = read<Dim>();
    for (int seq = 0; seq < 6; seq++) {
      on = cycle(on);
    }
    return on.size();
  }
}

int main() {
  std::cout << boot<3>() << std::endl;
  std::cout << boot<4>() << std::endl;
}
