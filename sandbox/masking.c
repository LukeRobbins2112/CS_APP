#include <stdio.h>

void leastSigByte(int x){
  
  int leastSigByte = (x & 0xFF);
  printf("Least significant byte: 0x%.8X\n", leastSigByte);

}

void allButLeast(int x){

  int allButLeast = (x ^ ~0xFF);
  printf("All but least complemented: 0x%.8X\n", allButLeast);
    
}

void leastSigByteOnes(int x){

  int lsbo  = (x | 0xFF);
  printf("Least sig bytes set to 1: 0x%.8X\n", lsbo);
    
}


int main(){

  int x = 0x87654321;

  leastSigByte(x);
  allButLeast(x);
  leastSigByteOnes(x);
  
  return 0;

  
}
