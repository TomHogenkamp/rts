
#if 1

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <kernel.h>
#include <kernel2.h>
#include <kernelConst.h>
#include <api.h>
#include <InfoFIFO.h>
#include <fcntl.h>
#include <unistd.h>

uint32_t *shmaddr;
uint32_t *shmaddr_flag;
uint32_t numFifos=0;
uint32_t  apiDebug;
uint32_t A[10];

void task_write(){
  uint32_t data = 0;
  int j,k;
    
  printf("Starts task_write\n");

  for(j=0;j<10000000;j++){
      
    acquire();  
      
    writeFifo(data);
    
    //fprintf(stdout, "task_write %d %d\n",j, data);
    
    release();
    
    for(k=0;k<10000;k++){}
    
    data++;
    
  }
}

void task_read_even(){
  uint32_t result;
  int j;
  
  printf("Starts task_read_even\n");

  for(j=0;j<10000000;j++){
    
    acquire();  
    
    //If FIFO is empty, no data can be read
    if(readFifo(&result, EVEN) == ERROR){ 
      release();  
        
      continue;   
    }
    
    release();
            
    fprintf(stdout, "task_read_even %d >> %d <<\n",j,result);
  }
}

void task_read_odd(){
  uint32_t result;
  int i,j;
  
  printf("Starts task_read_odd\n");
  
  for(j=0;j<10000000;j++){


    acquire();  
      
    //If FIFO is empty, no data can be read
    if(readFifo(&result, ODD) == ERROR){ 
      release();  
        
      continue;   
    }
    
    release();
            
    fprintf(stdout, "task_read_odd %d >> %d <<\n",j,result);
  }
}


int main()
{
  volatile uint32_t *ptr, *ptr2;
  printf("main\n");

  while(*shmaddr_flag==0);
  printf("flag has been set\n");

  int i,j;

  uint32_t result;

  shmaddr=FIFO_ADMINISTRATION;
  //  initFifoInfo(shmaddr);

  numFifos=0;
  apiDebug=0;
  ptr=0x900000; // debug address

  *ptr=9999;
  printf("main1 %x\n",shmaddr);  


  //result = createFifo(shmaddr, 16*4, 1, 1);
  initFifo(shmaddr);

  InitTaskInfo(shmaddr);

  CreateNewTask(1, (uint32_t)&task_write, 1000000, 0);

  TaskStart(1); 

  InitKernel();

  for(j=0;j<1000;j++){
    for(i=0;i<1000;i++){
    }
  }
  return 0;

}

int main2()
{ 
  int i,j;

  uint32_t result;
  volatile uint32_t *ptr, *ptr2;

  shmaddr_flag=FIFO_ADMINISTRATION+0x10000;

  *shmaddr_flag=0;

  printf("begin main2 wait1\n");
  // wait loop
  for(i=0;i<100;i++){
  }
  printf("end main2 wait\n");

  *shmaddr_flag=1;

  ptr=0x900000; // debug address

  *ptr=9999;
  

  for(i=0;i<10000;i++){};

  printf("main2 %x\n",shmaddr); 

  numFifos=0;
  apiDebug=0;

  //InitTaskInfo2(shmaddr);

  CreateNewTask2(1, (uint32_t)&task_read_even, 50000, 0);
  CreateNewTask2(2, (uint32_t)&task_read_odd, 50000, 0);

  TaskStart2(1); 
  TaskStart2(2);


  InitKernel2();



  for(j=0;j<1000;j++){
    for(i=0;i<1000;i++){
    }
  }
  return 0;

}

#endif
 
