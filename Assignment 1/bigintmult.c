// Clifford Rice
// COP 3502C, Tappen
// January 14, 2013
// Program designed to implement multiplication of extremely long integers.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LENGTH 10001
#define ZERO_VALUE 48


// Struct outline for the large integers to be dealt with.
struct integer{
	int* digits;
	int size;
};

//Preconditions: the first parameter is string that only
//               contains digits and is 10000 digits or
//               fewer. No leading 0s will be included
//               unless the number represented is 0.
//Postconditions: The function will read the digits of the
//	large integer character by character,
//	convert them into integers and return a
//             pointer to the appropriate struct integer.
struct integer* convert_integer(char* stringInt);

//Preconditions: p is a pointer to a big integer.
//Postconditions: The big integer pointed to by p is
//                printed out.
void print(struct integer *p);

//Preconditions: p and q are pointers to struct integers.
//Postconditions: A new struct integer is created that
//                stores the product of the integers
//                pointed to by p and q and a pointer to it
//                is returned.
struct integer* multiply(struct integer *p,
                         struct integer *q);

void noZeros(struct integer *p);

int main(){

    FILE *ifp = fopen("bigint.txt", "r");
    int reps, i;
    fscanf(ifp, "%d", &reps);

    for(i=1; i <= reps; i++){
        char* str1 = malloc(LENGTH * sizeof(char));
        char* str2 = malloc(LENGTH * sizeof(char));

        fscanf(ifp, "%s %s", str1, str2);

        struct integer* int1 = convert_integer(str1);
        struct integer* int2 = convert_integer(str2);

        free(str1);
        free(str2);

        struct integer* product = multiply(int1,int2);

        printf("Problem #%d: ", i);
        print(int1);
        printf(" * ");
        print(int2);
        printf(" = ");
        print(product);
        printf("\n");

        free(int1->digits);
        free(int2->digits);
        free(product->digits);
        free(int1);
        free(int2);
        free(product);
    }

    return 0;

}


struct integer* convert_integer(char* stringInt){
    // Create an empty big integer struct
    struct integer *bigInt = malloc(sizeof(struct integer));

    // Set the size of the struct equal to the length of the string
    bigInt->size = strlen(stringInt);

    bigInt->digits = malloc(sizeof(int) * bigInt->size);

    // Create loop index variables
    int i;

    // Loop through the string and the array of ints in reverse directions to create the reverse effect
    for(i = bigInt->size - 1; i >= 0; i--){
        bigInt->digits[i] = stringInt[bigInt->size - i - 1] - ZERO_VALUE;
    }


    // Return the pointer
    return bigInt;
}

void print(struct integer *p){
    // Index variable
    int i;

    // Begin at the last number in 'p' and print consecutive numbers down to the last.
    for(i = p->size - 1; i >= 0; i--){
        printf("%d", p->digits[i]);
    }
}

struct integer* multiply(struct integer *p, struct integer *q){

    // Allocate enought memory for a new integer struct
    struct integer* newInt = malloc(sizeof(struct integer));

    // Acknowledge a max length of the new integer
    int maxLen = p->size + q->size;

    // Allocate memory forhe integer's digits
    newInt->digits = malloc(sizeof(int) * maxLen);
    newInt->size = 0;

    // Set up size monitors
    struct integer* bigger;
    struct integer* smaller;

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

void noZeros(struct integer* p){
    // Verify that there a no leading zeros
    while(1){
        if((p->digits[p->size - 1] == 0) && (p->size > 1)){
            p->size--;
        } else {
            break;
        }
    }
}
