#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#define STACK_DUMP(stack, filep) stack_dump((stack), (filep), #stack, __LINE__, __FILE__, __func__)

typedef int elem_t;
typedef unsigned long long elem_cannary;

const int STACK_SIZE = 5;
const int MULTIPLIER = 2;
const int CANNARY_VALUE = 858; //another number
const int POISON_VALUE = 0xBADEDA;
const int LEFT_CANNARY_PLACE = (sizeof(elem_cannary) - sizeof(elem_t)) / sizeof(elem_t);

enum ERRORS {CLEAR = 0,
             NULL_STRUCT_POINTER = -1,
             NULL_ARRAY_POINTER = -2,
             TOP_LESS_THEN_SIZE = -3,
             STACK_CHANGED = -4};

struct stack_t
{
    int stack_size = 0;
    int top = 0;
    elem_t * data = 0;
    unsigned long long stack_hash = 0;
};

void stack_dump(const stack_t * stk1, FILE* fp, const char * stack_name, int line, const char * file, const char * func); //const stack
int resize_stack (stack_t * stk1);

int stack_pop (stack_t * stk1, FILE * fp);
elem_t stack_push (stack_t * stk1, elem_t value, FILE * fp);

ERRORS stack_error (const stack_t * stk1);
int stack_error_analyse(int error_key);

int stack_dtor(struct stack_t * stk1);
int stack_ctor(stack_t * stk1);

unsigned long long hash_function (stack_t *stk1);     //check constants value in other function
elem_t check_hash (stack_t *stk1);

elem_cannary check_cannary (elem_cannary cannary_left, elem_cannary cannary_right);
void cannary_init(stack_t *stk1, elem_cannary * cannary_left, elem_cannary * cannary_right);

int main()
{
    struct stack_t stk_main;
    struct stack_t * stk1 = &stk_main;

    FILE * fp = fopen("stack_logs2.txt", "w");
    //is file open? create function for analyse calloc, realloc

    int value1 = 2;
    int value2 = 4;

    stack_ctor(stk1);

    for(int i = 0; i < 4; i++)
    {

        stk1->stack_hash = hash_function(stk1);

        stack_push(stk1, value1, fp);

        stack_push(stk1, value2, fp);

        stk1->data[0] = 234;

        //stack_pop(stk1, fp);

        value1 += 1;
        value2 += 1;
    }

    stack_dtor(stk1);

    fclose(fp);

    return 0;
}

int stack_ctor(stack_t * stk1)      //second call ctor and dtor?
{
    int cannary_memory = sizeof(elem_cannary)*2 / sizeof(elem_t);

    stk1->data = (elem_t*)calloc(STACK_SIZE + cannary_memory, sizeof(elem_t));

    int key_error = stack_error(stk1);
    stack_error_analyse(key_error);

    stk1->stack_size = STACK_SIZE;

    int cannary_left = (sizeof(elem_t) - sizeof(elem_cannary)) + 2;

    int cannary_right = stk1->stack_size+1;

    cannary_init(stk1, (elem_cannary*)stk1->data[cannary_left], (elem_cannary*)stk1->data[cannary_right]);

    if(stk1->stack_size > 0)
        return 1;

    else
        return 0;
}

int stack_dtor(struct stack_t * stk1)
{
    if(stk1->data == NULL)
    {
        return 0;
    }

    for(int i = 0; i < stk1->stack_size; i++)
    {
        stk1->data[i] = POISON_VALUE;
    }

    free(stk1->data);               //clean stack, free(), malloc(), calloc()
    stk1->data = NULL;

    return 1;

}

ERRORS stack_error (const stack_t * stk1)
{
    if (stk1 == NULL)
    {
        return NULL_STRUCT_POINTER;
    }

    if(stk1->data == NULL)
    {
        return NULL_ARRAY_POINTER;
    }

    if(stk1->top < 0)
    {
        return TOP_LESS_THEN_SIZE;          //add check for top < size_stack
    }
    return CLEAR;
}

int stack_error_analyse(int error_key)
{

    if(error_key < 1)
    {
        if(error_key == NULL_STRUCT_POINTER)
        {
            fprintf(stderr, "structure pointer is null");
            exit(1);
        }

        if(error_key == NULL_ARRAY_POINTER)  //support more than 1 error
        {
            fprintf(stderr, "array pointer is null");
            exit(1);
        }

        if(error_key == TOP_LESS_THEN_SIZE)
        {
            fprintf(stderr, "stack's memory is full: stk->top > stk->size");
            exit(1);
        }
    }
    return 1;
}

elem_t stack_push (stack_t * stk1, elem_t value, FILE * fp)
{
    int error_key = stack_error(stk1);
    stack_error_analyse(error_key);

    int cannary_right = stk1->stack_size + 1;

    check_hash(stk1);
    check_cannary(stk1->data[-2], stk1->data[cannary_right]);           //fix it
    //printf("stack size: %d \n", stk1->stack_size);

    int cannary_left = (sizeof(elem_t) - sizeof(elem_cannary)) + 2;

    stk1->data[stk1->top] = value;
    stk1->top++;
    if(stk1->stack_size - 2 <= stk1->top)
    {
        printf("RESIZE REALIZED \n");
        resize_stack(stk1);
    }

    stk1->stack_hash = hash_function(stk1);
    //printf("stk hash = %u \n", stk1->stack_hash);

    STACK_DUMP(stk1, fp);
    printf("\n \nSTK1->DATA[%d] = %d \n"
    "STK1->DATA[%d] ADDRESS = %p \n", stk1->top-1, stk1->data[stk1->top-1], stk1->top-1, &stk1->data[stk1->top-1]);

    return stk1->data[stk1->top];
}

int stack_pop (stack_t * stk1, FILE * fp)
{
    int error_key = stack_error(stk1);
    stack_error_analyse(error_key);
    check_hash(stk1);

    stk1->data[stk1->top-1] = 0;
    if(stk1->top * 2 < stk1->stack_size)
    {
        printf("RESIZE REALIZED \n");
        resize_stack(stk1);
    }

    //printf("stack size: %d \n", stk1->stack_size);

    //check_cannary(stk1->data[0], stk1->data[stk1->stack_size-1]);  //check cannary up before pop\push

    stk1->stack_hash = hash_function(stk1);
    //printf("stk hash = %u \n", stk1->stack_hash);

    STACK_DUMP(stk1, fp);

    printf("STK1->DATA[%d] = %d \n", stk1->top-1, stk1->data[stk1->top-1]);

    return stk1->top--;
}

int resize_stack (stack_t * stk1)
{
    int error_key = stack_error(stk1);
    stack_error_analyse(error_key);

    if(stk1->stack_size - 2 <= stk1->top)
    {
        stk1->stack_size *= MULTIPLIER;

        stk1->data = (elem_t*)realloc(stk1->data, stk1->stack_size * sizeof(elem_t));  //maybe change reallocated memory for data

        if(stk1->data == NULL)
        {
            fprintf(stderr, "stk1->data = NULL");
            exit(1);
        }

        memset(stk1->data + stk1->top, 0, (stk1->stack_size - stk1->top - 1) * sizeof(elem_t));

        int cannary_left = (sizeof(elem_t) - sizeof(elem_cannary)) + 2;
        int cannary_right = (stk1->stack_size + 1);
        printf("cannary left = %d", cannary_left);
        printf("cannary right = %d", cannary_right);
        cannary_init(stk1, (elem_cannary*)stk1->data[cannary_left], (elem_cannary*)stk1->data[cannary_right]);
    }

    else if(stk1->top * 2 < stk1->stack_size)
    {
        //int old_stack_size = stk1->stack_size;

        stk1->stack_size /= MULTIPLIER;

        stk1->data = (elem_t*)realloc(stk1->data, (stk1->stack_size+2) * sizeof(elem_t));

        if(stk1->data == NULL)
        {                                                                          //incorrect cannary
            fprintf(stderr, "stk1->data = NULL");
            exit(1);
        }

        memset(stk1->data + (stk1->top+1), POISON_VALUE, stk1->stack_size);
        cannary_init(stk1, (elem_cannary*)stk1->data[-2], (elem_cannary*)stk1->data[stk1->stack_size + 1]);
    }

    return 1;
}

void stack_dump(const stack_t * stk1, FILE * fp, const char * stack_name, int line, const char * file, const char * func)
{
    fprintf(fp, " \n \nStack [%p] name: %s \n"
    "Function called on line %d in %s() \n "
    "In file %s \n", &stk1, stack_name, line, func, file);

    for (int i = 0; i < stk1->stack_size; i++)
    {
        fprintf(fp, "\n stk->data[%d]: %d \n", i, stk1->data[i]);
    }

    /*printf("fprintf value: %d  ", a);
    printf("stk->data[0] = %d", stk->data[0]);*/

    int error_key = stack_error(stk1);
    stack_error_analyse(error_key);
}

unsigned long long hash_function (stack_t *stk1)
{
    unsigned long long result = 0;

    for(int i = 0; i < stk1->stack_size; i++)
    {
        result += (((stk1->data[i])*37 + 4)*3);
    }

    //printf("\n hash_function result = %u \n", result);

    return result;

}

elem_t check_hash (stack_t *stk1)
{
    unsigned long long calculated_hash = hash_function(stk1);

    /*printf("     \n stk1-stack_hash = %d \n", stk1->stack_hash);
    printf("     \n IF old hash = %d \n", calculated_hash);*/

    if(calculated_hash != stk1->stack_hash)
    {
        fprintf(stderr, "\n STACK CHANGED \n");
        exit(1);
    }

    //printf("calculated hash = %u   stk1->stack_hash = %u \n", calculated_hash, stk1->stack_hash);
    return 1;
}

elem_cannary check_cannary (elem_cannary cannary_left, elem_cannary cannary_right)
{
    if(cannary_left != CANNARY_VALUE && cannary_right != CANNARY_VALUE)
    {
        fprintf(stderr, "\n CANNARY GUARD WENT OFF \n");
        exit(1);
    }
    printf("cannary left = %u \n", cannary_left);
    printf("cannary right = %u \n", cannary_right);

    return 1;
}

void cannary_init(stack_t *stk1, elem_cannary * cannary_left, elem_cannary * cannary_right)
{
    int elem_t_size = sizeof(elem_t);
    int elem_cannary_size = sizeof(elem_cannary);

    int cannary_left_element = (elem_t_size - elem_cannary_size) + 2;

    int cannary_right_element = stk1->stack_size + 1;   //check stack_size

    cannary_left = (elem_cannary*)&stk1->data[-2];

    cannary_right = (elem_cannary*)&stk1->data[cannary_right_element];

    printf("\n \ncannary size: %d \n\n", sizeof(*cannary_left));

    *cannary_left = CANNARY_VALUE;
    *cannary_right = CANNARY_VALUE;

    printf("\n \nDATA left address: %p\n left value: %u \n", &stk1->data[-2], stk1->data[0]);

    printf("DATA right address: %p\n right value: %u \n \n", &stk1->data[stk1->stack_size], stk1->data[stk1->stack_size]);

    printf("\n \nCANNARY left address: %p\n left value: %u \n", cannary_left, *cannary_left);
    printf("CANNARY right address: %p\n right value: %u \n \n", cannary_right, *cannary_right);
}

