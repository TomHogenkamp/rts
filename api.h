#ifndef __API_H__
#define __API_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "InfoFIFO.h"
#include "memmap.h"

#define FIFO_SIZE 4

#define ERROR -1

#define TRUE 1
#define FALSE 0

#define EVEN 0
#define ODD 1

/*
#  Function: initFifo();
#  Description: Initializes the FIFO buffer
*/
void initFifo();

/*
#  Function: writeFifo(uint32_t data);
#  Description: Writes data to the FIFO. This is a circular buffer, so
#               when the FIFO is full, the oldest element will be deleted
#               from the FIFO and the new element will be pushed at the 
#               of the FIFO
*/
void writeFifo(uint32_t data);

/*
#  Function: isFifoEmpty()
#  Description: Returns true if the FIFO is empty.
#  Returns: 1 if the FIFO is empty
#           0 if the FIFO contains at least one element
*/
int isFifoEmpty();

/*
#  Function: readFifo(uint32_t* result, uint8_t even_odd)
#  Description: Reads one element from the FIFO
#  Returns: uint32_t One element from the FIFO
*/
uint32_t readFifo(uint32_t* result, uint8_t even_odd);

/*
#  Function: acquire();
#  Description: Acquires the lock to read from the FIFO
*/
void acquire();

/*
#  Function: release();
#  Description: Releases the lock from the FIFO
*/
void release();

/*
#  Function: isOddEvenRationingCorrect(uint8_t even_odd)
#  Description: Check whether the 'first in' element in the FIFO
#               is equal to the type current task (odd or even) .
#  Returns: 1 if current task is odd and 'first in' element in FIFO
#             is odd or if current task is even and 'first in' element
#             in FIFO is even
#           0 When task and 'first in' element do not match
*/
int isOddEvenRationingCorrect(uint8_t even_odd);

#endif







