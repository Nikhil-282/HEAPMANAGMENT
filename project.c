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
            printf("enter valid address to deallocate\n\n");
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
            printf("- * - succesfully freed up the memory - * -\n\n");
        }
        
    }
}

int main(){
    initialise_heap();
    int* p = (int*)allocate(sizeof(int)*10);
    float* q = (float*)allocate(sizeof(float) * 100);
    char* r = (char*)allocate(sizeof(char) * 200);
    char* s = (char*)allocate(sizeof(char) * 0);
    deallocate(p);
    deallocate(q);
    deallocate(r);
    int* t = (int*)allocate(sizeof(int)*50);
    int* z = (int*)allocate(sizeof(int)*10);
    display();

    return 0;
}
