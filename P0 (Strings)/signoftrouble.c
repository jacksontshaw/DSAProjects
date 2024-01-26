/*
Jackson Shaw
Prof. Guha
Problem 0
COP3502C - 00282 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() 
{
    //take input of strings and create frequency array for each

    char* original = malloc(100002 * sizeof(char));
    char* new = malloc(100002 * sizeof(char));
    
    fgets(original, 100002, stdin);
    fgets(new, 100002, stdin);

    int* freq_original = calloc(26, sizeof(int));
    int* freq_new = calloc(26, sizeof(int));

    //get length of strings
    //add frequency of each character in original message
    //then do same for new message
    //(excluding spaces)

    int length_original = strlen(original);
    int length_new = strlen(new);

    for (int i = 0; i < length_original; i++) {
        if (original[i] != ' ') {
            freq_original[original[i] - 'A']++;
        }
    }

    for (int i = 0; i < length_new; i++){
        if (new[i] != ' '){
            freq_new[new[i] - 'A']++;
        }
    }
    //compare for each letter and if the new amount is higher than original, 
    //add the difference to letters_needed variable 
 
    int letters_needed = 0;
    for (int i = 0; i < 26; i++){
        if (freq_new[i] > freq_original[i]){
            letters_needed += (freq_new[i] - freq_original[i]);
        } 
    }

    printf("%d\n", letters_needed);

    //print the final number of letters needed and free the strings/arrays

    free(original);
    free(new);
    free(freq_original);
    free(freq_new);

    return 0;
}