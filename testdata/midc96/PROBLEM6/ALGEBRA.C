#include <stdio.h>
#include <stdlib.h>

#define OFILE ofile
#define MAX 20

void print(FILE* file, long op_count, char operation[], long operand[], char varstring[])
{
        long index;

	for(index = 0; index < op_count; index++){
                fprintf(file, "%li ", operand[index]);
		fprintf(file, "%c ", operation[index]);
	}
        fprintf(file, "%li ", operand[op_count]);
	fprintf(file, "= %s\n", varstring);
}

void main( void )
{
	char operation[MAX];
        long  operand[MAX+1];
        long  level[MAX];
        long  currentlevel;
	char varstring[10];
	char equal;
        long  op_count;
        long  index;
	FILE* ifile;
	FILE* ofile;

        ifile = fopen("algebra.in", "rt");
        ofile = fopen("algebra.out", "wt");

	while (!feof(ifile)){
		op_count = 0;
		do{
                        op_count += fscanf(ifile, "%li ", &operand[op_count]);
			operation[op_count-1] = fgetc(ifile);
		} while ((op_count > 0) && (operation[op_count-1] != '='));
		if (op_count == -1) break;
		fscanf(ifile, "%s", varstring);
		op_count--;
		print(OFILE, op_count, operation, operand, varstring);

		currentlevel = 0;
		for(index = 0; index < op_count; index++)
			if(operation[index] == '*' || operation[index] == '/')
				level[index] = currentlevel++;
		for(index = 0; index < op_count; index++)
			if(operation[index] == '+' || operation[index] == '-')
				level[index] = currentlevel++;	

		for(currentlevel = 0; currentlevel < op_count; currentlevel++){
			for(index = 0; level[index]!=currentlevel; index++);
			switch(operation[index]){
				case '+': operand[index] += operand[index+1]; break;
				case '-': operand[index] -= operand[index+1]; break;
				case '*': operand[index] *= operand[index+1]; break;
				case '/': operand[index] /= operand[index+1]; break;
			}
			for(index++; index < op_count; index++){
				operand[index] = operand[index+1];
				operation[index-1] = operation[index];
				level[index-1] = level[index];
			}
			print(OFILE, op_count-currentlevel-1, operation, operand, varstring);
		}
                fprintf(ofile,"\n");
	}
	fclose(ifile);
	fclose(ofile);
}
