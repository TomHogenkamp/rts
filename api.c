#include "api.h"
#include "io.h"
#include "kernel.h"	// Fixes implicit declarations (good imho)
#include "kernel2.h"

//start address of FIFO
uint32_t* sAddress;

//Start with full capacity
uint16_t fifoCapacity = FIFO_SIZE;

//semaphore
volatile uint8_t lock = 1;

volatile uint8_t oddOrEven = ODD;


void initFifo(uint32_t* startAddress){
    
  sAddress = startAddress;   
    
  writePos = 0;
  readPos = 0;
}

int isFifoEmpty(){
  if(fifoCapacity == FIFO_SIZE){
    return 1;
  } else return 0;
}

uint32_t readFifo(uint32_t* result, uint8_t even_odd){ 
  int i;  
  
  if(fifoCapacity == FIFO_SIZE){
    return ERROR; // FIFO is empty   
  }
  
  //Can only read from buffer if Odd-Even rationing is correct
  if(isOddEvenRationingCorrect(even_odd) == FALSE){
    return ERROR;  
  }

  *result = *(sAddress);
  
  //Shift data to the left, so 2nd oldest element in FIFO becomes at start
  for(i = 0; i < FIFO_SIZE-1; i++){
    *(sAddress+i) = *(sAddress+i+1); 
  }  
  
  fifoCapacity++;
  
  return 1;     
}

void writeFifo(uint32_t data){
  int i, pos;
  
  if(fifoCapacity == 0){ // FIFO is full
    //Remove first element, shift all and place new element at the end of the FIFO
    for(i = 0; i < FIFO_SIZE-1; i++){
      *(sAddress+i) = *(sAddress+i+1); 
    }
    *(sAddress+FIFO_SIZE-1) = data;
  }else {
    //calculate the spot in the FIFO
    pos = FIFO_SIZE - fifoCapacity; 
    
    //place data in the FIFO
    *(sAddress+pos) = data;
    
    fifoCapacity--;
  }
}

void release(){
  lock = 1;  
}

void acquire(){
  if(lock == 1){
    lock = 0;    
  } else {
    while(lock == 0){}
  }
}

int isOddEvenRationingCorrect(uint8_t odd_even){
  if(*(sAddress)%2 && odd_even == EVEN){ //odd == even
    return FALSE;
  } else if((*(sAddress)%2 == 0) && odd_even == ODD){ //even == odd
    return FALSE;  
  } else { //odd == odd || even == even
    return TRUE;  
  }
}







