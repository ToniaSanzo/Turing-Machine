/*
MIT License

Copyright (c) 2019 Tonia Sanzo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// Author: Tonia T. Sanzo
// Professor: Daniel R. Schlegel
// CSC 344
// 2/27/19

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Tape {
    char val;
    struct Tape *left;
    struct Tape *right;
}Tape;



typedef struct StateInstructions {
    char writeVal;
    // 1 means to the left, 2 means to the right, and 0 means to halt(Because it isn't determined)
    int moveDirection;
    int newState;

}StateInstructions;



/**
*  This is used to read the text file that has all the state information and the content of the tape.
*  @precondition: None.
*  @postcondition: The infinite tape will be initialized and the state diagram based on the text file being read.
*  @argument: The address of the current cell, head cell, and tail cell. Note* they have 2 '*' because this function
*             is a (call-by-reference) and will change the values of the calling data. Also passes in a pointer to
*             a 2-Dimensional array and an int that will update the start & halt state in the main function.
*/
void readInput(Tape **arg_curr_cell, Tape **arg_head,Tape **arg_tail, StateInstructions ***arg_array, int *arg_start, int *arg_halt);


/**
*  Will move the current cell to the right, and if there is not an already instantiated cell to the right the
*  program will create a new cell, and set both the current cell and the tail cell equal to this new cell.
*  @argument: a reference to the current cell pointer and a reference to the current tail cell pointer.
*/
void moveRight(Tape **arg_curr_cell, Tape **arg_tail);



/**
*  Will move the current cell to the left, and if there is not an already instantiated cell to the left the
*  program will create a new cell, and set both the current cell and the head cell equal to this new cell.
*  @argument: a reference to the current cell pointer and a reference to the current head cell pointer.
*/
void moveLeft(Tape **arg_curr_cell, Tape **arg_head);


/**
*  Prints the entirety of the tape from the first element to the last element without changing the current
*  location of the cell that the Turing machines "Read/write" head is located.
*  @arguments: a reference to the head and tail of the doubly linked list array.
*/
void printTape(Tape *arg_head, Tape *arg_tail);





int main()
{
    //------------------ VARIABLES---------------------------------------------------------------------
    // Instantiates a 2-D array.
    StateInstructions **stateArray;
    // This lets the program know which is the current start & halt state the direction to move and the
    // state to change too.
    int currentState, haltState, moveDirection, updateState;
    // Allocates and Initializes the first Tape cell.
    Tape* currentCell = (Tape*)malloc(sizeof(Tape));
    // The Head and Tail are set to the initial cell in the tape.
    Tape *head = currentCell;
    Tape *tail = currentCell;


    //------------------- PROGRAM BODY----------------------------------------------------------------

    printf("Author: Tonia T. Sanzo\nProfessor: Daniel R. Schlegel\nCSC 344\n2/27/19\n");
    system("pause");
    // Initialize the value of the first cell.
    currentCell->val = 'B';
    currentCell->left = NULL;
    currentCell->right = NULL;

    // The doubly linked list, start & halt state, and the stateArray are initialized with the values.
    readInput(&currentCell, &head, &tail, &stateArray, &currentState, &haltState);


    while(currentState != haltState) {

        // Figure out which direction to move.
        moveDirection = stateArray[currentState][(int)currentCell->val].moveDirection;
        updateState = stateArray[currentState][(int)currentCell->val].newState;

        // change the contents of the cell
        currentCell->val = stateArray[currentState][(int)currentCell->val].writeVal;

        // move left and update state
        if(moveDirection == 1){
                moveLeft(&currentCell,&head);
                currentState = updateState;
                continue;
        }

        // move right and update the state
        moveRight(&currentCell,&tail);
        currentState = updateState;
    }

    printf("\nFinal Tape Content: ");
    printTape(head, tail);


    return 0;
}





/**
*  This is used to read the text file that has all the state information and the content of the tape.
*  @precondition: None.
*  @postcondition: The infinite tape will be initialized and the state diagram based on the text file being read.
*  @argument: The address of the current cell, head cell, and tail cell. Note* they have 2 '*' because this function
*             a 2-Dimensional array and an int that will update the start & halt state in the main function.
*/
void readInput(Tape **arg_curr_cell, Tape **arg_head,Tape **arg_tail, StateInstructions ***arg_array, int *arg_start, int *arg_halt) {
    // This is a character string that is used to path to the correct input file.
    char fileName[50];
    // The instructionString are used to hold a line from the input file that
    // will contain the State, ReadVal, WriteVal, MoveDirection, NewState.
    char instructionString[11];
    // This will contain a temporary instruction that will be put into the 2-d array
    StateInstructions temp_Instruction;
    // Used as a memory location to hold the chars and the integer's within a file.
    char char_var;
    int  int_var;

    // Prompts the user for the file name.
    printf("File: ");
    scanf("%s",fileName);
    // appends the end of the string to include the file type
    strcat(fileName,".txt");

    // Create and open a variable of type FILE*.
    FILE *in_file = fopen(fileName, "r"); // READ-ONLY

    // Makes sure that the file can be properly opened, and exits if it can not be.
    if(in_file == NULL) {
        printf("ERROR OPENING FILE\nNOTE* THE PROGRAM APPENDS YOUR INPUT WITH \'.txt\'");
        exit(-1);
    }

    fscanf(in_file,"%c", &char_var);

    printf("Initial Tape Content: ");
    // Creates the infinite tape based on what the file's first line of data is.
    while(char_var != '\n'){
            printf("%c", char_var);
           (*arg_curr_cell)->val = char_var;
            fscanf(in_file,"%c", &char_var);

            // Adds cells for as many elements in the first line of the input stream.
            if(char_var != '\n')
                moveRight(arg_curr_cell,arg_tail);
    }

    // int variable is used to hold the number of states, and the char variable is used to
    // clear the newline character.
    fscanf(in_file,"%d%c", &int_var, &char_var);

    // the arg_array is dynamically allocated for 2-D array, the dimensions are,
    // the # of states * the ASCII alphabet.
    *arg_array = (StateInstructions**)malloc(sizeof(StateInstructions*) * int_var);
    for(int i = 0; i < int_var; i++) {
            (*arg_array)[i] = (StateInstructions*)malloc(sizeof(StateInstructions) * 128);
    }

    // int variable is used to hold the number of states, and the char variable is used to
    // clear the newline character. The int variable is used to create a start state.
    fscanf(in_file,"%d%c", &int_var, &char_var);
    *arg_start = int_var;

    // int variable is used to hold the number of states, and the char variable is used to
    // clear the newline character. The int variable is used to create a halt state.
    fscanf(in_file,"%d%c", &int_var, &char_var);
    *arg_halt = int_var;

    // Reads the instructions from the input file and updates the 2-D array
    while(fgets(instructionString, sizeof(instructionString), in_file)) {
          // the write val is put into the temporary stateInstruction struct.
          temp_Instruction.writeVal = instructionString[4];

          // Ternary converts 'L' to 1, and 'R' to 2 and updates temporary stateInstruction struct.
          temp_Instruction.moveDirection = ((instructionString[6]) == 'L')? 1 : 2;

          // the temporary stateInstruction struct's new state value is initialized.
          temp_Instruction.newState = (instructionString[8]-48);

          // Updates the correct position in the state array to execute the correct instructions
          (*arg_array)[(instructionString[0]-48)][(int)instructionString[2]] = temp_Instruction;
    }

    // sets the current cell to the first cell in the tape.
    *arg_curr_cell = *arg_head;

    // Close the input file stream.
    fclose(in_file);
}



/**
*  Will move the current cell to the right, and if there is not an already instantiated cell to the right the
*  program will create a new cell, and set both the current cell and the tail cell equal to this new cell.
*  @argument: a reference to the current cell pointer and a reference to the current tail cell pointer.
*/
void moveRight(Tape **arg_curr_cell, Tape **arg_tail){
    // Checks to see if the current cell is the last cell in the Doubly-linked list
    if(*arg_curr_cell == *arg_tail){
        // Allocate a new cell.
        Tape *new_cell = (Tape*)malloc(sizeof(Tape));

        // Initialize the new cell, with the correct values.
        new_cell->val = 'B';
        new_cell->right = NULL;
        new_cell->left = (*arg_tail);

        // Sets the tail and current cell, cell to the right of them equal to the new cell.
        (*arg_tail)->right = new_cell;

        // Updates arg_tail, and arg_curr_cell so that they point to the last element in the linked list.
        *arg_tail = new_cell;
        *arg_curr_cell = new_cell;

        return; // exits the function
    }

    // moves the current cell one cell to the right.
    *arg_curr_cell = (*arg_curr_cell)->right;
}



/**
*  Will move the current cell to the left, and if there is not an already instantiated cell to the left the
*  program will create a new cell, and set both the current cell and the head cell equal to this new cell.
*  @argument: a reference to the current cell pointer and a reference to the current head cell pointer.
*/
void moveLeft(Tape **arg_curr_cell, Tape **arg_head) {
    // Checks to see if the current cell is the first cell in the Doubly-linked list
    if(*arg_curr_cell == *arg_head){
        // Allocate a new cell.
        Tape *new_cell = (Tape*)malloc(sizeof(Tape));

        // Initialize the new cell, with the correct values.
        new_cell->val = 'B';
        new_cell->right = (*arg_head);
        new_cell->left = NULL;

        // Sets the head and current cell, cell to the left of them equal to the new cell.
        (*arg_head)->left = new_cell;

        // Updates arg_head, and arg_curr_cell so that they point to the first element in the linked list.
        *arg_head = new_cell;
        *arg_curr_cell = new_cell;

        return; // exits the function
    }

    // moves the current cell one cell to the left.
    *arg_curr_cell = (*arg_curr_cell)->left;
}


/**
*  Prints the entirety of the tape from the first element to the last element without changing the current
*  location of the cell that the Turing machines "Read/write" head is located.
*  @arguments: a reference to the head and tail of the doubly linked list array.
*/
void printTape(Tape *arg_head, Tape *arg_tail) {
    Tape *tempCell = arg_head;

    while(1){
        printf("%c",tempCell->val);
        if(tempCell == arg_tail)
            return;
        tempCell = tempCell->right;
    }
}
