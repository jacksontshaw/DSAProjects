/*
Jackson Shaw
Guha
P5
COP3502C - 00282 
11/10/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19

typedef struct customer{
    char name[MAXLEN + 1];
    int points;
} customer;

typedef struct treenode{
    customer* cPtr;
    int size;
    struct treenode* left;
    struct treenode* right;
} treenode;

treenode* create_node(customer* cPtr);
treenode* insert(treenode* root, customer* cPtr);
treenode* search(treenode* root, char* name);
treenode* find_max(treenode* root);
treenode* delete(treenode* root, char* name);
int calculate_depth(treenode* root, char* name);
int count_smaller(treenode* root, char* name);
int compare_names(char* name1, char* name2);
int count_nodes(treenode* root);
void fill_array(treenode* arr[], treenode* root, int* index);
int compare_nodes(struct treenode* a, struct treenode* b);
void merge_sort(treenode* arr[], int low, int high);
void merge(treenode* arr[], int low, int mid, int high);

int main(){
    //initialize values
    int n;
    scanf("%d", &n);
    treenode* root = NULL;

    //loop through inputs
    for (int i = 0; i < n; i++) {
        char input[20], name[MAXLEN + 1];
        int points;
        scanf("%s %s", input, name);

        if (strcmp(input, "add") == 0) {
            scanf("%d", &points);
            treenode* found = search(root, name);
            if (found){
                //if customer already exists then update their points
                found->cPtr->points += points;
            } 
            else{
                //if customer doesn't exist, create and insert them
                customer* newCustomer = (customer*)malloc(sizeof(customer));
                strcpy(newCustomer->name, name);
                newCustomer->points = points;
                root = insert(root, newCustomer);
            }
            //print according value
            if (found) {
                printf("%s %d\n", name, found->cPtr->points);
            } 
            else {
                printf("%s %d\n", name, points);
            }
        } 
        else if(strcmp(input, "sub") == 0){
            scanf("%d", &points);
            //for every input, the node is ensured to be found before any action is taken
            treenode* found = search(root, name);
            if(found){
            //if subtraction will make points negative, make the customer's points 0
                if (points >= found->cPtr->points){
                    found->cPtr->points = 0; 
                }
                else{
                    found->cPtr->points -= points;
                }
                printf("%s %d\n", name, found->cPtr->points);
            }
            else{
                printf("%s not found\n", name);
            }
        }
        else if(strcmp(input, "del") == 0){
            treenode* found = search(root, name);
            if(found){
                //run delete function on input
                root = delete(root, name);
                printf("%s deleted\n", name);
            }
            else{
                printf("%s not found\n", name);
            }
        }
        else if(strcmp(input, "search") == 0){
            treenode* found = search(root, name);
            if(found){
                //calculate node's depth and print
                int depth = calculate_depth(root, name);
                printf("%s %d %d\n", name, found->cPtr->points, depth);
            }
            else{
                printf("%s not found\n", name);
            }
        } 
        else if(strcmp(input, "count_smaller") == 0){
            //run count_smaller function to search value
            int k = count_smaller(root, name);
            printf("%d\n", k);
        } 
    }

    //k = the number of nodes in the tree
    int k = count_nodes(root);

    //allocate an array of pointers of size k
    treenode** arr = (treenode**)malloc(k * sizeof(treenode*));

    //traverse the tree and fill the array
    int index = 0;
    fill_array(arr, root, &index);

    merge_sort(arr, 0, k - 1);

    //print array and free allocated memory
    for (int i = 0; i < k; i++) {
        printf("%s %d\n", arr[i]->cPtr->name, arr[i]->cPtr->points);
    }
    free(arr);

    return 0;
}

treenode* create_node(customer* cPtr){
    //copy over input data to a new node
    treenode* newNode = (treenode*)malloc(sizeof(treenode));
    newNode->cPtr = cPtr;
    newNode->size = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

treenode* insert(treenode* root, customer* cPtr){
    //base case
    if (root == NULL) return create_node(cPtr);

    int cmp = strcmp(cPtr->name, root->cPtr->name);
    //traverse left or right subtree based on strcmp
    if (cmp < 0) {
        root->left = insert(root->left, cPtr);
    } else if (cmp > 0){
        root->right = insert(root->right, cPtr);
    } 
    //update tree size
    root->size++;
    return root;
}

treenode* search(treenode* root, char* name){
    if (root == NULL) return NULL;
    //search tree until matching name is found
    int cmp = strcmp(name, root->cPtr->name);
    if (cmp == 0) return root;
    if (cmp < 0) return search(root->left, name);
    return search(root->right, name);
}

treenode* find_max(treenode* root){
    if (root == NULL){
        return NULL;
    }
    //finds max / right-most node in a subtree (used in delete function)
    while (root->right != NULL){
        root = root->right;
    }
    return root;
}

treenode* delete(treenode* root, char* name){
    treenode* temp;
    if (root == NULL){
        printf("not found\n");
    } else{
        int cmp = strcmp(name, root->cPtr->name);
        if (cmp < 0){
            root->left = delete(root->left, name);
        } else if (cmp > 0){
            root->right = delete(root->right, name);
        } else{
            //2 child case
            if (root->right && root->left){
                //find the maximum node in the left subtree
                treenode* maxLeft = find_max(root->left);
                //copy over that nodes data to the node being deleted
                strcpy(root->cPtr->name, maxLeft->cPtr->name);
                root->cPtr->points = maxLeft->cPtr->points;
                //delete max right node
                root->left = delete(root->left, maxLeft->cPtr->name);
                //update the size of the root node
                root->size--;
            } else{
                //1 child/no child cases
                temp = root;
                if (root->left == NULL)
                    root = root->right;
                else if (root->right == NULL)
                    root = root->left;
                free(temp);
            }
        }
    }
    return root;
}

int calculate_depth(treenode* root, char* name){
    if (root == NULL){
        return -1; 
    }
    int cmp = strcmp(name, root->cPtr->name);

    //node found
    if (cmp == 0){
        return 0; 
    } else if (cmp < 0){
        //node is in left subtree
        int leftDepth = calculate_depth(root->left, name);
        if (leftDepth != -1) {
            //increase depth count for each call
            return 1 + leftDepth;
        }
    } else{
        //node is in the right subtree
        int rightDepth = calculate_depth(root->right, name);
        if (rightDepth != -1){
            return 1 + rightDepth;
        }
    }

    return -1; 
}

int count_smaller(treenode* root, char* name){
    //run an inorder traversal
    if (root == NULL) {
        return 0;
    }
    int k = count_smaller(root->left, name);
    //add to k if the node name is smaller than name given
    if (compare_names(root->cPtr->name, name) < 0) {
        k++;
    }
    k += count_smaller(root->right, name);

    return k;
}

int compare_names(char* name1, char* name2){
    return strcmp(name1, name2);
}

int count_nodes(treenode* root){
    //add up number of nodes in the tree
    if (root == NULL){
        return 0;
    } else{
        //root + right nodes + left nodes
        return 1 + count_nodes(root->left) + count_nodes(root->right);
    }
}

void fill_array(treenode* arr[], treenode* root, int* index){
    //fill array with an inorder traversal
    if (root != NULL){
        fill_array(arr, root->left, index);
        //assign the current root node to the array & move index
        arr[*index] = root;  
        (*index)++;          
        fill_array(arr, root->right, index);
    }
}

int compare_nodes(struct treenode* a, struct treenode* b){
    //compare based on points
    if (a->cPtr->points != b->cPtr->points){
        return b->cPtr->points - a->cPtr->points;
    }

    //break ties based on name
    return strcmp(a->cPtr->name, b->cPtr->name);
}


void merge_sort(treenode* arr[], int low, int high){
    if (low < high){
        int mid = low + (high - low) / 2;

        //recursively sort the left and right halves
        merge_sort(arr, low, mid);
        merge_sort(arr, mid + 1, high);

        //merge the sorted halves
        merge(arr, low, mid, high);
    }
}

void merge(treenode* arr[], int low, int mid, int high){
    int i, j, k;
    int n1 = mid - low + 1;
    int n2 = high - mid;

    //create temporary arrays
    treenode* left[n1];
    treenode* right[n2];

    //copy data to temporary arrays left[] and right[]
    for (i = 0; i < n1; i++){
        left[i] = arr[low + i];
    }
    for (j = 0; j < n2; j++){
        right[j] = arr[mid + 1 + j];
    }

    //merge the temporary arrays back into arr[low..high]
    i = 0;
    j = 0;
    k = low;
    while (i < n1 && j < n2){
        //use the compare_nodes function to decide the order
        if (compare_nodes(left[i], right[j]) <= 0){
            arr[k++] = left[i++];
        } else{
            arr[k++] = right[j++];
        }

    }

    //copy the remaining elements of left[], if there are any
    while (i < n1){
        arr[k++] = left[i++];
    }

    //copy the remaining elements of right[], if there are any
    while (j < n2){
        arr[k++] = right[j++];
    }
}

