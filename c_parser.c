#include <stdio.h>
#include <string.h>

int main(){
    char fileInput[100];
    char fileData[100];
    char fullPath[150];
    FILE* filePtr;

    printf("Please enter the file you'd like to check: \n");
    scanf("%s", fileInput);

    strcpy(fullPath, "./test_files/");

    strcat(fullPath, fileInput);

    filePtr = fopen(fullPath, "r");

    if(filePtr){
        while(fgets(fileData, 128, filePtr) != NULL){
            printf("%s \n", fileData);
        }
    }
    else{
        printf("File not found in test files, please try again!\n");
    }
    return 0;
}