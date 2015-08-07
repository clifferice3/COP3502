// Clifford Rice
// COP 3502C
// April 20, 2013
// Program that creates/adds/checks user info into a database using hash tables and
// Crytopgrahic hashing.

// Import needed libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Define Constants
#define MAX_USERNAME_LEN 15
#define MAX_PASSWORD_LEN 20
#define MIN_PASSWORD_LEN 8
#define MAX_ENTRIES 10000

// Necessary struct implementations
typedef struct {
    char username[MAX_USERNAME_LEN + 1];
    unsigned long password;
} DBEntry;

typedef struct {
    DBEntry db[MAX_ENTRIES];
    int size;
} DB;

// Function prototypes
int printMenu(FILE* ifp, int isSignedIn);
int insertUser(DBEntry* user, DB* database);
int login(FILE* ifp, DB* database);
void createUser(FILE* ifp, DB* database);
static unsigned long CryptoHash(unsigned char *str);
int validPass(unsigned char* pass);
int isSpecSym(char a);
int addUser(DBEntry user, DB* database);
int find(char name[], DB* database);
int nameHash(char name[]);

int main(){
    // Set up the file for input streaming
    FILE* ifp = fopen("input.txt", "r");

    // Declare the variables needed, get number of operations from file
    int numOps, i, signedIn = 0,numFail = 0;
    fscanf(ifp, "%d", &numOps);

    // Set up the database and initialize the starting names to the empty string
    DB* database = malloc(sizeof(DB));
    for(i=0;i<MAX_ENTRIES;i++){
        strcpy(database->db[i].username, "");
    }

    // Loop for the specified number of operations or until there are too many consecutive failed logins
    for(i=0; i<numOps; i++){
        // Get the choice from the menu
        int choice = printMenu(ifp,signedIn);

        // Perform the specified task
        if(choice == 1){
            // Try to log in, if successful, reset failed logins and mark the fact that we are signed in
            if(login(ifp, database)){
                signedIn = 1;
                numFail = 0;
            } else {
                // Increment number of failures, if too high end the program
                numFail++;
                if(numFail >= 3){
                    printf("Sorry, your username/password was incorrect. You have too many failed attempts. Please wait a while and come back again later.\n");
                    break;
                } else {
                    printf("Sorry, your username/password was incorrect. Please try again.\n\n");
                }
            }
        } else if(choice == 2){
            // Try to add the user, reset the failures
            createUser(ifp, database);
            numFail=0;
        } else if ((choice == 3)&&(signedIn)){
            // If we were logged in and we want to log out, say so and adjust the variable
            printf("Successfully logged out.\n\n");
            signedIn = 0;
        }
    }
    // close the file and free the database
    close(ifp);
    free(database);

    // process returns 0
    return 0;
}

int printMenu(FILE* ifp, int isSignedIn){
    // Print first two choices and the third if we are signed in at the time
    int choice;
    printf("1. Log in as an existing user\n");
    printf("2. Create a new account\n");
    if(isSignedIn)
        printf("3. Log out\n");
    printf("\n");

    // Get the choice from the file and return it
    fscanf(ifp,"%d",&choice);
    return choice;
}

int login(FILE* ifp, DB* database){
    char name[MAX_USERNAME_LEN+1];
    char pass[2 * MAX_PASSWORD_LEN];

    // Get the info from the file
    fscanf(ifp,"%s",name);
    fscanf(ifp,"%s",pass);

    // Print the process information
    printf("Log in as an existing user\n");
    printf("Username: %s\n", name);
    printf("Password: %s\n", pass);
    printf("\n");

    // Get the index of this name if it exists
    int found = find(name,database);

    // If it doesn't exist return false
    if (found == -1){
        return 0;
    }

    // If the password is vaild and the hashes match, return a successful login, otherwise return false
    if((validPass(pass)) && (database->db[found].password == CryptoHash((unsigned char *)(strcat(pass,"Qx9nUvW4"))))){
        printf("Successfully logged in. Welcome, %s\n\n", database->db[found].username);
        return 1;
    } else {
        return 0;
    }
}

void createUser(FILE* ifp, DB* database){
    char name[MAX_USERNAME_LEN+1];
    char pass[2 * MAX_PASSWORD_LEN];

    // Get the info from the file
    fscanf(ifp,"%s",name);
    fscanf(ifp,"%s",pass);

    // Print operation info
    printf("Create an account\n");
    printf("Username: %s\n", name);
    printf("Password: %s\n", pass);
    printf("\n");

    // Check the password, stop if it is invalid
    if(!(validPass(pass))){
        printf("Sorry, your password must be 8-20 characters in length and contain either a number or one of these special characters: !@#$%%*.\n\n");
        return;
    }

    // Store the user username
    DBEntry user;
    strcpy(user.username, name);

    // Salt the password
    strcat(pass,"Qx9nUvW4");

    // Hash and store the password
    user.password = CryptoHash((unsigned char*)pass);

    // store whether the user was successfully added
    int added = addUser(user,database);

    // Print if the account was created or not
    if(!(added)){
        printf("Sorry, that username is already taken. Please try again.\n\n");
        return;
    }

    printf("Account successfully created. Please log in.\n\n");
}

int validPass(unsigned char* pass){
    // Check the size restraints first
    if((strlen(pass)<MIN_PASSWORD_LEN)||(strlen(pass)>MAX_PASSWORD_LEN)){
        return 0;
    }

    // Ensure all characters are alphanumeric or special characters
    int numOrSpecChar = 0, upper = 0, i;
    for(i=0;i<strlen(pass);i++){
        if(isupper(pass[i]))
            upper = 1;
        else if (isalpha(pass[i]));
        else if((isdigit(pass[i]))||(isSpecSym(pass[i])))
            numOrSpecChar = 1;
        else
            return 0;
    }

    // Ensure there is at least one special char/number and uppercase letter
    if(numOrSpecChar && upper)
        return 1;
    else
        return 0;
}

int isSpecSym(char a){
    // Check the specified special characters
    if((a=='!')||(a=='@')||(a=='#')||(a=='$')||(a=='%')||(a=='*')||(a=='.'))
        return 1;
    else
        return 0;
}

int addUser(DBEntry user, DB* database){
    // Set the initial point based on the hash of the username
    int check = nameHash(user.username);
    int i;
    for(i = check;;i++){
        // Start at this spot and move forward until an empty spot is found
        if(strcmp(database->db[i%MAX_ENTRIES].username, "")==0){
            strcpy(database->db[i%MAX_ENTRIES].username, user.username);
            database->db[i%MAX_ENTRIES].password = user.password;
            return 1;
        } else if(strcmp(database->db[i%MAX_ENTRIES].username,user.username)==0){
            // If the same username is found, return that this is the case
            return 0;
        }
    }
}

int find(char name[], DB* database){
    // Set the initial point based on the hash of this name
    int check = nameHash(name);
    int i;
    for(i = check;;i++){
        // Move forward until either an empty point is found, the name is found, or a full cycle is complete
        if((database->db[i%MAX_ENTRIES].username == "")){
            return -1;
        } else if(strcmp(database->db[i%MAX_ENTRIES].username,name)==0){
            return i;
        }
        if (i%MAX_ENTRIES == check-1)
            return -1;
    }
}

int nameHash(char name[]){
    // Sum the ASCII values of the characters in the string
    int num = 0, i;
    for(i=0;i<strlen(name);i++){
        num += (int)name[i];
    }
    // Multiply by the specified value
    num *= 5737;
    // Return this modded by the max entries
    return num % MAX_ENTRIES;
}

static unsigned long CryptoHash(unsigned char *str){
    // Specified code from the assignment description to cryptoash the password
   unsigned long hashval = 0;
   int c;
   while (c = *str++)
      hashval = c + (hashval << 6) + (hashval << 16) - hashval;

   return hashval;
}

