/*
Jackson Shaw
Guha
P1
COP3502C - 00282 
9/11/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITSIZE 10
#define MAXLEN 51
#define MAXROWS 100001

typedef struct order {
    int s_seat;
    int e_seat;
    char* name;
} order;

typedef struct theaterrow {
    order** list_orders;
    int max_size;
    int cur_size;
} theaterrow;

order* make_order(int start, int end, char *this_name);
theaterrow* make_empty_row();
int conflict(order* order1, order* order2);
int can_add_order(theaterrow* this_row, order* this_order);
void add_order(theaterrow* this_row, order* this_order);
char* get_owner(theaterrow** theater, int row, int seat_num);
char* get_row_owner(theaterrow* this_row, int seat_num);
int contains(order* myorder, int seat_no);
void free_order(order* this_order);
void free_row(theaterrow* this_row);

int main() {
    theaterrow** amc_theater = (theaterrow**)calloc(MAXROWS + 1, sizeof(theaterrow*));
    char action[MAXLEN];
    int row, start, end;
    char name[MAXLEN];
    char line[MAXLEN];

    while (fgets(line, sizeof(line), stdin) != NULL) {
        sscanf(line, "%s", action);

        if (strcmp(action, "BUY") == 0) {
            sscanf(line, "%*s %d %d %d %s", &row, &start, &end, name);
            order* new_order = make_order(start, end, name);
            if (amc_theater[row] == NULL) {
                amc_theater[row] = make_empty_row();
            }
            if (can_add_order(amc_theater[row], new_order)) {
                add_order(amc_theater[row], new_order);
                //attempt to add order to theater row
                printf("SUCCESS\n");
            } else {
                //order cannot be added
                printf("FAILURE\n");
                free_order(new_order);
            }
        } else if (strcmp(action, "LOOKUP") == 0) {
            sscanf(line, "%*s %d %d", &row, &start);
            if (amc_theater[row] == NULL) {
                printf("No one\n");
            } else {
                char* owner = get_owner(amc_theater, row, start);
                if (owner == NULL) {
                    printf("No one\n");
                } else {
                    printf("%s\n", owner);
                }
            }
            //if nobody has seat print no one, else print the owner
        } else if (strcmp(action, "QUIT") == 0) {
            break;
        }
    }
    //free memory
    for (int i = 0; i <= MAXROWS; i++) {
        if (amc_theater[i] != NULL) {
            free_row(amc_theater[i]);
        }
    }
    free(amc_theater);

    return 0;
}

order* make_order(int start, int end, char *this_name) {
    order* new_order = malloc(sizeof(order));
    new_order->s_seat = start;
    new_order->e_seat = end;
    new_order->name = malloc(MAXLEN * sizeof(char));
    strcpy(new_order->name, this_name);
    return new_order;
    //copy over starting and ending seats and the name to the new_order array
}

theaterrow* make_empty_row() {
    theaterrow* new_row = malloc(sizeof(theaterrow));
    new_row->list_orders = malloc(INITSIZE * sizeof(order*));
    new_row->max_size = INITSIZE;
    new_row->cur_size = 0;
    return new_row;
    //initialize theater row variables
}

int conflict(order* order1, order* order2) {
    return (order1->s_seat <= order2->e_seat && order1->e_seat >= order2->s_seat);
    //check if start and end seats of orders line up
}

int can_add_order(theaterrow* this_row, order* this_order) {
    for (int i = 0; i < this_row->cur_size; i++) {
        if (conflict(this_row->list_orders[i], this_order)) {
            return 0; //conflict between orders
        }
    }
    return 1; //no conflict
}

void add_order(theaterrow* this_row, order* this_order) {
    if (this_row->cur_size == this_row->max_size) {
        this_row->max_size *= 2;
        this_row->list_orders = realloc(this_row->list_orders, this_row->max_size * sizeof(order*));
        //double max orders allocated for row
    }
    this_row->list_orders[this_row->cur_size++] = this_order;
}

char* get_owner(theaterrow** theater, int row, int seat_num) {
    theaterrow* this_row = theater[row];
    if (this_row == NULL) {
        return NULL;
    }
    for (int i = 0; i < this_row->cur_size; i++) {
        if (contains(this_row->list_orders[i], seat_num)) {
            return this_row->list_orders[i]->name;
        }
    }
    //give seat owner if current seat falls within order
    return NULL;
}

char* get_row_owner(theaterrow* this_row, int seat_num) {
    for (int i = 0; i < this_row->cur_size; i++) {
        if (contains(this_row->list_orders[i], seat_num)) {
            return this_row->list_orders[i]->name;
        }
        //if seat_num in the row pointed to by this_row is occupied, return a
        //pointer to the string storing the owner’s name
    }
    return NULL;
}

int contains(order* myorder, int seat_no) {
    return (seat_no >= myorder->s_seat && seat_no <= myorder->e_seat);
    //check if seat number is within bounds of given order
}

void free_order(order* this_order) {
    free(this_order->name);
    free(this_order);
    //free this_order struct
}

void free_row(theaterrow* this_row) {
    for (int i = 0; i < this_row->cur_size; i++) {
        free_order(this_row->list_orders[i]);
    }
    free(this_row->list_orders);
    free(this_row);
    //free orders and row
}