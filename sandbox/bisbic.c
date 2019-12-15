#include <stdio.h>

int bis (int x, int m){
  return x | m;
}

int bic (int x, int m){
  return x & (~m);
}

int bool_or(int x, int y){

  int result = bis(x, y);
  return result;
}

int bool_xor(int x, int y){
  int result = bis(bic(x, y), bic(y,x));
  return result;
}


int main(){

  short x = 0x1234; // 0001 0010 0011 0100
  short m = 0x0F0F; // 0000 1111 0000 1111

  int or = bool_or(x, m);
  int xor = bool_xor(x, m);

  printf("Or: %.4X\n", or);
  printf("Xor: %.4X\n", xor);

  return 0;
}
