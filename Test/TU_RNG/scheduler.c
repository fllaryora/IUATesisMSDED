#include "scheduler.h"
#include "RNGs.h"
/* 
   Return the jackpot in drawnNumbers[stakeholderAccount -1]
*/
void Raffle(int* drawnNumbers, int stakeholderAccount){
	int temp;
	int newPos = stakeholderAccount > 0?RandomInt(0, stakeholderAccount):0;
	//swap drawnNumbers[stakeholderAccount] with drawnNumbers[newPos]
	temp =  drawnNumbers[newPos];
	drawnNumbers[newPos]  =  drawnNumbers[stakeholderAccount];
	drawnNumbers[stakeholderAccount]  = temp;	
}
