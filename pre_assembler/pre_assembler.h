

typedef struct {

	char word[MAX_LINE]; /*macro name*/
	char *content; /*macro content*/

} Macro;

typedef struct {
    Macro *mcr_arr;
    int mcr_arr_len;/*number of macros in this array*/
	int mcr_arr_size;/*macro array alloc size*/
} Macro_arr;


/*function that initializes the array of macros*/
void init_macro_arr(Macro_arr *macros);

/*function that checks if a given string is a 

name of a macro already in the system*/

int is_macro(char *str, Macro_arr *macros);



/*func that adds a creates a new macro and adds it to the arr of macros*/

void add_word(char *word, char *cont, Macro_arr *macros);



/*function that adds string to another string (dynamically).*/

void append_string(char **str, const char *suffix);



/*function used to free the content fields of the macros*/

void free_mcr_cont(Macro_arr *macros);



/*the main function that runs the preassembler process*/

void pre_assembler(FILE *file, FILE *output_file);









