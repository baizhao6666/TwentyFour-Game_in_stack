/**------------------------------------------
Program 3: TwentyFour Part Two
Course: CS 211, Spring 2023, UIC
System: CLion on MacOs
Author: Baizhao Zhao, starter code provided by Dr. Ellen Kidane
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * struct for linked list
 */
typedef struct NODE {
    char data;
    struct NODE *pNext;
} Node;

/**
 * struct for stack
 */
typedef struct STACK {
    Node *top;
    int size;
} Stack;

/**
 * push a item into stack
 * @param stack the target stack
 * @param data the value of new item
 */
void push(Stack *stack, char data) {
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->pNext = stack->top;
    stack->top = node;
    stack->size++;
}

/**
 * pop a item form stack
 * @param stack the target stack
 * @return the value of the top of stack; -1 - if the stack is empty
 */
char pop(Stack *stack) {
    if (stack->top == NULL) {
        return -1;
    }
    Node *node = stack->top;
    char ret = node->data;
    stack->top = node->pNext;
    free(node);
    stack->size--;
    return ret;
}

/**
 * check the stack is empty or not
 * @param stack the target stack
 * @return 0 - stack is empty; 1 - stack is not empty
 */
int isEmpty(Stack *stack) { return stack->size == 0; }

/**
 * get the data of the top of given stack
 * @param stack given stack
 * @return the data member, 0 - when the stack is empty
 */
char peek(Stack *stack) {
    // if stack is empty
    if (isEmpty(stack)) {
        return 0;
    }
    return stack->top->data;
}

/**
 * get the size of the given stack
 * @param stack given stack
 * @return size
 */
int sizeOfStack(Stack* stack) {
    return stack->size;
}

/**
 * free all the node in stack
 * @param stack the target stack
 */
void clear(Stack *stack) {
    while (stack->top != NULL) {
        Node *node = stack->top;
        stack->top = stack->top->pNext;
        free(node);
    }
}

/**
 * create a stack object and initial the variables
 * @return a pointer of Stack
 */
Stack* createStack() {
    Stack* stack = malloc(sizeof(Stack));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

/**
 * used to select the difficulty
 * @param pPPNum a pointer to a 2D array
 * @return the total amount of numbers
 */
int selectDifficulty(char ***pPPNum) {
    // print difficulty information
    printf("Choose your difficulty level: E for easy, M for medium, and H for "
           "hard (default is easy).\n");
    printf("Your choice --> ");

    char choice;        // for user input
    char fileName[81];  // store the filename
    char* input = malloc(81 * sizeof(char));     // for file content
    int size = 10;      // initial size
    int numsTotal = 0;
    scanf("%c", &choice); // get user input
    getchar();

    // for difficulty selection
    switch (choice) {
        case 'M':
            strcpy(fileName, "medium.txt");
            break;
        case 'H':
            strcpy(fileName, "hard.txt");
            break;
        default:
            strcpy(fileName, "easy.txt");
            break;
    }

    // open the file for read
    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == NULL) {
        printf("Error: could not open %s for reading\n", fileName);
        fclose(filePtr);
        exit(-1);
    }

    // allocate memory for 2D array
    *pPPNum = malloc(size * sizeof(char *));
    while (fgets(input, sizeof(input), filePtr) != NULL) {
        if (strcmp(input, "\n") == 0) {
            continue;
        }
        if (numsTotal >= size) { // need to resize
            size *= 2;
            char **pNew = malloc(size * sizeof(char *)); // create a new 2D array
            for (int i = 0; i < numsTotal; ++i) {
                // copy the old array to new array
                pNew[i] = malloc(8 * sizeof(char));
                strcpy(pNew[i], (*pPPNum)[i]);
                free((*pPPNum)[i]); // free the old array
            }
            free(*pPPNum);          // free the memory
            *pPPNum = pNew;         // reassign the 2D array
        }
        // store the nums into array
        (*pPPNum)[numsTotal] = malloc(8 * sizeof(char));
        strcpy((*pPPNum)[numsTotal], input);
        numsTotal++;  // increment the counter

    }

    // close the file
    fclose(filePtr);
    free(input);

    return numsTotal;
}

/**
 * check if the solution is valid
 * @param solution given string of solution
 * @param numsToUse assigned number in use
 * @return 1-valid, 0-not valid
 */
int checkValid(char *solution, char *numsToUse) {
    int numsArr[10] = {};                       // an array for number in use
    // increment the value of the corresponding index
    for (int i = 0; i < strlen(numsToUse); ++i) {
        int idx = numsToUse[i] - '0';
        numsArr[idx]++;
    }
    size_t length = strlen(solution);
    for (int i = 0; i < length; ++i) {
        if (solution[i] == ' ') {   // skip the spaces
            continue;
        }
        // when meet number, decrease the value of corresponding index
        if (solution[i] >= '1' && solution[i] <= '9') {
            numsArr[solution[i] - '0']--;
            continue;
        }
        // when meet other symbol, return false
        if (solution[i] != '+' && solution[i] != '-' && solution[i] != '*' &&
            solution[i] != '/' && solution[i] != '(' && solution[i] != ')') {
            printf("Error! Invalid symbol entered. Please try again.\n");
            return 0;
        }
    }

    // traverse the number array, if we use all four number, every element should be 0
    for (int i = 0; i < 10; ++i) {
        if (numsArr[i] != 0) {
            printf("Error! You must use all four numbers, and use each one only "
                   "once. Please try again.\n");
            return 0;
        }
    }

    return 1;
}

/**
 * check the priority of given operator
 * I used a array just to store the postfix expression
 * but I compute the postfix expression using stack, hopefully that doesn't break the rule
 * @param newOperator given operator
 * @param operator operator to be compared
 * @return 1-given operator is higher
 */
int checkPriority(const char newOperator, const char operator) {
    // '(' is the lowest
    if (operator== '(') {
        return 1;
    }
    // '*','/' are highest
    if (operator == '*' || operator == '/') {
        return 0;
    }
    // '*','/' are highest
    if (newOperator == '*' || newOperator == '/') {
        return 1;
    }
    return 0;
}

/**
 * check the result of the given solution
 * @param solution given solution
 * @return 1-if solution evaluates to 24, 0-if solution did not evaluate to 24
 */
int checkResult(char* solution) {
    size_t size = strlen(solution);     // get the size of the solution
    char result[size];      // an array of postfix expression
    Stack* output = createStack();          // a stack to compute the postfix expression
    int idx = 0;    // the index of array, start from 0
    Stack* operators = createStack();       // a stack for operators
    // traverse the solution string
    for (int i = 0; i < size; ++i) {
        if (solution[i] == ' ') {       // skip the spaces
            continue;
        } else if (solution[i] >= '1' && solution[i] <= '9') {
            // when meet numbers, put into array
            result[idx] = solution[i];
            idx++;
        } else {
            // when meet ')', pop the operators from stack until we meet '('
            if (solution[i] == ')') {
                while (!isEmpty(operators) && peek(operators) != '(') {
                    // put the operators into array
                    result[idx] = pop(operators);
                    idx++;
                }
                // if the operator stack is empty, means there is extra closing parenthesis
                if (isEmpty(operators)) {
                    result[idx] = solution[i];      // put it back to result
                    idx++;
                } else {
                    pop(operators);     // pop the '('
                }
            } else if (solution[i] == '(' || isEmpty(operators) ||
                       checkPriority(solution[i], peek(operators))) {
                // when meet '(' or a higher priority operator, push into stack
                push(operators, solution[i]);
            } else if (!checkPriority(solution[i], peek(operators))) {
                // if the operator is lower priority than the one on the top of stack
                // pop the one on the top of stack and put in into array
                while (!isEmpty(operators) && !checkPriority(solution[i], peek(operators))) {
                    result[idx] = pop(operators);
                    idx++;
                }
                // push the other on into stack
                push(operators, solution[i]);
            }
        }
    }


    // put the remaining elements in stack into array
    while (!isEmpty(operators)) {
        result[idx] = pop(operators);
        idx++;
    }
    result[idx] = '\0';
    clear(operators);
    free(operators);

    // then we get a postfix expression
    // compute the postfix expression using stack
    for (int i = 0; i < idx; ++i) {
        // when meet numbers, push into stack
        if (result[i] >= '1' && result[i] <= '9') {
            push(output, result[i]);
        } else if (result[i] == ')') {      // when there are too many closing parentheses
            printf("Error! Too many closing parentheses in the expression.\n");
            clear(output);
            free(output);        // free the memory
            return 0;
        }else {
            // when meet operator
            // pop an element as right number
            int rightNum = pop(output) - '0';
            // pop one more element as left number
            int leftNum = pop(output) - '0';
            int temp;
            // compute the result depend on the operator
            switch (result[i]) {
                case '+':
                    temp = leftNum + rightNum;
                    break;
                case '-':
                    temp = leftNum - rightNum;
                    break;
                case '*':
                    temp = leftNum * rightNum;
                    break;
                case '/':
                    temp = leftNum / rightNum;
                    break;
            }
            printf("%d %c %d = %d.\n", leftNum, result[i], rightNum, temp);
            // push the result into stack
            push(output, (char)(temp + '0'));
        }
    }

    // if the output stack is more than 1 element, means too many value in the expression
    if (sizeOfStack(output) != 1) {
        printf("Error! Too many values in the expression.\n");
        clear(output);
        free(output);        // free the memory
        return 0;
    }

    // the remaining element in stack is the result
    int res = pop(output) - '0';
    clear(output);
    free(output);        // free the memory
    if (res == 24) {
        printf("Well done! You are a math genius.\n\n");
        return 1;
    }
    printf("Sorry. Your solution did not evaluate to 24.\n\n");
    return 0;
}

/**
 * play the 24-game
 * @param pPNums a 2D array for number to be used
 * @param cnt total amount of numbers to be use
 */
void playGame(char **pPNums, int cnt) {

    // allocate memory
    char *numsToUse = malloc(8 * sizeof(char));
    char *solution = malloc(81 * sizeof(char));

    while (1) {
        strcpy(numsToUse, pPNums[rand() % cnt]);        // get random numbers
        // print the numbers
        printf("The numbers to use are: %c, %c, %c, %c.\n", numsToUse[0],
               numsToUse[2], numsToUse[4], numsToUse[6]);
        printf("Enter your solution: ");
        fgets(solution, 81, stdin);     // get user solution
        solution[strlen(solution) - 1] = '\0';
        // check if the solution is valid
        if (checkValid(solution, numsToUse)) {
            break;
        }
    }

    // check the solution can evaluate to 24
    checkResult(solution);
    free(numsToUse);        // free the memory
    free(solution);
}

int main() {

    srand(1);
    printf("Welcome to the game of TwentyFour Part Two!\n");
    printf("Use each of the four numbers shown exactly once, \n");
    printf("combining them somehow with the basic mathematical operators "
           "(+,-,*,/) \n");
    printf("to yield the value twenty-four.\n");

    char **pPNums = NULL;
    // get the 2D array and amount
    int cnt = selectDifficulty(&pPNums);
    playGame(pPNums, cnt);      // play 24-game

    int menuOption = 0;
    // when the game is finished
    while (menuOption != 3) {
        // play again or exit the game
        printf("Enter: \t1 to play again,\n");
        printf("\t2 to change the difficulty level and then play again, or\n");
        printf("\t3 to exit the program.\n");
        printf("Your choice --> ");
        scanf("%d", &menuOption);
        getchar();
        switch (menuOption) {
            case 1:     // play again in the same difficulty level
                playGame(pPNums, cnt);
                break;
            case 2:     // reselect the difficulty level and play again
                // free the old array
                for (int i = 0; i < cnt; ++i) {
                    free(pPNums[i]);
                }
                free(pPNums);
                // get new array and amount
                cnt = selectDifficulty(&pPNums);
                playGame(pPNums, cnt);
                break;
            default:
                break;
        }

        // exit the game
        if (menuOption == 3) {
            printf("\nThanks for playing!\n");
            printf("Exiting...\n");
            break;
        }
    }

    // free the memory
    for (int i = 0; i < cnt; ++i) {
        free(pPNums[i]);
    }
    free(pPNums);

    return 0;
}