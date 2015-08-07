// Clifford Rice
// COP 3502C, Tappen
// March 1, 2013
// Program designed to implement multiplication of extremely long integers.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LENGTH 10001
#define ZERO_VALUE 48


// Struct from the original program
typedef struct{
	int* digits;
	int size;
} arrayInt;

// New link list struct
typedef struct integer{
   int digit;
   struct integer *next;
} BigInt;

// Required methods
BigInt* makeBigInt(char* stringInt);
void printBigInt(BigInt* p);
BigInt* multiply(BigInt* p, BigInt* q);

void empty(BigInt* p);
BigInt* changeBack(arrayInt* p);

arrayInt* convert_integer(BigInt* head);
arrayInt* old_multiply(arrayInt *p, arrayInt *q);
void noZeros(arrayInt *p);

int main(){

    FILE *ifp = fopen("bigint.txt", "r");
    int reps, i;
    fscanf(ifp, "%d", &reps);

    for(i=1; i <= reps; i++){

        // Allocate memory for strings
        char* str1 = malloc(LENGTH * sizeof(char));
        char* str2 = malloc(LENGTH * sizeof(char));

        // Load in strings from file
        fscanf(ifp, "%s %s", str1, str2);

        // Make the two linked lists from the strings
        BigInt* int1 = makeBigInt(str1);
        BigInt* int2 = makeBigInt(str2);

        // Free memory from the strings
        free(str1);
        free(str2);

        // Find and store the product of the two integers
        BigInt* product = multiply(int1,int2);

        // Print results to output
        printf("Problem #%d: ", i);
        printBigInt(int1);
        printf(" * ");
        printBigInt(int2);
        printf(" = ");
        printBigInt(product);
        printf("\n");

        // Clear memory from the structs
        empty(int1);
        empty(int2);
        empty(product);
    }

    return 0;

}

BigInt* makeBigInt(char* stringInt){
    int counter;

    BigInt* p = malloc(sizeof(BigInt));
    BigInt* help = p;

    for(counter = strlen(stringInt) - 1; counter > 0; counter--){
        help->digit = stringInt[counter] - ZERO_VALUE;
        help->next = malloc(sizeof(BigInt));
        help = help->next;
    }

    help->digit = stringInt[0] - ZERO_VALUE;
    help->next = NULL;

    return p;
}

BigInt* multiply(BigInt* p, BigInt* q){
    arrayInt* int1 = convert_integer(p);
    arrayInt* int2 = convert_integer(q);

    arrayInt* product = old_multiply(int1,int2);

    free(int1->digits);
    free(int2->digits);
    free(int1);
    free(int2);

    BigInt* answer = changeBack(product);

    free(product->digits);
    free(product);

    return answer;
}

void empty(BigInt* p){
    if(p->next == NULL){
        free(p);
    } else{
        empty(p->next);
        free(p);
    }
}

BigInt* changeBack(arrayInt* p){
    int i;

    BigInt* q = malloc(sizeof(BigInt));
    BigInt* help = q;

    for(i = 0; i < p->size - 1; i++){
        help->digit = p->digits[i];
        help->next = malloc(sizeof(BigInt));
        help = help->next;
    }

    help->digit = p->digits[p->size - 1];
    help->next = NULL;

    return q;
}

arrayInt* convert_integer(BigInt* head){

    // Create an empty big integer struct
    arrayInt* p = malloc(sizeof(arrayInt));

    // Allocate enough memory
    p->digits = malloc(sizeof(int) * LENGTH);
    p->size = 0;

    BigInt* help = head;

    while(help != NULL){
        p->digits[p->size] = help->digit;
        help = help->next;
        p->size += 1;
    }

    // Return the pointer
    return p;
}

void printBigInt(BigInt *p){
    if(p->next == NULL){
        printf("%d",p->digit);
    } else {
        printBigInt(p->next);
        printf("%d",p->digit);
    }
}

arrayInt* old_multiply(arrayInt *p, arrayInt *q){

    // Allocate enought memory for a new integer struct
    arrayInt* newInt = malloc(sizeof(arrayInt));

    // Acknowledge a max length of the new integer
    int maxLen = p->size + q->size;

    // Allocate memory forhe integer's digits
    newInt->digits = malloc(sizeof(int) * maxLen);
    newInt->size = 0;

    // Set up size monitors
    arrayInt* bigger;
    arrayInt* smaller;

    // Distinguish between the larger and smaller for ease of multiplication. Default is p.
    if(q->size > p->size){
        bigger = q;
        smaller = p;
    } else {
        bigger = p;
        smaller = q;
    }

    // Declare index variables
    int i, j;

    // Declare a "carry" variable to deal with remainders
    int carry = 0;

    // Loop that performs generic multiplication
    for(i = 0; i < bigger->size; i++){

        int temp = 0;

        // Multiplies one digit of the first number by all applicable digits of the second and sums the products
        for(j = 0; j <= i; j++){
            // Breaks loop if the "bottom" number is out of digits
            if(j >= smaller->size)
                break;

            temp += smaller->digits[j] * bigger->digits[i - j];
        }


        // Remainder is added to the total sum of the products
        temp += carry;

        // A new remainder is calculated
        carry = temp / 10;

        // The appropriate digits is stored in
        newInt->digits[i] = temp % 10;
        newInt->size++;
    }

    int lower = 1;

    for(i = bigger->size; i < maxLen; i++){

        int temp = 0;

        int index = i - bigger->size + 1;

        for(j = bigger->size - 1; j >= lower; j--){
            if(index >= smaller->size)
                break;
            temp += smaller->digits[index] * bigger->digits[j];
            index++;
        }

        // Remainder is added to the total sum of the products
        temp += carry;

        // A new remainder is calculated
        carry = temp / 10;

        // The appropriate digits is stored in the array
        newInt->digits[i] = temp % 10;
        newInt->size++;



        if(i - bigger->size > smaller->size)
            break;
    }

    // Sets the final digit and the size of the new number if needed
    while (carry > 0){
        newInt->digits[i] = carry % 10;
        carry /= 10;
        newInt->size++;
    }

    noZeros(newInt);

    // Returns new integer
    return newInt;
}

void noZeros(arrayInt* p){

    // Verify that there a no leading zeros
    while(1){
        if((p->digits[p->size - 1] == 0) && (p->size > 1)){
            p->size--;
        } else {
            break;
        }
    }
}
