
#include "../headers.h"

/*
gets as input symbols table and target symbol.
returns the symbol's line number and wether or not it's extern.
*/
void get_is_extern_and_line_num(Array *symbols_table ,char *symbol, int *line_num, int *is_extern){
    int i =0; 
    i = searchArray(symbols_table, symbol);
    if(i != -1){
        *line_num = symbols_table->symbol[i].line_num;
        if(symbols_table->symbol[i].ext)
            *is_extern = 1;
	else
	    *is_extern = 0;
    }
}

int reading_file_second_time(Array *symbols_table, Binary_table *instructions_table, const char **instructions, FILE *p_outputFile, char *filename){
   
    char line[82];
    char file_name[100];
    int i, j;
    int error_counter = 0;
    int extern_counter = 0;
    Array entry_table;
/*
variables for analyzing contents of line in file
*/
    char *is_data;
    char *is_extern;
    char *is_string;
    char *is_entry;
    bool found;
    initArray(&entry_table);
  
    while(fgets(line, sizeof(line), p_outputFile) != NULL){
        
        if(is_empty(line) || is_comment(line))
            continue;
	
	/*
	analyzing contents of line
	*/
        is_data = strstr(line, ".data");
        is_extern = strstr(line, ".extern");
        is_string = strstr(line, ".string");
        is_entry = strstr(line, ".entry");
       
       
        if(is_data || is_string)
            continue;
        if(is_extern){
            extern_counter++;
            continue;
        }
	/*
	saving all symbols that are declared as 'entry'
	*/
        if(is_entry){
            int temp = 0;
            add_Extern_Entry_Symbol(&entry_table,&error_counter, line, &temp);        
        }
    }
    
	/*
	looping through entry symbols array and making sure each of them is initialyzed in file
	*/
    found = false;
    for(i = 0; i < entry_table.size; i++){
        found = false;
        for(j = 0; j < symbols_table->size; j++){
            char *symbol1 = entry_table.symbol[i].name;
            char *symbol2 = symbols_table->symbol[j].name; 
            if(strcmp(symbol1, symbol2) ==0){
                symbols_table->symbol[j].ent = true;
                found = true;
            }
        }
        if(!found){
            error_counter ++;
            printf("Error: declared entry symbol '%s' without initialzing it.\n", entry_table.symbol[i].name);
        }
    }


    if(error_counter > 0){
        printf("Errors found in file. Terminating program.\n");
        free(entry_table.symbol);
        return 1;
    }    
	/*
	creating entry files and extern files if needed.
	*/
    if(extern_counter >0){
        sprintf(file_name, "%s.ext", filename);
        create_extern_file(symbols_table, instructions_table, file_name);
    }
    if(entry_table.size >0){
        sprintf(file_name, "%s.ent", filename);
        create_entry_file(symbols_table, file_name);
    }

    symbolToBinary(instructions_table, symbols_table);
    free(entry_table.symbol);
    return 0;
}

void create_extern_file(Array *table,Binary_table* instructions_table, char *file_name){

    int extern_counter = 0;
    int i;
    FILE *extern_file = fopen(file_name, "w");
    if(extern_file == NULL){
        printf("Error: Could not open entry file.\n");
        return;
    }
    
    
    for(i = 0; i< instructions_table->size; i++){
        int line_num = 0;
        int is_extern = 0;
        char *symbol = instructions_table->table[i].bin_str;
        if(symbol[0] =='?'){
            char *new_symbol = (symbol +1);
            get_is_extern_and_line_num(table, new_symbol, &line_num, &is_extern);
            if(is_extern){
                fprintf(extern_file,"%s\t%d\n", new_symbol, (i+100));
		extern_counter ++;
            }
            
        }
        
    }
    fclose(extern_file);
    if(extern_counter == 0){
	remove(file_name);
    }
}



void create_entry_file(Array *table, char *file_name){
    int i;
    FILE *entry_file = fopen(file_name, "w");
    if(entry_file == NULL){
        printf("Error: Could not open entry file.\n");
        return;
    }
        
    
    for (i = 0; i < table->size; i++){
        if(table->symbol[i].ent){
            fprintf(entry_file, "%s\t%d\n", table->symbol[i].name, table->symbol[i].line_num);
        }
        
    }
    fclose(entry_file);
}
/*
translating all unknown symbols from first read to binary
*/
void symbolToBinary(Binary_table *instructions_table, Array *symbols_table){
    int line_num = 0;
    char *temp_bin = (char*)calloc(15, sizeof(char));
    int external = 0; /*flag to check if it is external or local symbol*/
    int i;
    for (i = 0; i < instructions_table->size; i++) {
        if(instructions_table->table[i].bin_str[0] == '?'){ 
            get_is_extern_and_line_num(symbols_table, &instructions_table->table[i].bin_str[1], &line_num, &external);

            temp_bin = decimalToBinary(line_num, 12);
            if(external==1){
                strcat(temp_bin, "01");
            }
            else if(external==0){
                strcat(temp_bin, "10");
            }
            strcpy(instructions_table->table[i].bin_str,temp_bin);
	    memset(temp_bin, '0', 14);
            temp_bin[14] = '\0';
        }
    }
    free(temp_bin);
}
