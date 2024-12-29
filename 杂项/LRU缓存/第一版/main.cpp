#include <iostream>
#include "LRU.h"
using namespace std;


int main() {

  LRU<int,int> lru(5);

  lru.put(10,10);
  lru.put(30,30);
  lru.put(90,90);
  lru.show();
  if (lru.get(30) != std::nullopt) {
    cout << lru.get(30).value() << endl;
  }
  lru.show();
  lru.put(40,40);
  lru.show();
  lru.put(30,3);
  lru.show();
  lru.put(90,9);
  lru.show();
  lru.put(100,100);
  lru.show();
  lru.put(110,110);
  lru.show();
  if (lru.get(90) != std::nullopt) {
    cout << lru.get(90).value() << endl;
  }
  lru.show();
  return 0;

}

/*
(90,90)--->(30,30)--->(10,10)--->NULL
30
(30,30)--->(90,90)--->(10,10)--->NULL
(40,40)--->(30,30)--->(90,90)--->(10,10)--->NULL
(30,3)--->(40,40)--->(90,90)--->(10,10)--->NULL
(90,9)--->(30,3)--->(40,40)--->(10,10)--->NULL
(100,100)--->(90,9)--->(30,3)--->(40,40)--->(10,10)--->NULL
(110,110)--->(100,100)--->(90,9)--->(30,3)--->(40,40)--->NULL
-1
(110,110)--->(100,100)--->(90,9)--->(30,3)--->(40,40)--->NULL
*/