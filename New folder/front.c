/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>  // Standard input/output library (for printf, fopen, etc.)
#include <ctype.h>  // Character handling functions (isalpha, isdigit, etc.)

/* Global declarations */
/* Variables */
int charClass;       // Stores the classification of current character (LETTER, DIGIT, etc.)
char lexeme[100];    // Buffer to store the current token being built
char nextChar;       // Holds the next character to be processed
int lexLen;          // Current length of the lexeme
int token;           // Current token code
int nextToken;       // Next token code (used for lookahead)
FILE *in_fp;         // File pointer for input file
FILE *fopen();       // File opening function declaration

/* Function declarations */
void addChar();      // Adds a character to the lexeme
void getChar();      // Gets the next character and classifies it
void getNonBlank();  // Skips whitespace characters
int lex();           // Main lexical analyzer function

/* Character classes */
#define LETTER 0     // Character is an alphabetic letter (a-z, A-Z)
#define DIGIT 1      // Character is a digit (0-9)
#define UNKNOWN 99   // Character is not recognized (operators, etc.)

/* Token codes */
#define INT_LIT 10   // Token is an integer literal (e.g., 123)
#define IDENT 11     // Token is an identifier (e.g., variable names)
#define ASSIGN_OP 20 // Assignment operator (=) - not used in this version
#define ADD_OP 21    // Addition operator (+)
#define SUB_OP 22    // Subtraction operator (-)
#define MULT_OP 23   // Multiplication operator (*)
#define DIV_OP 24    // Division operator (/)
#define LEFT_PAREN 25 // Left parenthesis (()
#define RIGHT_PAREN 26 // Right parenthesis ()) 

/******************************************************/
/* main driver */
int main() {
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in \n"); 
    else {
        getChar(); // Initialize by getting first character
        do {
            lex(); // Call lexical analyzer until EOF
        } while (nextToken != EOF); // Continue until end of file
    } 
    return 0;
}

/*****************************************************/
/* lookup - identifies operators and parentheses */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar(); // Add to lexeme
            nextToken = LEFT_PAREN; // Set token type
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN; 
            break;
        case '+':
            addChar();
            nextToken = ADD_OP; 
            break;
        case '-':
            addChar();
            nextToken = SUB_OP; 
            break;
        case '*':
            addChar();
            nextToken = MULT_OP; 
            break;
        case '/':
            addChar();
            nextToken = DIV_OP; 
            break;
        default:
            addChar(); 
            nextToken = EOF; // End of file
            break;
    } 
    return nextToken;
}

/*****************************************************/
/* addChar - adds nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) { // Check for buffer overflow
        lexeme[lexLen++] = nextChar; // Add character
        lexeme[lexLen] = '\0'; // Null-terminate string
    } 
    else
        printf("Error - lexeme is too long \n"); 
}

/*****************************************************/
/* getChar - gets next character and classifies it */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) { // Read next character
        if (isalpha(nextChar)) // Check if letter
            charClass = LETTER; 
        else if (isdigit(nextChar)) // Check if digit
            charClass = DIGIT;
        else 
            charClass = UNKNOWN; // Operator, parenthesis, etc.
    }
    else
        charClass = EOF; // End of file reached
}

/*****************************************************/ 
/* getNonBlank - skips whitespace characters */
void getNonBlank() {
    while (isspace(nextChar)) // Skip spaces, tabs, newlines
        getChar();
}

/*****************************************************/
/* lex - main lexical analyzer function */
int lex() {
    lexLen = 0; // Reset lexeme length
    getNonBlank(); // Skip leading whitespace
    
    switch (charClass) { 
        /* Parse identifiers */
        case LETTER:
            addChar(); // Add first letter
            getChar(); // Get next character
            // Continue while letters or digits
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar(); 
            }
            nextToken = IDENT; // Set token type
            break;
            
        /* Parse integer literals */
        case DIGIT:
            addChar(); // Add first digit
            getChar();
            // Continue while digits
            while (charClass == DIGIT) {
                addChar();
                getChar(); 
            }
            nextToken = INT_LIT; // Set token type
            break;
            
        /* Handle operators and parentheses */
        case UNKNOWN:
            lookup(nextChar); // Identify operator/paren
            getChar(); // Move to next character
            break;
            
        /* Handle end of file */
        case EOF:
            nextToken = EOF; 
            lexeme[0] = 'E'; 
            lexeme[1] = 'O'; 
            lexeme[2] = 'F'; 
            lexeme[3] = '\0'; 
            break;
    }
    
    /* Print the token information */
    printf("Next token is: %d, Next lexeme is %s\n",
        nextToken, lexeme);
    return nextToken;
}
