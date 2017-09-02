// TOP-DOG DATABASE ARCHIVER - by Frank McCown
// Developed for Programming Contest for Ron Pacheco - Harding Univ

#include <iostream.h>
#include <stdio.h>
#include <string.h>

main()
{
  #define TABLE_NAME_LEN  25
  #define MAX_DATA_LEN    100
  #define MAX_COLS	  100

  #define TABLES_IN	  "TABLES.IN"
  #define DESCRIBE_IN	  "DESCRIBE.IN"
  #define DISPATCH_IN	  "DISPATCH.IN"
  #define DISPATCH_OUT    "DISPATCH.OUT"
  #define EXPORT_OUT      "EXPORT.OUT"

  typedef char col_array_type[TABLE_NAME_LEN];

  FILE *table_fp;          // TABLES.IN
  FILE *desc_fp;           // DESCRIBE.IN
  FILE *dispatch_fp;       // DISPATCH.IN
  FILE *disin_fp;          // DISPATCH.OUT
  FILE *export_fp;         // EXPORT.OUT

  char t_name[TABLE_NAME_LEN];
  char table_name[TABLE_NAME_LEN];
  char search_name[MAX_DATA_LEN];
  char data_in[MAX_DATA_LEN];
  char col_name[25];
  char col_type[8];
  char end_char = ' ';
  int col_size, i, num_of_cols, num_of_rows;
  col_array_type col[MAX_COLS];            // Array to hold col names
  int colsize[MAX_DATA_LEN];               // Array to hold col sizes

  table_fp = fopen(TABLES_IN, "r");
  dispatch_fp = fopen(DISPATCH_OUT, "w");
  export_fp = fopen(EXPORT_OUT, "w");

  while (!feof(table_fp))
  {
    fscanf(table_fp, "%s\n", table_name);

    desc_fp = fopen(DESCRIBE_IN, "r");

    sprintf(t_name, "");
    while( (!feof(desc_fp)) && (strcmp(table_name, t_name) != 0) )
      fscanf(desc_fp, "%s\n", t_name);

    if (strcmp(table_name, t_name) == 0)
    {
      // Start building SQL statement
      fprintf(dispatch_fp, "SELECT ");

      col_name[0] = ' ';
      end_char = ' ';
      i = 0;
      while( (!feof(desc_fp)) && (col_name[0] != '#') )
      {
	fscanf(desc_fp, "%s", col_name);

	if (col_name[0] != '#')
	{
	  // Place , or space after last col_name in SQL statement
	  if (end_char == ' ')
	    end_char = ',';
	  else
	    fprintf(dispatch_fp, ", ");

	  fscanf(desc_fp, " %s", col_type);

	  // If it's VARCHAR then there must be a size
	  if (strcmp(col_type, "VARCHAR") == 0)
	  {
	    fscanf(desc_fp, " %i\n", &col_size);
	    fprintf(dispatch_fp, "(CHAR) ");

	    colsize[i] = col_size;
	  }
	  else
	  {
	    fscanf(desc_fp, "\n");

	    // DATE must be a CHAR
	    if (strcmp(col_type, "DATE") == 0)
	      fprintf(dispatch_fp, "(CHAR) ");
	    else
	      fprintf(dispatch_fp, "(NUM) ");

	    // Numbers and dates don't have sizes
	    colsize[i] = 0;
	  }

	  fprintf(dispatch_fp, "%s", col_name);

	  strcpy(col[i], col_name);

	  ++i;
	}
	else        // read end of line
	  fscanf(desc_fp, "\n");
      }

      num_of_cols = i;

      // Finish off SQL statement
      fprintf(dispatch_fp, " FROM %s;\n", t_name);

      disin_fp = fopen(DISPATCH_IN, "r");

      // Find the table I'm looking for in DISPATCH.IN
      sprintf(search_name, "");
      i = 1;
      while ( (i != 0) && (fgets(search_name, MAX_DATA_LEN, disin_fp)
	    != NULL) )
      {
	search_name[strlen(search_name)-1] = '\0';
	i = strcmp(search_name, t_name);
      }

      // Find out how many rows there are
      i = 0;
      while( (fgets(data_in, MAX_DATA_LEN, disin_fp) != NULL) &&
	     (data_in[0] != '#') )
	++i;

      num_of_rows = i / num_of_cols;

      // Put tablename(cols)(rows) in EXPORT.OUT
      fprintf(export_fp, "%s(%i)(%i)\n", t_name, num_of_cols,
	num_of_rows);

      if (num_of_rows > 0)
      {
	// Reset disin_fp to beginning of file
	disin_fp = fopen(DISPATCH_IN, "r");

	// Find the table I'm looking for in DISPATCH.IN
	sprintf(search_name, "");
	i = 1;
	while ( (i != 0) && (fgets(search_name, MAX_DATA_LEN, disin_fp)
		!= NULL) )
	{
	  search_name[strlen(search_name)-1] = '\0';
	  i = strcmp(search_name, t_name);
	}

	// Read the rows
	i = 0;
	while( (fgets(data_in, MAX_DATA_LEN, disin_fp) != NULL) &&
		(data_in[0] != '#') )
	{
	  fprintf(export_fp, "%s", col[i]);

	  if (colsize[i] > 64)
	    fprintf(export_fp, "(%i)", colsize[i]);

	  data_in[strlen(data_in)-1] = '\0';

	  // Should leave nothing in between quotes when NULL field
	  if (strcmp(data_in, "<NULL>") == 0)
	    strcpy(data_in, "");

	  fprintf(export_fp, " \"%s\"\n", data_in);

	  ++i;
	  if (i == num_of_cols)
	    i = 0;
	}
      }
    }
    else
    {
      fprintf(dispatch_fp, "<TABLE NOT FOUND>\n");
    }

    fclose(desc_fp);
  }

  fclose(table_fp);
  fclose(dispatch_fp);
  fclose(export_fp);

  return 0;
}