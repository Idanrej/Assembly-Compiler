#include "asm.h"

/**
 * @brief This function receive a pointer to a string and cut all the spaces in it - if there's
 * no spcae, return the same line that was was given
 * @param lineCut - the line to cut
 * @return the next pointer that is not a space or a tab
 */
char *cutWhite(char *lineCut)
{
    while(*lineCut != END_OF_ROW && isspace(*lineCut))
    {
        lineCut++;
    }
    return lineCut;
}

/**
 * @brief This function checks to which letter or number the number fits. if its lower then 10 -
 * it will be the number itself, and else - it will be converted according to the match case.
 * @param hex - the number to check the case for
 * @return the char that fits the input
 */
char checkCase(int hex)
{
    char charToReturn = ' ';
    switch (hex)
    {
        case 10:
            charToReturn = 'a';
            break;
        case 11:
            charToReturn = 'b';
            break;
        case 12:
            charToReturn = 'c';
            break;
        case 13:
            charToReturn = 'd';
            break;
        case 14:
            charToReturn = 'e';
            break;
        case 15:
            charToReturn = 'f';
            break;
        default:
            charToReturn = hex + '0';
            break;
    }
    return charToReturn;
}

/**
 * @brief This function checks if the line that has read in the file is too long and if it does
 * to long - prints a massage
 * @param data - struct that holds all the arrays and parameters related to the arrays, lineCut -
 * the line to check, file - the file that we read, fileName - the file name, errCounter - the
 * error counter
 * @return 0 - if line has no errors and 1 if error was found
 */
int lengthCheck(char *lineCut, FILE *file, int *errCounter, Data *data, char *fileName)
{
    int tempChar;
    if (strchr(lineCut, END_OF_ROW) == NULL && !feof(file))
    {
        printf("line : %d, file: %s, ERROR: line inserted is to long!\n", data->lineCounter, fileName);
        (*errCounter)++;
        data->lineCounter++;
        do
        {
            tempChar = fgetc(file);
        } while (tempChar != EOF && tempChar != END_OF_ROW);
        return FAILURE;
    }
    return SUCCESS;
}
