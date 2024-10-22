#include <stdio.h>
#include "head.h"

void TestINI(){
  const char* path = "config.ini";

  char* password = parseINI("password", path);
  if (password) {
    printf("password=%s\n", password);
    free(password);
  } else {
    printf("Key 'password' not found.\n");
  }

}

int main(void) {


  TestINI();

  return 0;
}
