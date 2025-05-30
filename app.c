#include "headers.h"


int main(int argc, char *argv[]) {
    /*
    variables that checks if the first and or second read were successful
    */
    int valid_first_read =0;
    int valid_second_read = 0;
    
    int i;
    char filename[100];
    char file_name_orig[100];
    /*
    output files
    */
    FILE *ob_file;
    FILE *macroed_file;
    FILE *as_file;
    const char* instructions[] = {"mov", "cmp", "add", "sub","not","clr","lea",
        "inc", "dec", "jmp", "bne", "red","prn","jsr","rts","stop", NULL};
    const char* registers[] = {"r0","r1","r2","r3","r4","r5","r6","r7", NULL};
    
    Array symbols_table; /*Declare a new symbols_table*/
    Binary_table data_table; /* Declare a new binary_table*/
    Binary_table instructions_table; /* Declare a new binary_table */
    
    for (i = 1; i < argc; i++) {  
        initArray(&symbols_table);/*initializes a new symbols_table*/
        initBinaryTable(&instructions_table); /* Initialize the binary_table*/
        initBinaryTable(&data_table); /* Initialize the binary_table*/
	/*
	saving original file name since we are changing it later
	*/
        sprintf(file_name_orig, "%s", argv[i]);

        /*opening origin file*/
        sprintf(filename, "%s.as", argv[i]);
        as_file = fopen(filename, "r"); 
        if (as_file == NULL) {
            printf("Error: Could not open file %s\n", filename);
            continue;
        }

        /*opening the file with all macros expanded. */
        sprintf(filename, "%s.am", argv[i]);
        macroed_file = fopen(filename, "w"); 
        if (macroed_file == NULL) {
            printf("Error: Could not open file %s\n", filename);
	    fclose(as_file);
            continue;
        }
        /*opening all macros from the .as file*/
        pre_assembler(as_file, macroed_file);
        fclose(as_file);

        /*reopening file with pointer to the beginning of the file, in read mode*/
        fclose(macroed_file);
        macroed_file = fopen(filename,"r"); 
        if (macroed_file == NULL) {
            printf("Error: Could not reopen file %s\n", filename);
            continue;
        }

       

        

        /*reading file for the first time */
        valid_first_read = reading_file_first_time(&symbols_table, instructions, macroed_file, registers, &instructions_table, &data_table);
        if(valid_first_read == 1)
		continue;

        /*reopening file with pointer to the beginning of the file, in read mode*/
        fclose(macroed_file);
        macroed_file = fopen(filename,"r"); 
        if (macroed_file == NULL) {
            printf("Error: Could not reopen file %s\n", filename);
            continue;
        }
        /*reading file for the second time */
        valid_second_read = reading_file_second_time(&symbols_table, &instructions_table ,instructions, macroed_file, file_name_orig);

	if(valid_second_read == 1)
		continue;

        /* Open the .ob file for writing*/
        sprintf(filename, "%s.ob", argv[i]);
        ob_file = fopen(filename, "w");
        if (ob_file == NULL) {
            printf("Error: Could not open file %s\n", filename);
	    fclose(macroed_file);
            continue;
        }

        build_object_file(&instructions_table, &data_table, ob_file);


	fclose(ob_file);
        fclose(macroed_file); 
        free(symbols_table.symbol);
        free(instructions_table.table);
        free(data_table.table);
    }
    return 0;
}

char* int_to_four_char_string(int input) {
    static char output[5];
    sprintf(output, "%04d", input);
    return output;
}

void convert_to_dots_slashes(char *input, char *output) {
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '0') {
            output[i] = '.';
        } else if (input[i] == '1') {
            output[i] = '/';
        } else {
            printf("Invalid input string!\n");
            return;
        }
    }
    output[i] = '\0';
}

void get_length_and_converted_string(int num, char *input, FILE *fp) {
    char converted[BINARY_SIZE];
    char input_copy[14];
    strncpy(input_copy, input, 14);
    convert_to_dots_slashes(input_copy, converted);
    fprintf(fp, "%s\t%s\n", int_to_four_char_string(num), converted);
}

void build_object_file(Binary_table *instructions_table, Binary_table *data_table, FILE *object_file){
    int i;
    for (i = 0; i < instructions_table->size; i++) { /*add the instruction lines*/
        get_length_and_converted_string(instructions_table->table[i].line_num, instructions_table->table[i].bin_str, object_file);
    }
    for (i = 0; i < data_table->size; i++) {/*adds the data lines*/
        get_length_and_converted_string(data_table->table[i].line_num, data_table->table[i].bin_str, object_file);
    }
}
