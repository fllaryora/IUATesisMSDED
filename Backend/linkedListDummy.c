#include<stdio.h>
#include<stdlib.h>

typedef struct{
        unsigned long long int  initialDelay;
        unsigned long long int  currentDelay;
        struct Worktask *next;
}Worktask;

void insert(Worktask *pointer, unsigned long long int currentDelay){
        /* Iterate through the list till we encounter the last node.*/
        while(pointer->next!=NULL)
        {
                pointer = pointer -> next;
        }
        /* Allocate memory for the new node and put data in it.*/
        pointer->next = (Worktask *)malloc(sizeof(Worktask));
        pointer = pointer->next;
        pointer->currentDelay = currentDelay;
        pointer->next = NULL;
}

int find(Worktask *pointer, unsigned long long int key){
        pointer =  pointer -> next; //First node is dummy node.
        /* Iterate through the entire linked list and search for the key. */
        while(pointer!=NULL)
        {
                if(pointer->currentDelay == key) //key is found.
                {
                        return 1;
                }
                pointer = pointer -> next;//Search in the next node.
        }
        /*Key is not found */
        return 0;
}

void delete(Worktask *pointer, unsigned long long int currentDelay){
        /* Go to the node for which the node next to it has to be deleted */
        while(pointer->next!=NULL && (pointer->next)->currentDelay != currentDelay)
        {
                pointer = pointer -> next;
        }
        if(pointer->next==NULL)
        {
                printf("Element %d is not present in the list\n",currentDelay);
                return;
        }
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
        return;
}

void print(Worktask *pointer){
        if(pointer==NULL)
        {
                return;
        }
        printf("%d ",pointer->currentDelay);
        print(pointer->next);
}

int main(){
        /* start always points to the first node of the linked list.
           temp is used to point to the last node of the linked list.*/
        Worktask *start,*temp;
        start = (Worktask *)malloc(sizeof(Worktask)); //nodo dummy
        temp = start;
        temp -> next = NULL;
        /* Here in this code, we take the first node as a dummy node.
           The first node does not contain data, but it used because to avoid handling special cases
           in insert and delete functions.
         */
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Print\n");
        printf("4. Find\n");
        while(1) {
                int query;
                scanf("%d",&query);
                if(query==1) {
                        unsigned long long int data;
                        scanf("%d",&data);
                        insert(start,data);
                }
                else if(query==2) {
                        unsigned long long int data;
                        scanf("%d",&data);
                        delete(start,data);
                }
                else if(query==3) {
                        printf("The list is ");
                        print(start->next);
                        printf("\n");
                }
                else if(query==4) {
                        unsigned long long int data;
                        scanf("%d",&data);
                        int status = find(start,data);
                        if(status) {
                                printf("Element Found\n");
                        }
                        else
                        {
                                printf("Element Not Found\n");

                        }
                }
        }


}
