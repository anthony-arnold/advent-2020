#include <string>
#include <sstream>
#include <iostream>

namespace {
   using big_t = std::uint64_t;

   big_t expr(std::istream& str, bool sub = false);
   big_t tok(std::istream& str) {
      char c;
      str >> c;
      if (c == '(') {
	 return expr(str, true);
      }

      str.putback(c);
      big_t i;
      str >> i;
      return i;
   }
   big_t expr(std::istream& str, bool sub) {
      big_t v = tok(str);

      char op;
      while ((str >> op).good()) {
	 switch(op) {
	    case '+':
	       v += tok(str);
	       break;
	    case '*':
	       v *= expr(str);
	       break;
	    default:
	       if (!sub) {
		  str.putback(op);
	       }
	       return v;
	 }
      }
      return v;
   }
}

int main() {
   using std::getline;
   std::string s;
   big_t sum = 0;
   while (getline(std::cin, s)) {
      std::istringstream ss(s);
      sum += expr(ss);
   }
   std::cout << sum << std::endl;
}
