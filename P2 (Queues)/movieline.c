/*
Jackson Shaw
Guha
P2
COP3502C - 00282 
9/24/2023
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 12
#define MAXTIME 2147483647

typedef struct customer {
    char name[51];
    int num_tickets;
    int line_number;
    int arrival_time;
} customer;

typedef struct node {
    customer* customer;
    struct node* next;
} node;

typedef struct queue {
    node* front;
    node* back;
    int size;
} queue;

customer* create_customer(char* temp_name, int num_tickets, int arrival_time);
void enqueue(queue* line, customer* cust);
int is_empty(queue* line);
customer* peek(queue* line);
void free_customer(customer* cust);
void dequeue(queue* line);
void exit_booths(queue* transfer, int qpb, int b, int extra_queue);
int is_booth_empty(queue* this_booth, int booth_size);

int main() {
    int n; //num customers
    int b; //num booths
    int num_tickets;
    int arrival_time;
    char* temp_name = malloc(51 * sizeof(char));
    

    queue* lines = (queue*)calloc(MAXLINES + 1, sizeof(queue));
    //to deal with line 0 I just made the array size 13 and will be ignoring index 0

    for (int i = 0; i < MAXLINES + 1; i++) {
        lines[i].front = NULL;
        lines[i].back = NULL;
        lines[i].size = 0;
    }

    scanf("%d %d", &n, &b); 

    for (int i = 0; i < n; i++) {
        scanf("%s %d %d", temp_name, &num_tickets, &arrival_time);
        customer* cust = create_customer(temp_name, num_tickets, arrival_time);
        enqueue(lines, cust); 
        //transfer each customer to customer struct pointer and enqueue
    }

    int k = 0;
    int* filled_queues = (int*)calloc(13, sizeof(int));
    //array containing line numbers of non empty queues

    for (int i = 0; i < 13; i++ ){
        if(is_empty(&lines[i]) == 0){

            filled_queues[k] = i;            
            k++;
        }//k = number of filled queues
    }

    int qpb = k / b; //queues per booth
    int extra_queue = k % b; //booths with 1 extra queue

    queue* transfer = (queue*)calloc(k, sizeof(queue));
    //array of queues containing non-empty queues - used to split into booths

    for(int i = 0; i < k; i++){
        transfer[i] = lines[filled_queues[i]];
    }

    int booth_number = 1;

    exit_booths(transfer, qpb, b, extra_queue);
    //exit function splits the lines into their respective booths
    //and prints their exits from the simulation


    free(temp_name);
    free(lines); //free the lines array itself & filled_queues
    free(filled_queues);

    return 0;
}



customer* create_customer(char* temp_name, int num_tickets, int arrival_time) {
    customer* custptr = (customer*)malloc(sizeof(customer));
    if (custptr != NULL) {
        strncpy(custptr->name, temp_name, 51);
        custptr->num_tickets = num_tickets;
        custptr->line_number = ((temp_name[0] - 'A') % 13);
        custptr->arrival_time = arrival_time;
    }
    //transfer info to customer struct pointer and return the pointer
    return custptr;
}

void enqueue(queue* lines, customer* cust) {
    if (cust->line_number == 0) {
        //for if customers calculated line number is 0
        int low_size = 10; 
        int lowest_queue = 0;

        for (int i = 1; i < MAXLINES + 1; i++) {
            if (!is_empty(&lines[i])) {
                if (lines[i].size < low_size) {
                    low_size = lines[i].size;
                    lowest_queue = i;
                    //find the number of the lowest non-empty line
                }
            }
        }

        cust->line_number = lowest_queue;
        //add to the lowest non-empty queue 
    }

    int line_number = cust->line_number;
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node != NULL) {
        new_node->customer = cust;
        new_node->next = NULL; //initialize the node

        if (is_empty(&lines[line_number])) {
            lines[line_number].front = new_node;
            lines[line_number].back = new_node;
            //if the line is empty, front and back equal the new node
        } else {
            if (lines[line_number].back == NULL) {
                //handle the case where the back of the line is NULL separately
                lines[line_number].front->next = new_node;
                lines[line_number].back = new_node;
            } else {
                lines[line_number].back->next = new_node;
                lines[line_number].back = new_node;
            }
            //if not empty, add it to the back and link it to the previous node
        }
        lines[line_number].size++;
    }
}

int is_empty(queue* line) {

    if (line->size == 0){
        return 1;
    }
    return 0;

    //return 1 if the line is empty and 0 if not
}

customer* peek(queue* line) {
    if (!is_empty(line)) {
        return line->front->customer;
    }
    return NULL;//return the front customer if the line is not empty
}

void free_customer(customer* cust) {
    free(cust);
}

void dequeue(queue* line) {
  
    if (is_empty(line)) {
        return;
    }
    //return if queue is empty

    node* temp = line->front;
    line->front = temp->next;
    //move the front of queue pointer to temp->next

    free(temp);
    //free node, reduce size of queue

    line->size--;
}

int is_booth_empty(queue* this_booth, int booth_size) {
    for (int i = 0; i < booth_size; i++) {
        if (!is_empty(&this_booth[i])) {
            return 0; 
        }
    }
    return 1; //return 1 if all queues are empty
}

void exit_booths(queue* transfer, int qpb, int b, int extra_queue) {
    int booth_number = 1;
    int ptr = 0;

    for (int i = 0; i < b; i++) {
        printf("Booth %d\n", booth_number);

        int booth_size = qpb + (i < extra_queue ? 1 : 0);
        //for the first extra_queue booths, booth_size = qpb + 1

        while (!is_booth_empty(&transfer[ptr], booth_size)) {
            int lowest_time = MAXTIME;
            int lowest_cust = -1;//set to invalid values that are guaranteed to be changed

            for (int j = 0; j < booth_size; j++) {
                customer* front_customer = peek(&transfer[ptr + j]);
                if (front_customer != NULL &&
                    front_customer->arrival_time < lowest_time) {
                    lowest_time = front_customer->arrival_time;
                    lowest_cust = j;
                }
            }//scan through customers in the booth to find the lowest arrival time

            if (lowest_cust != -1) {
                customer* checkout_customer = peek(&transfer[ptr + lowest_cust]);
                int checkout_time = ((((checkout_customer->num_tickets) * 5) + 30) + lowest_time);

                printf("%s from line %d checks out at time %d.\n", checkout_customer->name, checkout_customer->line_number, checkout_time);

                dequeue(&transfer[ptr + lowest_cust]);
            }//if theres a valid lowest customer, print their checkout time and dequeue them
        }

        booth_number++;
        ptr += booth_size;

        //increase booth number and add booth_size to ptr so that the next
        //booth begins from index ptr+booth_size

        printf("\n");
    }
}