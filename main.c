#include <stdio.h>
#include <stdlib.h>

//Declare and rename a struct to use it as a linked list
typedef struct node {
    int digit;
    int carryOver;
    struct node *next;
}node;
node *multiplicandHead;
node *multiplierHead;
node *productHead;

//Read the input file and initialize two linked lists as operands
void createLinkedLists();

//Do the multiplication and return the output as a linked list
void multiply();

//Write the output to the specified file
void writeOutput();

//Sort the linked list for proper file writing
node* createSortedLinkedList(node*);

int main() {
    createLinkedLists();
    multiply();
    writeOutput();
}

void createLinkedLists() {

    //Declare the necessary variables for placing linked list nodes
    node *n;
    node *temp;
    char digit;

    //Declare a file pointer and open the file "input.txt" for reading purpose
    FILE *fp;
    fp = fopen("input.txt", "r");

    //Check if the file exists
    if (fp == NULL) {
        printf("File does not exit! \n");
    } else {

        //Read the first operand's digits and assign them to a linked list
        while ((digit = fgetc(fp)) != '\n') {
            temp = (node*) calloc(1,sizeof (node));
            temp->digit = digit-'0';
            if (multiplicandHead == NULL) {
                multiplicandHead = temp;
            } else {
                n = multiplicandHead;
                temp->next = n;
                multiplicandHead = temp;
            }
        }

        //Read the second operand's digits and assign them to a different linked list
        while ((digit = fgetc(fp)) != EOF) {
            temp = (node*) calloc(1,sizeof (node));
            temp->digit = digit - '0';
            if (multiplierHead == NULL)
                multiplierHead = temp;
            else {
                n = multiplierHead;
                temp->next = n;
                multiplierHead = temp;
            }
        }

        //Close the input file
        fclose(fp);
    }
}

void multiply() {

    //Declare the necessary variables to do multiplication and store the output in a linked list
    int multiplier;
    int multiplierDigitCounter = 0;
    int numberDigitCounter = 0;
    int isLastDigit = 0;
    node *m = multiplierHead;
    node *n;
    node *temp;
    node *p;

    /*Check the operands digits numbers and do the multiplication digit by digit ,so it is possible to store it
    in a linked list */
    while (m != NULL) {
        multiplier = m->digit;
        n = multiplicandHead;
        while (n != NULL) {
            if (multiplierDigitCounter >= 1 && multiplierDigitCounter - numberDigitCounter <= 0) {
                p = productHead;
                for (int i = 0; i < numberDigitCounter; ++i) {
                    if (p->next != NULL) {
                        p = p->next;
                    } else {
                        p->next = (node*) calloc(1,sizeof (node));
                        p = p->next;
                        p->digit = (n->digit * multiplier) + n->carryOver;
                        isLastDigit = 1;
                    }
                }
                if (isLastDigit == 0)
                p->digit = (n->digit * multiplier) + n->carryOver + p->digit;

                //If digit is larger than 10 divide it and carry over the rest to the next digit
                if (p->digit >= 10 && n->next != NULL) {
                    n->next->carryOver = p->digit / 10;
                    p->digit = p->digit % 10;
                } else if (p->digit >= 10) {
                    p->next = (node*) calloc(1,sizeof (node));
                    p->next->digit = p->digit / 10;
                    p->digit = p->digit % 10;
                }
            } else if (multiplierDigitCounter - numberDigitCounter >= 0 && multiplierDigitCounter > 0) {
                numberDigitCounter++;
                continue;
            } else {
                temp = (node*)calloc(1,sizeof(node));
                temp->digit = (n->digit * multiplier) + n->carryOver;
                if (n->next != NULL && temp->digit >= 10) {
                    n->next->carryOver = temp->digit / 10;
                    temp->digit = temp->digit % 10;
                }
                if (numberDigitCounter == 0) {
                    productHead = temp;
                } else if (n->next == NULL && temp->digit >= 10) {
                    p = productHead;
                    while (p->next != NULL) {
                        p = p->next;
                    }
                    p->next = (node*) calloc(1,sizeof (node));
                    p->next->digit = temp->digit % 10;
                    p->next->next = (node*) calloc(1,sizeof (node));
                    p->next->next->digit = temp->digit / 10;
                } else {
                    p = productHead;
                    for (int i = 0; i < numberDigitCounter - 1; ++i) {
                        p = p->next;
                    }
                    p->next = temp;
                }
            }
            numberDigitCounter++;
            n = n->next;
        }
        isLastDigit = 0;
        numberDigitCounter = 0;
        multiplierDigitCounter++;
        n = multiplicandHead;
        m = m->next;

        //Reset the carry over values
        while (n != NULL) {
            n->carryOver = 0;
            n = n->next;
        }
    }
}

void writeOutput() {
    node *p = createSortedLinkedList(multiplicandHead);

    //Declare file pointer and open "output.txt" file for writing purpose
    FILE *fp;
    fp = fopen("output.txt","w");
    while (p != NULL) {
        putc(p->digit + '0',fp);
        p = p->next;
    }
    putc('\n',fp);
    p = createSortedLinkedList(multiplierHead);
    while (p != NULL) {
        putc(p->digit + '0',fp);
        p = p->next;
    }
    putc('\n',fp);
    p = createSortedLinkedList(productHead);
    while (p != NULL) {
        putc(p->digit + '0',fp);
        p = p->next;
    }

    //Close the file
    fclose(fp);
}

node* createSortedLinkedList(node* outputHead) {

    //Declare necessary variables for sorting given linked list
    node *p = outputHead;
    node *temp;
    node *n;
    node *sortedOutputHead = NULL;

    //Make a new linked list which is reverse of the given one
    while (p != NULL) {
        if (sortedOutputHead == NULL) {
            sortedOutputHead = (node*) calloc(1, sizeof (node));
            sortedOutputHead->digit = p->digit;
        } else {
            temp = (node*) calloc(1,sizeof (node));
            n = sortedOutputHead;
            temp->digit = p->digit;
            temp->next = n;
            sortedOutputHead = temp;
        }
        p = p->next;
    }

    //Return the sorted output's head node
    return sortedOutputHead;
}