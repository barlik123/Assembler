#include "../headers.h"




void initArray(Array *arr) {
    arr->symbol = (Symbol *)malloc(INITIAL_CAPACITY * sizeof(Symbol));
    if(arr->symbol == NULL){
        printf("Failed to allocate memory. Terminating program.\n");
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
}

void addArray(Array *arr, char *element, int *line_number, int type, int is_data) {
    Symbol s;
    if (arr->size % 10 == 0) {
        arr->symbol = (Symbol *) realloc(arr->symbol, (arr->size + 10) * sizeof(Symbol));
    }
    s.ent = false;
    s.ext = false;
    s.line_num = -1;
    strcpy(s.name, element);
    s.line_num = *line_number;
    
    if(type == 1)
        s.ext = true;
    else if(type ==2)
        s.ent = true;

    if(is_data == 1)
        s.is_data = true;
    else{
        s.is_data = false;
    }
    arr->symbol[arr->size] = s;
    (arr->size) ++;
}


int searchArray(Array *arr, char *element) {
    int i;
    for (i = 0; i < arr->size; i++) {
        if (strcmp(arr->symbol[i].name, element) == 0) {
            return i;
        }
    }
    return -1;
}

void initBinaryTable(Binary_table *tablePtr) {
    tablePtr->table = (Binary_line *) malloc(10 * sizeof(Binary_line)); /* Allocate memory for 10 binary_line elements*/
    tablePtr->size = 0; /* Initialize the size of the array to 0*/
}

void addBinaryLine(Binary_table *tablePtr, char *bin_str, int *line_num) {
    Binary_line newLine;
    newLine.line_num = *line_num;
    strcpy(newLine.bin_str, "");
    
    /*Binary_line newLine = (Binary_line) {*line_num, ""};*/
    
    /* Double the size of the array if necessary*/
    if (tablePtr->size % 10 == 0) {
        tablePtr->table = (Binary_line *) realloc(tablePtr->table, (tablePtr->size + 10) * sizeof(Binary_line));
    }

    /* Initialize the new binary_line element and add it to the array*/
    
    strncpy(newLine.bin_str, bin_str, 14); /* Copy the first 14 characters of the string*/
    tablePtr->table[tablePtr->size] = newLine;

    /* Update the size of the array and line number*/
    (tablePtr->size)++;
    (*line_num)++;
}




