#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <conio.h>
#include <string.h>

int  lower[10][10];
int  upper[10][10];
int  constant[10][11];
int  base[10];
int  dim[10];
int  index[10][10];
char name[10][11];

void cal_constant (int array, int tnum);
void cal_c0 (int array);
int  cal_address (int array);
int  search_name (int size, char * key);

void cal_constant (int array, int tnum)
 {
  constant[array][tnum] = (upper[array][tnum] - lower[array][tnum]
			+ 1) * constant[array][tnum + 1];
 }

void cal_c0 (int array)
 {
  int i;
  int sum = 0;

  for (i = 1; i <= dim[array]; i++)
   {
    sum += constant[array][i] * lower[array][i-1];
   }

  constant[array][0] = base[array] - sum;
 }

int cal_address (int array)
 {
  int i;
  int sum = 0;

  for (i = 1; i <= dim[array]; i++)
   sum += constant[array][i] * index[array][i-1];

  return constant[array][0] + sum;
 }

int search_name (int size, char * key)
 {
  int pos;

  for (pos = 0; pos < size; pos++)
   {
    if (strcmp (key, name[pos]) == 0)
     return pos;
   }
   return pos;
 }

void main ()
 {
  ifstream input_file;
  ofstream output_file;
  int      array;
  int      i;
  int      j;
  int      num_arrays;
  int      num_calc;
  int      comp_length;
  int      addr;
  char     one_name[11];

  input_file.open ("arraymap.in");
  output_file.open ("arraymap.out");

  input_file >> num_arrays >> num_calc;

  for (i = 0; i < num_arrays; i++)
   {
    input_file >> ws >> name[i] >> base[i] >> comp_length >> dim[i];

    for (j = 0; j < dim[i]; j++)
     input_file >> lower[i][j] >> upper[i][j];

    constant[i][dim[i]] = comp_length;

    for (j = dim[i] - 1; j > 0; j--)
     cal_constant (i, j);

    cal_c0 (i);
   }

  for (i = 0; i < num_calc; i++)
   {
    input_file >> ws >> one_name;

    array = search_name (num_arrays, one_name);

    for (j = 0; j < dim[array]; j++)
     input_file >> index[array][j];

    addr = cal_address (array);

    output_file << one_name << "[";

    for (j = 0; j < dim[array]; j++)
     if (j == 0)
      output_file << index[array][j];
     else
      output_file << ", " << index[array][j];

    output_file << "] = " << addr << endl;
   }

  input_file.close ();
  output_file.close ();
 }



