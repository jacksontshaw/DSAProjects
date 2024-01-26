/*
Jackson Shaw
Guha
P6
COP3502C - 00282 
12/1/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19
#define TABLESIZE 300007

//global cash / complexity variables
int cash = 100000;
int complexity = 0;

//assigned structs
typedef struct item {
    char name[MAXLEN + 1];
    int quantity;
    int saleprice;
} item;

typedef struct node {
    item* iPtr;
    struct node* next;
} node;

typedef struct hashtable {
    node** lists;
    int size;
} hashtable;

int hashfunc(char* word, int size);
hashtable* create_table(int size);
void insert(hashtable* table, item* new_item);
node* find(hashtable* table, char* name);
void buy(hashtable* table, char* name, int quantity, int totalprice);
void sell(hashtable* table, char* name, int quantity);
void change_price(hashtable* table, char* name, int newprice);

int main(){
    //number of commands
    int n;
    scanf("%d", &n);

    //initialize table
    hashtable* table = create_table(TABLESIZE);

    for (int i = 0; i < n; i++){
        char command[MAXLEN + 1];
        scanf("%s", command);

        if (strcmp(command, "buy") == 0){
            char name[MAXLEN + 1];
            int quantity, totalprice;
            scanf("%s %d %d", name, &quantity, &totalprice);
            //scan in variables and run buy function
            buy(table, name, quantity, totalprice);
        } else if (strcmp(command, "sell") == 0){
            char name[MAXLEN + 1];
            int quantity;
            scanf("%s %d", name, &quantity);
            //scan in variables and run sell function
            sell(table, name, quantity);
        } else if (strcmp(command, "change_price") == 0){
            char name[MAXLEN + 1];
            int newprice;
            scanf("%s %d", name, &newprice);
            //scan in variables and run change_price
            change_price(table, name, newprice);
        }
    }
    //print cash and complexity

    printf("%d\n%d\n", cash, complexity);

    for (int i = 0; i < TABLESIZE; i++){
        //free linked lists node by node
        node* current = table->lists[i];
        while (current != NULL) {
            node* next = current->next;
            free(current->iPtr);
            free(current);
            current = next;
        }
    }
    //free array of linked lists, then free the table
    free(table->lists);
    free(table);

    return 0;
}
//hash function as assigned
int hashfunc(char* word, int size){
    int len = strlen(word);
    int res = 0;
    for (int i = 0; i < len; i++)
        res = (1151 * res + (word[i] - 'a')) % size;
    return res;
}

hashtable* create_table(int size){
    //allocate a table
    hashtable* table = (hashtable*)malloc(sizeof(hashtable));
    //allocate each linked list && update size
    table->lists = (node**)malloc(size * sizeof(node*));
    table->size = size;
    //initialize to null
    for (int i = 0; i < size; i++){
        table->lists[i] = NULL;
    }
    return table;
}

void insert(hashtable* table, item* new_item){
    //calculate index of item
    int index = hashfunc(new_item->name, table->size);
    node* new_node = (node*)malloc(sizeof(node));
    //initialize node and insert it into front of list at index
    new_node->iPtr = new_item;
    //set node's next to current head of list
    new_node->next = table->lists[index];
    table->lists[index] = new_node;
}

node* find(hashtable* table, char* name){
    //calculate index
    int index = hashfunc(name, table->size);
    node* current = table->lists[index];
    //add 1 to complexity no matter what 
    //then it gets added to again every time a node is traversed
    complexity += 1;
    //traverse through list until node is found
    while (current != NULL){
        if (strcmp(current->iPtr->name, name) == 0)
            return current;
        current = current->next;
        complexity += 1;
    }
    return NULL;
}

void buy(hashtable* table, char* name, int quantity, int totalprice){
    //start by subtracting price of purchase from the total
    cash -= totalprice;
    node* found = find(table, name);
    if (found == NULL){
        //if item doesn't exist, create a new node and insert it into the table
        item* new_item = (item*)malloc(sizeof(item));
        strcpy(new_item->name, name);
        new_item->quantity = quantity;
        new_item->saleprice = 1;  
        insert(table, new_item);
        //print the name, quantity, and updated cash number
        printf("%s %d %d\n", name, quantity, cash);
    } else {
        //if it does exist just update the quantity and print
        found->iPtr->quantity += quantity;
        printf("%s %d %d\n", name, found->iPtr->quantity, cash);
    }
}

void sell(hashtable* table, char* name, int quantity){ 
    node* found = find(table, name);
    if (found != NULL){
        //if sale quantity is within amount owned, subtract quantity
        if (found->iPtr->quantity >= quantity){
            found->iPtr->quantity -= quantity;
            //add total sale price to cash
            cash += (quantity * found->iPtr->saleprice);
            //print updated variables
            printf("%s %d %d\n", name, found->iPtr->quantity, cash);
        } else {
            //if sale quantity > stored quantity
            int sold = found->iPtr->quantity;
            found->iPtr->quantity = 0;
            //sell all that's left and update it to 0
            cash += (sold * found->iPtr->saleprice);
            //calculate sale price and print variables
            printf("%s 0 %d\n", name, cash);
        }
    }
}

void change_price(hashtable* table, char* name, int newprice){
    node* found = find(table, name);
    //if the node exists, update the sale price in the struct
    if (found != NULL){
        found->iPtr->saleprice = newprice;
    }
}
