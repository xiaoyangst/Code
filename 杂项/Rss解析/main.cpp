#include <iostream>
#include "ParseRSS.h"

int main() {
  ParseRSS give_me_a_name("coolshell.xml","pagelib.txt");
  give_me_a_name.parseRSS();
  give_me_a_name.dump();
  return 0;
}
