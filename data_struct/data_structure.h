



typedef struct {
    char name[30]; 
    bool ent;
    bool ext;
    bool is_data;
    int line_num;
    
} Symbol;

typedef struct {
    Symbol *symbol;
    int size;
} Array;

/*
initializes new array
*/
void initArray(Array *arr);
/*
search 'element' in array, return its index (-1 if not found)
*/
int searchArray(Array *arr, char *element);

/*
adds new symbol to the array. 'type' is for determine if symbol in entry, extern or neither.
*/
void addArray(Array *arr, char *element, int *line_number, int type, int is_data);


/*Define the struct for each element of the table*/
typedef struct {
    int line_num;
    char bin_str[31]; 
} Binary_line;

/* Define the struct for the array (table) itself*/
typedef struct {
    Binary_line *table;
    int size;
} Binary_table;

/* Function to initialize a new binary_table*/
void initBinaryTable(Binary_table *tablePtr);

/* Function to add a new binary_line element to the dynamic array (binary_table),
given the binary table, string, and line number*/
void addBinaryLine(Binary_table *tablePtr, char *bin_str, int *line_num);



