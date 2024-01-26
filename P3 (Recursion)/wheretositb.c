/*
Jackson Shaw
Guha
P3-B
COP3502C - 00282 
10/6/2023
*/

#include <stdio.h>
#include <string.h>

#define MAX_N 10
#define MAX_NAME_LEN 20

int n;
char names[MAX_N][MAX_NAME_LEN];
int popcorn[MAX_N];
int pairs[MAX_N][MAX_N];
int found = 0;

int is_valid(int perm[]);
void run_perms();
void print_perms(int perm[], int used[], int k);

int main() {
    int p;

    scanf("%d %d", &n, &p);

    for (int i = 0; i < n; i++) {
        scanf("%s %d", names[i], &popcorn[i]);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pairs[i][j] = 0;
        }
    }//initialize pairs array to 0

    for (int i = 0; i < p; i++) {
        char name1[MAX_NAME_LEN], name2[MAX_NAME_LEN];
        scanf("%s %s", name1, name2);

        int index1 = -1;
        int index2 = -1;
        for (int j = 0; j < n; j++) {
            if (strcmp(names[j], name1) == 0) {
                index1 = j;
            }
            if (strcmp(names[j], name2) == 0) {
                index2 = j;
            }
        }//set index1 and index2 to the names that cant sit next to eachother

        if (index1 != -1 && index2 != -1) {
            pairs[index1][index2] = 1;
            pairs[index2][index1] = 1;
        }//put them in the pairs array with both possible orderings
    }

    printf("\n");

    run_perms();



    return 0;
}

void run_perms() {
    int perm[MAX_N];
    int used[MAX_N];

    for (int i = 0; i < n; i++) {
        used[i] = 0;
    }

    print_perms(perm, used, 0);
    printf("\n");
}

int is_valid(int perm[]) {
    for (int i = 0; i < n; i++) {
        int current = perm[i];
        int left;
        int right;

        if (i == 0) {//seperate case for left-most customer
            left = -1;                      
            right = perm[1];      
        } else if (i == n - 1) {//seperate case for right-most customer
            left = perm[n - 2];           
            right = -1;                   
        } else {//everyone else
            left = perm[i - 1];           
            right = perm[i + 1];      
        }
        if (!(popcorn[current] || (left != -1 && popcorn[left]) || (right != -1 && popcorn[right]))) {
            return 0;//check if customers to left and right have popcorn or not,
                    //only check right/left for first/last customer
        }
        if(pairs[current][left] && left != -1){
            return 0;//check if left is an allowed pair
        }
        if(pairs[current][right] && right != -1){
            return 0;//check if right is an allowed pair
        }
    }
    return 1; 
}

void print_perms(int perm[], int used[], int k) {
    if (k == n && !found) {
        if (is_valid(perm)) {//base case
            for (int i = 0; i < n; i++) {
                printf("%s ", names[perm[i]]);
                printf("\n");
            }
            found = 1;  //found variable used to exit recursive loop
        }
    } 
    else if(!found){ 
        for (int i = 0; i < n; i++) {
            if (!used[i]) {
                used[i] = 1;
                perm[k] = i;
                print_perms(perm, used, k + 1);
                //recursively run through all permutations
                used[i] = 0;
            }
        }
    }
}


