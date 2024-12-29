#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>
#include "WordConverter.h"
using namespace std;

int main(int argc,char* argv[]){

  if (argc != 3){
    cerr<<"./demo.exe rule.txt target.txt"<<endl;
  }

  string rule(argv[1]);
  string target(argv[2]);

  WordConverter w1(target,rule);

  return 0;
}

/*
template<typename T>
void showContainer(const T& data){
  for (const auto& item : data) {
    cout << item <<" ";
  }
  cout<<endl;
}

int main(){

  set<int,greater<int>> data = {1,4,9,9,9,89};
  for (auto it = data.begin(); it != data.end();) {
    if (*it == 9){
      it = data.erase(it);
    }else{
      ++it;
    }
  }

  showContainer(data);

  return 0;
}
*/