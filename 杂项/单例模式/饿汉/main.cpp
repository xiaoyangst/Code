#include <iostream>
#include <thread>
#include "Singleton.h"
using namespace std;

void Test(){
  Singleton::getInstance()->printAddr();
}

int main() {


  std::thread varname1(&Test);
  std::thread varname2(&Test);
  std::thread varname3(&Test);
  std::thread varname4(&Test);


  varname1.join();
  varname2.join();
  varname3.join();
  varname4.join();

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