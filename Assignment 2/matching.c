// Clifford Rice
// COP3502C
// February 3, 2013
// Program 2: Recursive dating match program

// Import necessary libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Define constants
#define NAME_SIZE 19

// Define two structs to keep track of program data
typedef struct{
    char* name;
    int* likes;
    int original;
} Person;

typedef struct{
    int matchPts;
    int differences;
    int* matches;
} Match;

// Prototype methods to be used in this program.
void fillLists(FILE* ifp, Person* women, Person* men, int k);
Match* recursivePermute(Match* best, Person* women, Person* men, int length, int k);
void exchange(Person* women, int i, int j);
void populateMatches(Match* m, Person* women, int length);
void sumsAndDifferences(Match* m, Person* women, Person* men, int length);
int worseOf(int a, int b);
int diff(int a, int b);
Match* compareMatches(Match* m1, Match* m2);
void printMatch(Match* best, Person* women, Person* men, int length, int num);
void freeList(Person* list, int length);

int main(){

    // Declare a counter variable and a limiting variable
    int reps, i;

    // Open the input file for reading
    FILE* ifp = fopen("matching.txt","r");

    // Obtain a limit from the file
    fscanf(ifp,"%d",&reps);

    // Loop for "reps" times
    for(i = 0; i < reps; i++){
        // Declare a variable for the number of pairs in the group and get the number from the file
        int people;
        fscanf(ifp,"%d",&people);

        // Allocate memory for two arrays of structs to keep track of themen and women respectively
        Person* men = malloc(sizeof(Person) * people);
        Person* women = malloc(sizeof(Person) * people);

        // Fill the lists of men and women with names/rankings/etc.
        fillLists(ifp, women, men, people);

        // Allocate memory for an empty match, set its match points to zero.
        Match* best = malloc(sizeof(Match));
        best->matchPts = 0;
        best->matches = malloc(sizeof(int)*people);

        // Set this empty match equal to the result of the recursive call
        best = recursivePermute(best, women, men, people, 0);

        // Print the best match out for the user
        printMatch(best, women, men, people, i);

        // Free the memory allocated for this loop
        freeList(women, people);
        freeList(men, people);
        free(best->matches);
        free(best);
    }

    return 0;
}

void fillLists(FILE* ifp, Person* women, Person* men, int k){

    int i,j;

    // Fill array structs with names and allocate memory for dynamic attributes
    for(j = 0; j < 2 * k; j++){
        // Fill the men array first
        if(j<k){
            men[j].name = malloc(sizeof(char) * NAME_SIZE);
            fscanf(ifp, "%s", men[j].name);
            men[j].original = j;
            men[j].likes = malloc(sizeof(int) * k);
        }
        // Then fill the women array
        else{
            women[j-k].name = malloc(sizeof(char) * NAME_SIZE);
            fscanf(ifp, "%s", women[j-k].name);
            women[j-k].original = j - k;
            women[j-k].likes = malloc(sizeof(int) * k);
        }
    }

        // Fill structs with likings
    for(j = 0; j < 2 * k; j++){
        // Men first again
        if (j < k){
            for(i = 0; i < k; i++){
                fscanf(ifp,"%d", &(men[j].likes[i]));
            }
        }
        // Then women.
        else{
            for(i = 0; i < k; i++){
                fscanf(ifp,"%d", &(women[j-k].likes[i]));
            }
        }
    }
}

// Recursively loop through all possible permutations and return the best
Match* recursivePermute(Match* best, Person* women, Person* men, int length, int k) {

     int j;

     // Base-case: This is one of the unique permutations of the women array. Calculate the points accordingly
     if (k == length){
         // Allocate space for a new match and it's array
        Match* check = malloc(sizeof(Match));
        check->matches = malloc(sizeof(int)*length);

        // Fill the matches array with the original indices of the women
        populateMatches(check, women, length);

        // Calculate the relationship sums and differences
        sumsAndDifferences(check, women, men, length);

        // return the match
        return check;

     } else {

         // Loop through each possible starting woman for index k,
         // the first index for which we have a choice.
         for (j=k; j<length; j++) {

            // Place the woman stored in index j in location k.
            exchange(women, k, j);

            // Change "best" to the returned value if it is a better match
            best = compareMatches(best, recursivePermute(best, women, men, length, k+1));

             // Put the original woman that used to be there back
             exchange(women, j, k);
         }

        // return the best match
        return best;
     }
}

// Moves a woman from location i to location j
void exchange(Person* women, int i, int j) {
    Person temp = women[i];
    women[i] = women[j];
    women[j] = temp;
}

// Fills the matches array of a Match linearly with the original indices of the jumbled women
void populateMatches(Match* m, Person* women, int length){
    int i;

    for(i = 0; i < length; i++){
        m->matches[i] = women[i].original;
    }
}

// Fills the matchPts and differences of a match
void sumsAndDifferences(Match* m, Person* women, Person* men, int length){
    int i;

    m->matchPts = 0;
    m->differences = 0;

    for(i = 0; i < length; i++){
        // adds the lower like-ratio to the matchPts
        m->matchPts += worseOf(men[i].likes[m->matches[i]], women[i].likes[i]);
        // calculates and sums the difference in like-ratios
        m->differences += diff(men[i].likes[m->matches[i]], women[i].likes[i]);
    }
}

// Returns the lower of two integers
int worseOf(int a, int b){
    if (a > b)
        return b;
    else
        return a;
}

// Returns the positive difference of two numbers
int diff(int a, int b){
    if (a >= b)
        return a - b;
    else
        return b - a;
}

// Compares two matches and returns the better one
Match* compareMatches(Match* m1, Match* m2){
    // First checks based on match points
    if(m1->matchPts > m2->matchPts){
        return m1;
    }else if (m1->matchPts < m2->matchPts){
        return m2;
    }else{
        // If the match points are equal, picks the lower difference
        if(m1->differences <= m2->differences)
            return m1;
        else
            return m2;
    }
}

// Prints a match out
void printMatch(Match* best, Person* women, Person* men, int length, int num){
    int i;
    printf("Matching #%d: Maximum Score = %d.\n\n", num + 1, best->matchPts);
    for(i = 0; i<length; i++){
        printf("%s %s\n", men[i].name, women[best->matches[i]].name);
    }
    printf("\n\n");
}

// Frees memory from a complex array of People objects and the array itself
void freeList(Person* people,int length){
    int i;
    for(i=0; i <length;i++){
        free(people[i].likes);
        free(people[i].name);
    }
    free(people);
}
