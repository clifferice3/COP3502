// Clifford Rice
// COP 3502
// March 17, 2013
// Takes a file of students and sorts them by birthday, then accepts
// and replies to queries as to which students has the closest birthday
// to another.

// Import necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants
#define MONTHS {"JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER"}
#define MIY 12
#define DAYS_IN_YEAR 365
#define DAYS_IN_MONTHS {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
#define SIZE 15


// Define stucts to keep track of data
typedef struct{
    int month;
    int day;
    int year;
    int total;
} Date;

typedef struct{
    char first[SIZE];
    char last[SIZE];
} Name;

typedef struct{
    Name *myName;
    Date *birthday;
} Student;


// Method prototypes
void loadClass(FILE* ifp, Student* students[], int numStudents);
Student* makeStudent(char* first, char* last, int month, int day, int year);
int convertMonth(char* month);
void dateTotal(Date* d);
void mergeSort(Student* students[], Student* temp[], int left, int right);
void merge(Student* students[], Student* temp[], int left, int mid, int right);
int compareStudents(Student* a, Student* b);
int compareDates(Date* a, Date* b);
int dateDistance(Date* a, Date* b);
int compareNames(Name* a, Name* b);
void handleQueries(FILE* ifp, Student* students[], int length, int qs);
Name* closestBirthday(int index, Student* students[], int length);
int findIndex(Name* request, Student* students[], int length);
void printResult(Name* a, Name* b);
void freeClass(Student* students[], int length);


int main(){
    // Create input file pointer
    FILE * ifp = fopen("birthday.txt", "r");

    // Make a variable to store number of classes and a loop index variable
    int numClasses, i;
    fscanf(ifp, "%d", &numClasses);

    // Main loop for handling all cases
    for(i = 0; i < numClasses; i++){
        // Print out which class this is
        printf("Class #%d:\n\n", i+1);

        // Figure out how many students are in this class from file
        int numStudents;
        fscanf(ifp, "%d", &numStudents);

        // Make an array of pointers to students and another to help sort it
        Student* students[numStudents];
        Student* temp[numStudents];

        // Load in all student information for this class from the file
        loadClass(ifp, students, numStudents);

        // Sort the students in the class
        mergeSort(students, temp, 0, numStudents - 1);

        // Get the number of queries for this class
        int numQueries;
        fscanf(ifp, "%d", &numQueries);

        // Handle the inputted number of queries
        handleQueries(ifp,students,numStudents,numQueries);

        // Free the class
        freeClass(students, numStudents);

        // Add a line
        printf("\n");
    }

    // Close the file pointer
    close(ifp);

    // Return 0 as mandated
    return 0;
}

void loadClass(FILE* ifp, Student* students[], int numStudents){
    // Loop index
    int i;

    // Loop through to get data for each student
    for(i=0; i < numStudents; i++){

        // Initialize variables to read file input
        char first[SIZE];
        char last[SIZE];
        char month[SIZE];
        int day, year;

        // Store one line of file data in variables
        fscanf(ifp,"%s", first);
        fscanf(ifp,"%s ", last);
        fscanf(ifp,"%s", month);
        fscanf(ifp, "%d", &day);
        fscanf(ifp, "%d", &year);

        // Convert the month to a numeric index form (0-11)
        int mon = convertMonth(month);

        // Set this element of the class equal to a student with these values
        students[i] = makeStudent(first,last,mon,day,year);
    }
}

Student* makeStudent(char* first, char* last, int month, int day, int year){
    // Allocate memory for the student and its pointer attributes
    Student* s = malloc(sizeof(Student));
    s->myName = malloc(sizeof(Name));
    s->birthday = malloc(sizeof(Date));

    // Fill the name of the student
    strcpy(s->myName->first, first);
    strcpy(s->myName->last, last);

    // Fill the birthdate of the student
    s->birthday->month = month;
    s->birthday->day = day;
    s->birthday->year = year;

    // Set the 'total' value of the date
    dateTotal(s->birthday);

    return s;
}

int convertMonth(char* month){

    // Initialize the constant array "MONTHS"
    char* monthNames[MIY] = MONTHS;

    int i;

    for (i=0; i < MIY; i++){

        // Find which month this one is and return its index
        if(strcmp(monthNames[i], month)==0){
            return i;
        }
    }

    return -1;
}

void dateTotal(Date *d){

    // Set to the constant array holding the number of days in each month
    int month[MIY] = DAYS_IN_MONTHS;

    // Set the initial value of total to the days past of the current month
    d->total = d->day;

    int i;

    // Add all prior months' total days
    for(i=d->month; i > 0; i--){
        d->total += month[i-1];
    }

    // The total attribute will be set to x number of days past the start of the year.
}

int compareStudents(Student* a, Student* b){

    // Perform a date comparison
    int dcmp = compareDates(a->birthday, b->birthday);

    if(dcmp != 0){
        // If the dates are not equal, return the result
        return dcmp;
    } else {
        // Otherwise return the result of a name comparison
        return compareNames(a->myName, b->myName);
    }
}

int dateDistance(Date* a, Date* b){
    // Set the difference  equal to the absolute value of the difference of the two dates
    int diff = abs(a->total - b->total);

    // If this number is more than 1/2 of a year's length, reverse its direction
    if (diff > DAYS_IN_YEAR / 2){
        diff = DAYS_IN_YEAR - diff;
    }

    return diff;
}

int compareDates(Date* a, Date* b){
    // - if a < b || + if a > b || 0 if a == b
    return a->total - b->total;
}

int compareNames(Name* a, Name* b){

    // If the last names arent the same, return their comparison.
    // Otherwise return the value of the first name comparison.
    if(strcmp(a->last, b->last) != 0){
        return strcmp(a->last, b->last);
    } else {
        return strcmp(a->first, b->first);
    }
}

void mergeSort(Student* students[], Student* temp[], int left, int right){
    int mid;

    if (right > left){

        // Cut the list in half
        mid = (right + left) / 2;

        // Call this function on the right and left sides
        mergeSort(students, temp, left, mid);
        mergeSort(students, temp, mid+1, right);

        // "Merge" the two sorted sides
        merge(students, temp, left, mid+1, right);
    }
}

void merge(Student* students[], Student* temp[], int left, int mid, int right){
    int i, left_end, num_elements, tmp_pos;

    left_end = mid - 1;
    tmp_pos = left;
    num_elements = right - left + 1;

    // Do this until one of the lists is empty
    while ((left <= left_end) && (mid <= right)){
        // Put the lesser of the two objects in the temporary array first
        if (compareStudents(students[left], students[mid]) <= 0){
            temp[tmp_pos] = students[left];
            tmp_pos++;
            left++;
        } else{
            temp[tmp_pos] = students[mid];
            tmp_pos++;
            mid++;
        }
    }

    // If the right side runs out first, fill open spots with the rest of the left
    while (left <= left_end){
        temp[tmp_pos] = students[left];
        left++;
        tmp_pos++;
    }

    // If the left side runs out first, fill open spots with the rest of the right
    while (mid <= right){
        temp[tmp_pos] = students[mid];
        mid++;
        tmp_pos++;
    }

    // Move the sorted temp array back into the original array
    for (i=0; i <= num_elements; i++){
        students[right] = temp[right];
        right--;
    }
}

void handleQueries(FILE* ifp, Student* students[], int length, int qs){
    int i;

    Name* result;

    for(i = 0; i < qs; i++){

        // Store the name from the file
        Name* request = malloc(sizeof(Name));
        fscanf(ifp, "%s ", request->first);
        fscanf(ifp, "%s", request->last);

        // Find the index of this name
        int index = findIndex(request, students, length);

        // Find whether the person before or after this person has a closer birthday
        result = closestBirthday(index, students, length);

        // Print out the result
        printResult(request, result);

        // Empty the Name struct
        free(request);
    }
}

Name* closestBirthday(int index, Student* students[], int length){

    // Set the current student, initialize the forward/backward pointers
    Student* current = students[index];
    Student* forward;
    Student* backward;

    // Set the forward pointer allowing for wrap-around
    if(index == length - 1){
        forward = students[0];
    } else {
        forward = students[index + 1];
    }

    // Set the backward pointer allowing for wrap around
    if(index == 0){
        backward = students[length - 1];
    } else{
        backward = students[index - 1];
    }

    // Find the distance between current student and those in front of/behind him
    int backwardDistance = dateDistance(current->birthday, backward->birthday);
    int forwardDistance = dateDistance(current->birthday, forward->birthday);

    // Return the smaller of these
    if (forwardDistance <= backwardDistance){
        return forward->myName;
    } else {
        return backward->myName;
    }

}

int findIndex(Name* request, Student* students[], int length){
    int index;

    // Loop through the class and find the student with the exact requested name
    for(index=0; index<length; index++){
        if(compareNames(request, students[index]->myName)==0){
            return index;
        }
    }
}

void printResult(Name* a, Name* b) {
    // Print out the result in the specified format
    printf("%s %s has the closest birthday to %s %s.\n", b->first, b->last, a->first, a->last);
}

void freeClass(Student* students[], int length){
    int i;

    // Free all attributes of each student in the class
    for(i=0;i<length;i++){
        free(students[i]->birthday);
        free(students[i]->myName);
        free(students[i]);
    }
}
