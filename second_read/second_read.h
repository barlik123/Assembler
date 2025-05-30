
/*
gets line as input, checks if symbol in line is extern and gets its line number (in ob file).
updates pointers.
*/
void get_is_extern_and_line_num(Array *symbols_table ,char *symbol, int *line_num, int *is_extern);


/*
create an entry file from array that consists only 'entry' symbols
*/
void create_entry_file(Array *entry_table, char *file_name);

/*
creates a file that holds all extern symbols and their lines. 
*/
void create_extern_file(Array *table,Binary_table* instructions_table, char *file_name);

/*
updates unknown symbols from first read to binary
*/
void symbolToBinary(Binary_table *instructions_table, Array *symbols_table);
/*
returns true if line is empty (only white chars)
*/
bool is_empty(const char *line);
/*
returns true if given line is comment (starts with ';')
*/
bool is_comment(const char *line);
/*
adds extern or entry symbol that is in line into entry_table, assigns line num
*/
void add_Extern_Entry_Symbol(Array *entry_table,int *error_counter,const char * line, int *temp); 

