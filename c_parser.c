#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Stack
{
    char *array;
    int top;
    int capacity;
} Stack;

Stack *createStack(int capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (!stack)
        return NULL;

    stack->array = (char *)malloc(sizeof(char) * capacity);
    if (!stack->array)
    {
        free(stack);
        return NULL;
    }

    stack->capacity = capacity;
    stack->top = -1;
    return stack;
}

void freeStack(Stack *stack)
{
    if (!stack)
        return;
    free(stack->array);
    free(stack);
}

bool isEmpty(Stack *stack)
{
    return stack->top == -1;
}

bool isFull(Stack *stack)
{
    return stack->top == stack->capacity - 1;
}

bool push(Stack *stack, char c)
{
    if (isFull(stack))
        return false;
    stack->array[++stack->top] = c;
    return true;
}

bool pop(Stack *stack, char *out)
{
    if (isEmpty(stack))
        return false;
    *out = stack->array[stack->top--];
    return true;
}

bool peek(Stack *stack, char *out)
{
    if (isEmpty(stack))
        return false;
    *out = stack->array[stack->top];
    return true;
}

int main()
{
    char fileInput[1024];
    char fileData[1024];
    char fullPath[1024];
    FILE *filePtr;

    Stack *stack = createStack(100000);
    if (!stack)
        return 1;

    printf("Please enter the file you'd like to check: \n");
    scanf("%s", fileInput);

    strcpy(fullPath, "./test_files/");

    strcat(fullPath, fileInput);

    filePtr = fopen(fullPath, "r");

    if (filePtr)
    {
        int lineNum = 0;
        bool errorFound = false;

        while (fgets(fileData, 128, filePtr) != NULL)
        {
            lineNum++;
            for (int i = 0; fileData[i] != '\0'; i++)
            {

                char c = fileData[i];

                // opener -> push
                if (c == '(' || c == '{' || c == '[')
                {
                    if (!push(stack, c))
                    {
                        printf("ERROR: stack overflow, line %d.\n", lineNum);
                        errorFound = true;
                        break;
                    }
                }
                // closer -> match top
                else if (c == ')' || c == '}' || c == ']')
                {
                    char topOpen;

                    // no opener to match
                    if (!peek(stack, &topOpen))
                    {
                        char needed = (c == ')') ? '(' : (c == '}') ? '{'
                                                                    : '[';
                        printf("error:  missing %c or extra %c, line %d.\n", needed, c, lineNum);
                        errorFound = true;
                        break;
                    }

                    // figure out expected closer based on topOpen
                    char expected =
                        (topOpen == '(') ? ')' : (topOpen == '{') ? '}'
                                             : (topOpen == '[')   ? ']'
                                                                  : '\0';

                    if (c != expected)
                    {
                        printf("error:  found a %c, but expected %c, line %d.\n", c, expected, lineNum);
                        errorFound = true;
                        break;
                    }

                    // matched -> pop
                    pop(stack, &topOpen);
                }
            }
        }
        if (!errorFound )
            {
                if(!isEmpty(stack)){
                char topOpen;
                peek(stack, &topOpen);
                char needed = (topOpen == '(') ? ')' : (topOpen == '{')
                                    ? '}'
                                    : (topOpen == '[') ? ']'
                                                       : '\0';
                printf("error:  missing %c , EOF.\n", needed);
            }
        }
    }
    else
    {
        printf("File not found in test files, please try again!\n");
    }
    freeStack(stack);
    return 0;
}