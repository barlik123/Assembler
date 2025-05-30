
#include "../headers.h"


void init_macro_arr(Macro_arr *macros){
	/*array of all the macros*/
	macros->mcr_arr = NULL;
	macros->mcr_arr_len = 0;/*number of macros in this array*/
	macros->mcr_arr_size = 0;/*macro array alloc size*/
}

void free_mcr_cont(Macro_arr *macros) {
    int i;
    for (i = 0; i < macros->mcr_arr_len; i++) {
        free(macros->mcr_arr[i].content);
        macros->mcr_arr[i].content = NULL;
    }
    macros->mcr_arr_len = 0;
    macros->mcr_arr_size = 0;
}

int is_macro(char *str, Macro_arr *macros){
	int i;
	for (i = 0; i < macros->mcr_arr_len; i++) {
		if (strcmp(macros->mcr_arr[i].word, str) == 0) {
			return i;
		}
	}
	return -1;
}

void append_string(char **str, const char *suffix) {
    size_t old_size = (*str != NULL) ? strlen(*str) : 0;
    size_t suffix_size = strlen(suffix);
    size_t new_size = old_size + suffix_size + 1;

    char *new_str = realloc(*str, new_size);
    if (new_str == NULL) {
        printf("Failed to realloc\n");
        return;
    }

    memcpy(new_str + old_size, suffix, suffix_size + 1); 
    *str = new_str;
}

void add_word(char *word, char *cont, Macro_arr *macros) {
	if (macros->mcr_arr_len == macros->mcr_arr_size) { /*expanding the alloc*/
		macros->mcr_arr_size = \
		(macros->mcr_arr_size == 0) ? 1 : macros->mcr_arr_size * 2;
		macros->mcr_arr = (Macro *)realloc(macros->mcr_arr,\
		 macros->mcr_arr_size*sizeof(Macro));
	}
	strcpy(macros->mcr_arr[macros->mcr_arr_len].word, word);
	macros->mcr_arr[macros->mcr_arr_len].content = malloc(strlen(cont)+1);
	strcpy(macros->mcr_arr[macros->mcr_arr_len].content, cont);
	macros->mcr_arr_len++;
}

void pre_assembler(FILE *file, FILE *output_file) {
	char line[MAX_LINE];/*line read from the original file (buffer)*/
	char first_word[MAX_LINE];/*keeps the first word of a line*/
	char second_word[MAX_LINE];/*keeps the line cont except the first word*/
	int mcr_flag = 0;/*flag that keeps track if the iteration is cur in a mcr*/
	char name[MAX_LINE];/*save the name of the cur mcr the func is checking*/
	int line_cont;/*keeps track of how many parts are in a line (empty/1word/2parts)*/
	int macro_num;/*saves which mcr is called from the arr of mcrs*/
	char *mcr_cont = NULL;
	Macro_arr macros;
	init_macro_arr(&macros);

	while (fgets(line, sizeof(line), file)){
		line_cont = sscanf(line, "%s %s", first_word, second_word);
		if((line_cont == 1) && ((macro_num = is_macro(first_word, &macros)) != -1)){
			/*if a call to a macro it replaces it with 
			the macro's content*/
			fprintf(output_file, "%s",macros.mcr_arr[macro_num].content);
			continue;
		}
		else if(line_cont == 2 && strcmp(first_word, "mcr") == 0){
			/*if a start of a macro it saves the macro's name and 
			the content's start location*/
			strcpy(name, second_word);
			mcr_flag = 1;
		}
		else if((line_cont == 1) && (strcmp(first_word, "endmcr") == 0)){
			/*when the end of a macro is reached add the macro
			to the list of macros*/
			add_word(name, mcr_cont, &macros);
			memset(name, 0, sizeof(name));
			memset(mcr_cont, 0, sizeof(*mcr_cont)); 
			mcr_flag = 0;
			continue;
		}
		else if (mcr_flag == 1){
			/*if inside a macro*/
			append_string(&mcr_cont, line);
		}	
		else if(mcr_flag == 0){
			/*if not a line of a macro it puts the line in the new file*/
			fprintf(output_file, "%s",line);
		}
	}
	fseek(output_file, 0, SEEK_SET); /*set the new file ptr to the beginning*/
	
	free_mcr_cont(&macros);
}
