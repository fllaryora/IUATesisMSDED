#include<stdio.h>
#include<stdlib.h>

#define TIME_TO_DELTA_T 1

typedef struct WorkTaskType{
        long long int  initialDelay;
        long long int  currentDelay;
        struct WorkTaskType *next;
}Worktask;

void insert(Worktask *pointer, unsigned long long int currentDelay,  unsigned long long int  initialDelay){
        /* Iterate through the list till we encounter the last node.*/
        while(pointer->next!=NULL)
        {
                pointer = pointer -> next;
        }
        /* Allocate memory for the new node and put data in it.*/
        pointer->next = (Worktask *)malloc(sizeof(Worktask));
        pointer = pointer->next;
        pointer->currentDelay = currentDelay;
        pointer->initialDelay = initialDelay;
        pointer->next = NULL;
}

int delete(Worktask *pointer){
	int deletedWorkTask = 0;
	while( pointer->next!=NULL ){
    /* Go to the node for which the node next to it has to be deleted */
    /*la logica no es borrar donde estoy parado, sino que del anterior borras el siguiente, uso el dummy */
    while(pointer->next!=NULL && (pointer->next)->currentDelay > 0)
    {
		(pointer->next)->currentDelay--;
        pointer = pointer -> next;
    }
    if(pointer->next!=NULL){
        /* Now pointer points to a node and the node next to it has to be removed */
        Worktask *temp;
        temp = pointer -> next;
        /*temp points to the node which has to be removed*/
        pointer->next = temp->next;
        /*We removed the node which is next to the pointer (which is also temp) */
        free(temp);
        /* Beacuse we deleted the node, we no longer require the memory used for it . 
           free() will deallocate the memory.
         */
         deletedWorkTask++;
    }
    
    }
    return deletedWorkTask;
}

void print(Worktask *pointer){
        if(pointer==NULL)
        {
                return;
        }
        printf("%d ",pointer->currentDelay);
        print(pointer->next);
}



void delayOfWorktask(Worktask *pointer, int bodyResource){
    double* humanDelay = (double*) malloc( sizeof(double)* 2 * bodyResource );
    int i ;
    int currentDelay = 0;
    
    //printf("------%e     %e", (double)pointer->currentDelay / (double)TIME_TO_DELTA_T, (double)pointer->initialDelay/ (double)TIME_TO_DELTA_T);
    //count doun
    // printf("humanDelay[currentDelay]= %p == humanDelay = %p", &humanDelay[currentDelay], humanDelay);
    printf("bodyResourCE ---%d\n", bodyResource);
    do{
        if(currentDelay != 0)
            pointer = pointer -> next;

        humanDelay[currentDelay] = (double)pointer->currentDelay / (double)TIME_TO_DELTA_T ;
        printf("--%f  \n ", humanDelay[currentDelay]);
        humanDelay[currentDelay + bodyResource] = (double)pointer->initialDelay /(double)TIME_TO_DELTA_T;
        printf("--%f  \n ", humanDelay[currentDelay + bodyResource]);
        currentDelay++;
        
    } while( pointer->next != NULL);
    printf("-------\n ");
    
    for(i = 0; i < bodyResource * 2; i++){
        printf("%e ", humanDelay[i]);
    }

   free(humanDelay);
}

int main(){
        /* start always points to the first node of the linked list.
           temp is used to point to the last node of the linked list.*/
        Worktask *start;
        int bodyResource = 0;
        start = (Worktask *)malloc(sizeof(Worktask)); //nodo dummy
        start -> next = NULL;
        /* Here in this code, we take the first node as a dummy node.
           The first node does not contain data, but it used because to avoid handling special cases
           in insert and delete functions.
         */

        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Print\n");
        printf("4. PrintDos\n");
        while(1) {
                int query;
                scanf("%d",&query);
                if(query==1) {
                        unsigned long long int data;
                        scanf("%ld",&data);
                        insert(start,data,data);
                        bodyResource++;
                }
                else if(query==2) {
                        bodyResource -= delete(start);
                }
                else if(query==3) {
                        printf("The list is ");
                        print(start->next);
                        printf("\n");
                }
                else if(query==4) {
                        printf("The list is ");
                        delayOfWorktask(start->next, bodyResource);
                        printf("\n");
                }
        }


}
