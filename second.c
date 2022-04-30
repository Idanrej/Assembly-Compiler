#include "asm.h"

/**
 * @brief This function gets a line that start with a dot and check if the line gotten is entry
 * @param lineCut - the line to check
 * @return if it is an entry - return the rest of the line, else return NULL
 */
char *isItEntry(char *lineCut)
{
	char *space = lineCut, *holder = lineCut;
	space++;
	while(isalpha(*space) || isdigit(*space))
	{
		space++;
	}
	*space = END_OF_ARR;
	if(strcmp(lineCut, ENTRY) == EQUAL)
	{
		space++;
		holder = space;
		while (*holder != END_OF_ROW)
        {
		    holder++;
        }
		*holder = END_OF_ARR;
		return space;
	}	
	return EMPTY_STR;
}

/**
 * @brief This function reads the file for the second time, complete the label locations, and
 * deals with the extern and label errors
 * @param symbolArr - the symbol counter, data - struct that holds all the arrays and parameters
 * related to the arrays, file - the input file
 * @return number of errors accord in the second read
 */
int secondRead(FILE *file, Data *data, int symbolArr)
{
	int i = 0, p = 0, lineCount = 1, equalFlag, errorCount = 0;
    char lineCut[MAXLINE + INCREASING_NUM];
    while(fgets(lineCut, MAXLINE + INCREASING_NUM, file) != NULL)
    {
        equalFlag = FALSE;
        lineCount++;
        strcpy(lineCut, cutWhite(lineCut));
        if(*lineCut == DOT_SIGN)
        {
            strcpy(lineCut, isItEntry(lineCut));
            if(*lineCut)
            {
                while(p < symbolArr)
                {
                    if(strcmp(lineCut, data->symbolArr[p].name) == EQUAL)
                    {
                        data->symbolArr[p].isentry = TRUE;
                        equalFlag = TRUE;
                    }
                    p++;
                }
                if (equalFlag == FALSE)
                {
                    printf(NOT_FOUND_ERROR, lineCount, lineCut);
                    errorCount++;
                }
                p = INITIAL_INDICATOR;
            }
        }
    }
    p = INITIAL_INDICATOR;
    while (i < data->lab)
	{
		while(p < symbolArr)
		{
			if(strcmp(data->line[i].name, data->symbolArr[p].name) == EQUAL)
			{
				if(data->line[i].kind == LABEL_JUMP)
				{
					data->binCommand[data->line[i].addres - IC_STARTING_POINT].valbits.imdbit
					.imidate = data->symbolArr[p].address;
                    data->binCommand[data->line[i].addres - IC_STARTING_POINT].line = data->line[i]
                            .addres;
					if(data->symbolArr[p].isexternal == TRUE)
					{
                        data->binCommand[data->line[i].addres - IC_STARTING_POINT].valbits.imdbit
                        .areim = 1;
					}
					else
					{
                        data->binCommand[data->line[i].addres - IC_STARTING_POINT].valbits.imdbit.areim = 2;
					}
				}
				else
				{
                    data->binCommand[data->line[i].addres - IC_STARTING_POINT].valbits.relatbit.relative =
                            data->symbolArr[p].address - (data->line[i].addres - 1);
                    data->binCommand[data->line[i].addres - IC_STARTING_POINT].valbits.relatbit
                    .arerel = 4;
                    data->binCommand[data->line[i].addres - IC_STARTING_POINT].line = data->line[i].addres;
				}
				break;
			}
			p++;
		}	
		if(p == symbolArr)
		{
			printf(CMD_NOT_FOUND, data->line[i].lineNum, data->line[i].name);
			errorCount++;
		}
		p = INITIAL_INDICATOR;
		i++;
	}
	return errorCount;
}
