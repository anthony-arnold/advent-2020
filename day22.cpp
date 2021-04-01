#include <iostream>
#include <list>
#include <sstream>
#include <numeric>
#include <array>
#include <tuple>
#include <unordered_set>


namespace {
auto hash_card(std::size_t seed, int card) {
    return seed ^ card + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T>
auto hash_deck(std::size_t seed, const T& deck) {
    for(auto i : deck) {
        seed = hash_card(seed, i);
    }
    return seed;
}

template <typename TDecks>
auto hash_state(const TDecks& v) {
    std::size_t seed = std::get<0>(v).size() +  std::get<1>(v).size() + 1;
    seed = hash_deck(seed, std::get<0>(v));
    seed = hash_card(seed, 0);
    seed = hash_deck(seed, std::get<1>(v));
    return seed;
}

template <typename TDecks>
class combat_strategy_base {
public:
    combat_strategy_base(TDecks d) : decks(d) {}

    auto play() {
        while (!(decks[0].empty() || decks[1].empty())) { 
            winner = play_round();
            if (winner < 0) {
                winner = 0;
                break;
            }
        }
        return winner;
    }
    
    auto score() const {
        auto deck = decks[winner];
        auto factor = 0;
        return std::accumulate(
            deck.rbegin(), 
            deck.rend(), 
            0,
            [&factor] (int sum, int val) {
                factor++;
                return sum + factor * val;
            }
        );
    }
protected:
    auto draw() {
        auto c1 = decks[0].front(), c2 = decks[1].front();
        decks[0].pop_front();
        decks[1].pop_front();
        return std::make_tuple(c1, c2);
    }

    auto claim(int c1, int c2, int w) {
        if (w) {
            std::swap(c1, c2);
        }
        this->decks[w].push_back(c1);
        this->decks[w].push_back(c2);

        return w;
    }

    virtual int play_round() = 0;

    TDecks decks;
    int winner;
};

template <typename TDecks>
class basic_combat_strategy : public combat_strategy_base<TDecks> {
public:
    basic_combat_strategy(TDecks& d) : combat_strategy_base<TDecks>(d) {}

protected:
    int play_round() override {
        auto [c1, c2] = this->draw();
        return this->claim(c1, c2, c1 < c2);
    }
};

template <typename TDecks>
class recursive_combat_strategy : public combat_strategy_base<TDecks> {
public:
    recursive_combat_strategy(TDecks& d) : combat_strategy_base<TDecks>(d) {}

protected:
    int play_round() override {
        if (!unique()) {
            return -1;
        }

        auto [c1, c2] = this->draw();

        if (can_recurse(c1, c2)) {
            auto cpy = TDecks(this->decks);
            cpy[0].resize(c1);
            cpy[1].resize(c2);
            return this->claim(
                c1, c2, recursive_combat_strategy<TDecks>(cpy).play()
            );
        }
        else {
            return this->claim(c1, c2, c1 < c2);
        }
    }
private:
    bool can_recurse(int c1, int c2) const {
        return this->decks[0].size() >= c1 && this->decks[1].size() >= c2;
    }

    bool unique() {
        return mem.insert(hash_state(this->decks)).second;
    }

    std::unordered_set<std::size_t> mem;
};

auto read_input() {
    std::array<std::list<int>, 2> decks;

    std::string line;
    int c;
    for(int i = 0; i < 2; i++) {
        std::getline(std::cin, line); // discard name
        std::getline(std::cin, line);
        while (line.length() > 0) {
            std::istringstream(line) >> c;
            decks[i].push_back(c);       
            std::getline(std::cin, line);
        }
    }

    return decks;
}
}
int main() {
    auto decks = read_input();
    basic_combat_strategy basic(decks);
    recursive_combat_strategy recursive(decks);

    basic.play(); 
    recursive.play();
    std::cout << basic.score() << std::endl;
    std::cout << recursive.score() << std::endl;
}
