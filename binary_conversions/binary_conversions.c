
#include "../headers.h"


char *pos_to_binary(unsigned int num, int bit_count) {
    int i;
    /*Allocate memory for the binary string*/
    char *binary = (char*)malloc(bit_count + 1);
    int pos;
    if (binary == NULL) {
        printf("Error: Failed to allocate memory.\n");
        return NULL;
    }
    /*Convert the number to binary*/
    
    for (i = 0; i < bit_count; i++) {
        binary[i] = '0';
    }
    pos = bit_count - 1;
    while (num != 0 && pos >= 0) {
        if (num & 1) {
            binary[pos] = '1';
        }
        num = num >> 1;
        pos--;
    }
    /*Return the binary string*/
    binary[bit_count] = '\0';
    return binary;
}

char *neg_to_binary(int num, int bit_count) {
    int pos;
    int i;
    /*Allocate memory for the binary string*/
    char *binary = (char*)malloc(bit_count + 1);
    if (binary == NULL) {
        printf("Error: Failed to allocate memory.\n");
        return NULL;
    }
    /*Convert the number to binary using 2's complement*/
    
    for (i = 0; i < bit_count; i++) {
        binary[i] = '0';
    }
    pos = bit_count - 1;
    while (num != 0 && pos >= 0) {
        if (num & 1) {
            binary[pos] = '1';
        }
        num = num >> 1;
        pos--;
    }
    if (pos >= 0) {
        binary[pos] = '1';
        pos--;
        while (pos >= 0) {
            binary[pos] = '0';
            pos--;
        }
    }
    /* Return the binary string*/
    binary[bit_count] = '\0';
    return binary;
}


char* decimalToBinary(int decimalNum, int size) {
    if (decimalNum >= 0) {
        return pos_to_binary(decimalNum, size);
    } else {
        return neg_to_binary(decimalNum, size);
    }
}

int find_string(const char **array, const char *str) {
    int i;
    for (i = 0; array[i] != NULL; i++) {
        if (strcmp(array[i], str) == 0) {
            return i;
        }
    }
    return -1; /*not found*/
}


char* opecode_to_binary(const char **arr, const char *str){
    int num;
    num = find_string(arr,str);
    return decimalToBinary(num,4);
}

char *get_next_word(char *string, int *position) {
    static char word[80]; /* static variable to store the word*/
    int i = *position, j = 0;
    
    /*skip any spaces or commas*/
    while (isspace(string[i]) || string[i] == ',' || \
    string[i] == '(' || string[i] == ')') {
        i++;
    }
    
    /*copy the characters of the word to the word buffer*/
    while (string[i] != '\0' && !isspace(string[i]) && string[i] != ',' \
    && string[i] != '(' && string[i] != ')') {
        word[j] = string[i];
        i++;
        j++;
    }
    
    /*adds null character to the end of the word string*/
    word[j] = '\0';
    
    /*update the position variable to the next word*/
    *position = i;
    
    /* return the word buffer */
    return word[0] != '\0' ? word : NULL;
}


int isSymbol(char* str) {
    int len = strlen(str);
    if (len > 0 && str[len-1] == ':') {
        return 1;
    }
    return 0;
}


char *operands_params(char *orig, char *dest, const char** registers){
    char *bit_str = malloc(sizeof(char) * 4);
    if (orig == NULL){
        strcpy(bit_str,"00");
        
    }
    else if(orig != NULL){
        if(find_string(registers, orig) != -1){ /*Type 3*/
            strcpy(bit_str,"11");
        }
        else if(orig[0] == '#'){ /*Type 0*/
            strcpy(bit_str, "00");
        }
        else{ /*type 1*/
            strcpy(bit_str, "01");
        }
    }
    
    if(find_string(registers, dest) !=-1){ /*Type 3*/
        strcat(bit_str,"11");
    }
    else if(dest[0] == '#'){ /*Type 0*/
        strcat(bit_str, "00");
    }
    else{ /*type 1*/
        strcat(bit_str, "01");
    }
    return bit_str;
}

void string_to_binary(char* str, Binary_table *data_table, int *line_num){
    /* Find the first occurrence of a quote*/
    char* start = strchr(str, '\"');

    /* Loop over each character between the opening and closing quotes*/
    char *p;
    for (p = start + 1; *p!='"'; p++) {
        char *temp;
        temp = decimalToBinary((int)*p,14);
        addBinaryLine(data_table, temp, line_num);
        free(temp);
    }
    addBinaryLine(data_table, "00000000000000", line_num); /*end of string char '\0'*/
}

void data_to_binary(char* data, Binary_table *data_table, int *position, int *line_num){
    char* word;
    while((word=get_next_word(data, position)) != NULL){ /*gets each number into word*/
        char *temp;
        temp = decimalToBinary(atoi(word),14); /*converts number to binary*/
        addBinaryLine(data_table, temp, line_num); /*adds to data table*/
        free(temp);
    }
}

char* registers_addressing(char* orig_reg, char* dest_reg) {
    char* reg_part1;
    char* reg_part2;
    char *final_line = malloc(sizeof(char) * 14);
    final_line[0] = '\0';
 
    if(orig_reg == NULL) {
        /*only destination register exists*/
        reg_part1 = decimalToBinary(atoi(&dest_reg[1]),6);
        
        strcat(final_line,"000000"); /* no orig reg */
        strcat(final_line,reg_part1); /*adds the dest reg binary value*/
        strcat(final_line,"00"); /*A,R,E value*/
        return final_line;
    }
    else if(dest_reg == NULL) {
        /*only origin register exists*/
        reg_part2 = decimalToBinary(atoi(&orig_reg[1]),6);
        strcat(final_line,reg_part2); /*adds the orig reg binary value*/
        strcat(final_line,"000000"); /* no dest reg */
        strcat(final_line,"00"); /*A,R,E value*/
        return final_line;
    }
    else {
        /*both registers exists*/
        reg_part1 = decimalToBinary(atoi(&orig_reg[1]),6);
        reg_part2 = decimalToBinary(atoi(&dest_reg[1]),6);
        strcat(final_line,reg_part1); /*adds the dest reg binary value*/
        strcat(final_line,reg_part2); /*adds the orig reg binary value*/
        strcat(final_line,"00"); /*A,R,E value*/
        return final_line;
    }
}

void other_words(char* src_operand, char* dest_operand, const char** registers, \
Binary_table *Instructions_table, int* line_num){
    char *temp;
    char *temp1;
    char *temp2;
    char *temp3;
    if(src_operand == NULL){ /*if only dest exists*/
        temp = (char*)calloc(31, sizeof(char));
        if(find_string(registers,dest_operand)!=-1){/*if dest a register*/
            temp = registers_addressing(NULL, dest_operand);
            addBinaryLine(Instructions_table, temp, line_num);
        }
        else if(dest_operand[0]=='#'){/*if dest a number*/
            temp = decimalToBinary(atoi(dest_operand+1),12);
            strcat(temp, "00");
            addBinaryLine(Instructions_table, temp, line_num);
        }
        else{/*if dest a symbol*/
            strcat(temp,"?");
            strcat(temp, dest_operand);
            addBinaryLine(Instructions_table, temp, line_num);
        }
        free(temp);
        return;
    }
    else if((find_string(registers,src_operand) != -1) && (find_string(registers,dest_operand) != -1)){
        /*if both registers*/
        temp1 = registers_addressing(src_operand, dest_operand);
        addBinaryLine(Instructions_table, temp1, line_num);
        free(temp1);
        return;
    }
    else if(src_operand != NULL){ /*if src exists*/
        temp2 = (char*)calloc(31, sizeof(char));
        if(find_string(registers,src_operand)!=-1){/*if source a register*/
            temp2 = registers_addressing(src_operand, NULL);
            addBinaryLine(Instructions_table, temp2, line_num);
            
        }
        else if(src_operand[0]=='#'){/*if source a number*/
            temp2 = decimalToBinary(atoi(src_operand+1),12);
            strcat(temp2, "00");
            addBinaryLine(Instructions_table, temp2, line_num);
        }
        else{/*if source a symbol*/
            strcat(temp2,"?");
            strcat(temp2, src_operand);
            addBinaryLine(Instructions_table,temp2, line_num);
        }
        free(temp2);
    }
    temp3 = (char*)calloc(31, sizeof(char));
    if(find_string(registers,dest_operand)!=-1){/*if dest a register*/
        temp3 = registers_addressing(NULL, dest_operand);
        addBinaryLine(Instructions_table, temp3, line_num);
    }
    else if(dest_operand[0]=='#'){/*if dest a number*/
        temp3 = decimalToBinary(atoi(dest_operand+1),12);
        strcat(temp3, "00");
        addBinaryLine(Instructions_table, temp3, line_num);
    }
    else{/*if dest a symbol*/
        strcat(temp3,"?");
        strcat(temp3, dest_operand);
        addBinaryLine(Instructions_table, temp3, line_num);
    }
    free(temp3);
}

void create_binary_from_line(const char *cur_line, const char** instructions, const char** registers, \
Binary_table *instructions_table, Binary_table *data_table, int *IC, int *DC){
    char temp[31] = {'?'};  /*for the unknown symbol*/
    const char* first_type[] = {"mov", "cmp", "add", "sub","lea", NULL}; /*2 operands*/
    const char* second_type[] = {"not","clr","inc", "dec", "jmp", "bne", "red","prn","jsr", NULL}; /*1 operand*/
    const char* third_type[] = {"rts","stop", NULL};/* 0 operands*/
    char *first_word = (char*)calloc(14, sizeof(char));
    char opecode[80], src_operand[80], dest_operand[80], line[80];
    char *check_word;
    char first_operand[80];
    char *ope_bin, *address_type, *params;
    int position = 0;
    strcpy(line, cur_line);
    strcpy(opecode,get_next_word(line, &position));
    if(isSymbol(opecode)){ /*if begin with symbol, skip it*/
        strcpy(opecode,get_next_word(line, &position));
    }
    if(find_string(third_type, opecode) != -1){ /*third type of opecode*/
        strcat(first_word, "0000");/*bits 13-10*/
        ope_bin = opecode_to_binary(instructions,opecode);
        strcat(first_word, ope_bin);/*bits 9-6*/
        free(ope_bin);
        strcat(first_word, "0000");/*bits 5-2*/
        strcat(first_word, "00");/*bits 1-0*/
        addBinaryLine(instructions_table, first_word, IC);
        free(first_word);
    }
    else if(find_string(second_type, opecode) != -1){ /*second type of opecode*/
        
        strcpy(first_operand,get_next_word(line, &position));
        if((check_word = get_next_word(line, &position)) != NULL){ /*jump addressing instruction*/
            strcpy(src_operand,check_word);
            strcpy(dest_operand,get_next_word(line, &position));
            
            params = operands_params(src_operand,dest_operand, registers);
            strcat(first_word,params);/*bits 13-10*/
            free(params);
            
            ope_bin = opecode_to_binary(instructions,opecode);
            strcat(first_word, ope_bin);/*bits 9-6*/
            free(ope_bin);
            strcat(first_word,"0010");/*bits 5-2*/
            strcat(first_word, "00");/*bits 1-0*/
            addBinaryLine(instructions_table, first_word, IC);
            free(first_word);
            
            strcat(temp, first_operand); 
            addBinaryLine(instructions_table, temp, IC);
            other_words(src_operand, dest_operand, registers, instructions_table, IC);
        }
        else{
            strcpy(dest_operand,first_operand);
            strcat(first_word, "0000");/*bits 13-10*/
            ope_bin = opecode_to_binary(instructions,opecode);
            strcat(first_word, ope_bin);/*bits 9-6*/
            free(ope_bin);
            address_type = operands_params(NULL,dest_operand, registers);
            strcat(first_word,address_type);/*bits 5-2*/
            free(address_type);
            strcat(first_word, "00");/*bits 1-0*/
            addBinaryLine(instructions_table, first_word, IC);
            free(first_word);
            other_words(NULL, dest_operand, registers, instructions_table, IC);
        }
    }
    else if(find_string(first_type, opecode) != -1){/*if first type of opecode*/
        strcpy(src_operand,get_next_word(line, &position));
        strcpy(dest_operand,get_next_word(line, &position));
        strcat(first_word, "0000");/*bits 13-10*/
        ope_bin = opecode_to_binary(instructions,opecode);
        strcat(first_word, ope_bin);/*bits 9-6*/
        free(ope_bin);
        address_type = operands_params(src_operand,dest_operand, registers);
        strcat(first_word,address_type);/*bits 5-2*/
        free(address_type);
        strcat(first_word, "00");/*bits 1-0*/
        addBinaryLine(instructions_table, first_word, IC);
        other_words(src_operand, dest_operand, registers, instructions_table, IC);
    }
    else if(strcmp(opecode,".string") == 0){/*if string line converts the chars*/
        string_to_binary(line, data_table, DC);
    }
    else if(strcmp(opecode,".data") == 0){/*if a data line, converts the ints*/
        data_to_binary(line, data_table, &position, DC);
    }
}




