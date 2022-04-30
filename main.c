#include "asm.h"

/**
 * @brief This function opens the file
 * @param name - file name, end - the ending of the file, mode *- the mode for fopen
 * @return 1 if the line has no errors and 0 if an error was   * found
 */
FILE *openfile(char *name,char *end,const char *mode)
{
	FILE *file;
	char *mallocstring = (char *)malloc(strlen(name) + strlen(end)+1),*filename = mallocstring;
	if (!mallocstring)
    {
	    exit(1);
    }
	sprintf(filename,"%s%s",name,end);
	/*create the full file name on one string*/
	file = fopen(filename,mode);
	free(mallocstring);
	return file;
}

/**
 * @brief This function reads the AS file
 * @param filename - the file's name
 * @return void function
 */
void readasfile(char *filename)
{
	FILE *file = openfile(filename,".as", READ);
	int /*memarr[MAXDATA]={0}*/IC = 100,DC = 0;
	/*open the file*/
	if(file == NULL)
	{
		printf("Eror cant open the file %s\n",filename);
		return;
	}
	printf("file opend\n");
    firstRead(file, &IC, &DC, filename);
	fclose(file);
	return;
}
/**
 * @brief main function - handel all the program
 * @param argc - number of inputs for the program, argv - the inputs to the program
 * @return void function
 */
int main(int argc , char* argv[])
{
	int i = 1;
	if(argc < 2)
	{
		printf("Eror no file to read\n");
		return 0;
	}
	for(i = 1; i < argc; i++)
	{
		readasfile(argv[i]);
	}
	return 0;
}
