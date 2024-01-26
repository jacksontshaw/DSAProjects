/*
Jackson Shaw
Guha
P4
COP3502C - 00282 
10/27/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_N 500000

typedef struct {
    double angle;
    int people;
} group;

void merge_sort(group arr[], int low, int high);
void merge(group arr[], int low, int mid, int high);

int main() {

    int n;
    int a;
    scanf("%d %d", &n, &a);

    group *groups = (group*)malloc(2 * n * sizeof(group));

    //scan in number of people and angle, and allocate groups array

    for (int i = 0; i < n; i++) {
        
        int x;
        int y;
        int s;

        scanf("%d %d %d", &x, &y, &s);
        double angle = atan2(y, x) * 180 / M_PI;
        if (angle < 0) {
            angle += 360;
        }//calculate angle from x and y coordinates
        //convert from radians to degrees (0-360)

        groups[2 * i].angle = angle;
        groups[2 * i].people = s;
        groups[2 * i + 1].angle = angle + 360;
        groups[2 * i + 1].people = s;
        //fill groups with angle and the number of people
        //groups are stored twice, to consider if range wraps 
        //around the positive x axis
    }

    merge_sort(groups, 0, 2 * n - 1);

    int min_people = MAX_N; 
    //initialize min_people with the maximum possible value

    for (int i = 0; i < 2 * n; i++) {
        double gap = groups[i + 1].angle - groups[i].angle;
        if (gap > a) {
            min_people = 0;
            break;
            //if the gap between 2 groups > a, 
            //then there will be nobody in projection
        }
        else{
            
            int j = i;
            int angle;
            int in_proj = groups[i].people;

            while(angle <= a){
                angle = groups[j + 1].angle - groups[i].angle;
                in_proj += groups[j + 1].people;
                j++;
            }
            if(in_proj <= min_people){
                min_people = in_proj;
            }
        }
        //for each group, add adjacent groups until they make an angle > a
        //compare the num of people in projection to min_people
    }

    double max_angle = 0;

    for (int i = 0; i < 2 * n; i++) {
        double gap = groups[i + 1].angle - groups[i].angle;
        if (gap > max_angle) {
            max_angle = gap;
        }
        //calculate maximum angle gap between 2 groups 
    }       

    printf("%d\n%.4f\n", min_people, max_angle);
    free(groups);

    return 0;
}

void merge_sort(group arr[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        merge_sort(arr, low, mid);
        merge_sort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
    //run merge sort algorithm on arr
}

void merge(group arr[], int low, int mid, int high) {
    int i = low, j = mid + 1, k = 0;
    group temp[high - low + 1];
    //initialize variables and temp array

    while (i <= mid && j <= high) {
        if (arr[i].angle <= arr[j].angle) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= high) {
        temp[k++] = arr[j++];
    }

    for (i = 0; i < k; i++) {
        arr[low + i] = temp[i];
    }
    //merge the 2 lists into temp based on angle
}

