#include "asm.h"

/**
 * @brief This function opens the output ext file - only if there is an extern in the file.
 * @param symbolCount - symbol counter, data - struct that holds all the arrays and parameters
 * related to the arrays, fileName - the input file name
 * @return void function
 */
void writeEXTFile(Data *data, char *filename, int symbolCount)
{
    int i = 0, a = 0;
  
    char tempName[LINELEN];
    FILE* outputEXT;
    sprintf(tempName, "%s%s", filename, OUTPUT_FILE_END_EXT);
    
    outputEXT = fopen(tempName, "w");
    validityFileCheck(outputEXT, data);
    while(i < symbolCount)
    {
        if(data->symbolArr[i].isexternal == TRUE)
        {
            while(a < data->lab)
            {
                if(!strcmp(data->symbolArr[i].name, data->line[a].name))
                {
                    fprintf(outputEXT, PRINT_TO_FILE_MSG, data->line[a].name, data->line[a].addres);
                }
                a++;
            }
            a = INITIAL_INDICATOR;
        }
        i++;
    }
    fclose(outputEXT);
}
 
/**
 * @brief This function opened the output ob file, converting to binary, and writes to the ob
 * file, named like the input file.
 * @param DC - data counter, symbolCount - symbol counter, IC - line counter, data - struct that
 * holds all the arrays and parameters related to the arrays, fileName - the input file name
 * @return void function
 */
void writeOBFile(Data* data, const int *IC, const int *DC, char* fileName)
{
    int a = 0, f = 0 , t = 0, mask = 15;
    int stack[DIGITS_IN_ROW];
    FILE* outputOB;
    unsigned int pop = *(unsigned int*)&data->binCommand[a].valbits;
    char tempName[LINELEN];
    sprintf(tempName, OUTPUT_FILE_NAME, fileName, OUTPUT_FILE_END_OB);
    outputOB = fopen(tempName, WRITE);
    validityFileCheck(outputOB, data);
    
    fprintf(outputOB, WRITE_FIRST_ROW, (*IC) - IC_STARTING_POINT, (*DC));
    while(a < (*IC) - IC_STARTING_POINT)
    {
        t = INITIAL_INDICATOR;
        pop = *(unsigned int*)&data->binCommand[a].valbits;
        f = (data->binCommand[a].line);
        fprintf(outputOB, PRINT_ROW_IN_FILE, f);
        while(t < 6)
        {
            stack[t] = (pop) & mask;
            pop = pop >> 4;
            t++;
        }
        t--;
        while(t >= 0)
        {
            fprintf(outputOB, PRINT_SINGLE_CHAR, checkCase(stack[t]));
            t--;
        }
        fprintf(outputOB, END_OF_ROW_STR);
        a++;
    }
    a = INITIAL_INDICATOR;
    while(a < (*DC))
    {
        t = INITIAL_INDICATOR;
        pop = *(unsigned int*)&data->binData[a].valbits;
        fprintf(outputOB, PRINT_ROW_IN_FILE, (data->binData[a].line + (*IC)));
        while(t < 6)
        {
            stack[t] = (pop) & mask;
            pop = pop >> 4;
            t++;
        }
        t--;
        while(t >= 0)
        {
            fprintf(outputOB, PRINT_SINGLE_CHAR, checkCase(stack[t]));
            t--;
        }
        fprintf(outputOB, END_OF_ROW_STR);
        a++;
    }
    fclose(outputOB);
}

/**
 * @brief This function opens the output ent file - only if there is an entry in the file.
 * @param symbolCount - symbol counter, data - struct that holds all the arrays and parameters
 * related to the arrays, fileName - the input file name
 * @return void function
 */
void writeENTFile(Data *data, char *filename, int symbolCount)
{
    int i = 0;
    char tempName[LINELEN];
    FILE* outputENT;
    sprintf(tempName, "%s%s", filename, OUTPUT_FILE_END_ENT);
    outputENT = fopen(tempName, WRITE);
    validityFileCheck(outputENT, data);
    while (i < symbolCount)
    {
        if (data->symbolArr[i].isentry == TRUE)
        {
            fprintf(outputENT, PRINT_TO_FILE_MSG, data->symbolArr[i].name, data->symbolArr[i].address);
        }
        i++;
    }
    fclose(outputENT);
}
