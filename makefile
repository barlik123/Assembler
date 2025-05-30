


all: headers.h data_struct/data_structure.c validate_input/validate_input.c binary_conversions/binary_conversions.c second_read/second_read.c first_read/first_read.c pre_assembler/pre_assembler.c app.c
	gcc -ansi -Wall -pedantic headers.h data_struct/data_structure.c validate_input/validate_input.c binary_conversions/binary_conversions.c second_read/second_read.c first_read/first_read.c pre_assembler/pre_assembler.c app.c -o app -lm



