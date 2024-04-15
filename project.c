#include<stdio.h>
#include<stddef.h>
typedef enum { allocated , free } status;
typedef enum { false , true } boolean;
#define heap_size 30000

char heap[heap_size];

typedef struct meta_data{
    status curr_status;
    size_t size;
    struct meta_data* next;
}meta_data;

#define threshold sizeof(meta_data)

typedef struct Node
{
    short int data;
    short int ptrCount;
    short int inDegree;
    short int marked;
    struct Node *next[3];
} Node;

typedef struct Root
{
    Node *root;
} Root;

meta_data* strt = (void*)heap;

void initialise_heap(){
    strt -> curr_status = free;
    strt -> size = heap_size - threshold;
    strt -> next = NULL; 
    printf("successfully initialised heap\n\n");
}

void display(){
    meta_data* trav = strt;
    while(trav){
        printf("address of block  :  %p\n" , trav);
        printf("status of the block  :  ");
        if(trav -> curr_status == free) printf("free\n\n");
        else printf("allocated\n\n");
        trav = trav -> next;
    }
}

void* allocate(size_t size_request){
    void* ret_val = NULL;
    if(size_request <= 0){
        printf("size request of zero or negative bytes cannot be allocated\n\n");
    }
    else{
        meta_data* trav = strt;
        boolean found = false;
        while( found == false && trav != NULL ){
            if( trav -> curr_status == free ){
                if(trav -> size >= size_request){
                    found = true;
                }
            }
            else trav = trav -> next;
        }
        if(trav == NULL){
            printf("required size cannot be allocated\n\n");
        }
        else{
            ret_val = trav + 1;
            trav -> curr_status = allocated;
            if(trav -> size - size_request > threshold){
                meta_data* new_data = (meta_data*)(trav + size_request + threshold);
                new_data -> curr_status = free;
                new_data -> size = (trav -> size - size_request - threshold);
                new_data -> next = trav -> next;
                trav -> next = new_data;
            } 
            printf("succesfully allocated size\n\n");
        }
    }
    return ret_val;
}

void deallocate(void* p){
    if(p == NULL){
        printf("give valid pointer to deallocate\n\n");
    }
    else{
        meta_data* prev = NULL;
        meta_data* trav = strt;
        meta_data* strt_address = p - threshold;
        while(trav && trav != strt_address){
            prev = trav;
            trav = trav -> next;
        }
        if(trav == NULL){
            printf("- * - succesfully freed up the memory - * -  ");
        }
        else{
            if(prev == NULL || prev -> curr_status == allocated){
                meta_data* next_block = trav -> next;
                if(next_block == NULL || next_block -> curr_status == allocated){
                    trav -> curr_status = free;
                }
                else{
                    trav -> curr_status = free;
                    trav -> size += threshold + next_block -> size;
                    trav -> next = next_block -> next;
                }
            }
            else{
                meta_data* next_block = trav -> next;
                if(next_block == NULL || next_block -> curr_status == allocated){
                    prev -> next = next_block;
                    prev -> size += threshold + trav -> size;
                }
                else{
                    prev -> next = next_block -> next;
                    prev -> size += 2 * threshold + trav -> size + next_block -> size;
                }
            }
            printf("- * - succesfully freed up the memory - * -  ");
        }
        
    }
}

Node *createNode(int val, int noPtr)
{
    Node *newNode = (Node *)allocate(sizeof(Node));
    newNode->ptrCount = noPtr;
    newNode->inDegree = 0;
    newNode->data = val;
    newNode->marked = 0;
}

void initializeMatrix(int matrix[8][8])
{
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            matrix[i][j] = 0;
        }
    }
}

void print(int matrix[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("\n\n");
}

void adj_matrix(Node *root, int matrix[8][8])
{
    for (int i = 0; i < root->ptrCount; i++)
    {
        matrix[root->data][root->next[i]->data] = 1;
        adj_matrix(root->next[i], matrix);
    }
}

void addMatrix(int adjMatrix1[8][8], int adjMatrix2[8][8], int resMatrix[8][8])
{
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++)
            resMatrix[i][j] = adjMatrix1[i][j] + adjMatrix2[i][j];
    }
}

void RC(int adjMatrix1[8][8], int adjMatrix2[8][8], Node *heapArray[8])
{
    int zeros;
    int resMatrix[8][8];
    addMatrix(adjMatrix1, adjMatrix2, resMatrix);

    for (int col = 0; col < 8; col++)
    {
        if (heapArray[col]->marked == 0)
        {
            zeros = 0;
            for (int row = 0; row < 8; row++)
            {
                if (resMatrix[row][col] == 0)
                    zeros++;
            }

            if (zeros == 8)
            {
                deallocate((void *)heapArray[col]);
                printf("(Node-%d).\n", col);
            }
        }
    }
}

void mark(int adjMatrix1[8][8], int adjMatrix2[8][8], Node *heapArray[8])
{
    int zeros;
    int resMatrix[8][8];
    addMatrix(adjMatrix1, adjMatrix2, resMatrix);

    for (int col = 0; col < 8; col++)
    {
        if (heapArray[col]->marked == 0)
        {
            zeros = 0;
            for (int row = 0; row < 8; row++)
            {
                if (resMatrix[row][col] == 0)
                    zeros++;
            }

            if (zeros != 8)
            {
                heapArray[col]->marked = 1;
                printf("Marked Node-%d as not garbage.\n", col);
            }
        }
        else
        {
            heapArray[col]->marked = 1;
            printf("Marked Node-%d as not garbage.\n", col);
        }
    }
    printf("\n");
}

void sweep(Node *heapArray[8])
{
    for (int i = 0; i < 8; i++)
    {
        if (heapArray[i]->marked == 0)
        {
            deallocate((void *)heapArray[i]);
            printf("(Node-%d).\n", i);
        }

        if (i != 0 && i != 3)
            heapArray[i]->marked = 0;
    }
}

int main(){
    initialise_heap();
    Node *n1,*n2,*n3,*n5,*n7,*n8,*n9,*n10;
    //creating nodes
    n1 = createNode(0, 3);
    n2 = createNode(1, 0);
    n3 = createNode(2, 2);
    n5 = createNode(3, 1);
    n7 = createNode(4, 2);
    n8 = createNode(5, 1);
    n9 = createNode(6, 0);
    n10 = createNode(7, 0);
    //connecting the graph
    n1->next[0] = n2;
    n1->next[1] = n9;
    n1->next[2] = n10;
    n3->next[0] = n8;
    n3->next[1] = n10;
    n5->next[0] = n1;
    n7->next[0] = n1;
    n7->next[1] = n8;
    n8->next[0] = n9;
    //pointing from root
    Root root1, root2;
    root1.root = n5;
    n5->marked = 1;
    root2.root = n1;
    n1->marked = 1;
    //adjacency matrices
    int adj1[8][8];
    int adj2[8][8];
    initializeMatrix(adj1);
    initializeMatrix(adj2);

    adj_matrix(root1.root , adj1);
    print(adj1);
    adj_matrix(root2.root , adj2);
    print(adj2);
    Node *temp[8] = {n1,n2,n3,n5,n7,n8,n9,n10};
    //reference count
    RC(adj1, adj2, temp);
    adj_matrix(root1.root, adj1);
    print(adj1);
    adj_matrix(root2.root, adj2);
    print(adj2);
    //marksweep
    mark(adj1, adj2, temp);
    sweep(temp);
    print(adj1);
    print(adj2);

    display();
    return 0;
}
