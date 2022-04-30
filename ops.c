#include "asm.h"

/**
 * @brief this function deals with the commands that use the 1 or 2 op sort receive a string and
 * check if its legal and put its data in the line arr and in the code arr
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * commandLine - the command line, firstOp - indicates if the operand is the first or the second,
 * IC - indicates on the next line number, lineCut - holds the line to check
 * @return NULL - if the line is invalid, string for the next check
 */
char *op12(char *lineCut, int *IC, const int *commandLine, int firstOp, Data *data, char *retVal)
{
    char *str = lineCut, *s = lineCut;
    if(*lineCut == IS_DATA)
    {
        printf(NOT_LEGAL_CHAR_MSG, IS_DATA);
        return NULL; /*src - 0 illigal in those cmnds*/
    }
    if(isReg(&s) > NOT_REG)
    {
        printf(MISPLACED_OPERAND_MSG);
        return NULL; /*src - 3 illigal in those cmnds*/
    }
    while(*str != END_OF_ROW && !isspace(*str))
    {
        if(*str == VALID_CHAR)
        {
            printf(TOO_MANY_OP);
            return NULL;
        }
        str++;
    }
    strcpy(retVal, str);
    *str = END_OF_ARR;
    if(*lineCut == IS_RELATIVE)
    {
        lineCut++;
        jumpKind(data, lineCut, commandLine, IC, RELATIVE_REG_SORT, IMMEDIATE_SORT,
                 RELATIVE_JUMP, TRUE);
    }
    else
    {
        jumpKind(data, lineCut, commandLine, IC, DIRECT_SORT, IMMEDIATE_SORT,
                 LABEL_JUMP, TRUE);
    }
    return retVal;
}


/**
 * @brief this function deal with the commands that use the 1 op sort receive a string and check if
 * its legal and put its data in the line arr and in the code arr
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * commandLine - the command line, firstOp - indicates if the operand is the first or the second,
 * IC - indicates on the next line number, lineCut - holds the line to check
 * @return  return NULL if its illegal sort or the rest of the line if its legal sort
 */
char *op1(char *lineCut, int *IC, int *commandLine, int firstOp, Data *data)
{
    char* p = lineCut, *s = lineCut;
    if(*lineCut == IS_RELATIVE)
    {
        printf(NOT_LEGAL_CHAR_MSG, IS_RELATIVE);
        return NULL; /*src - 2 illigal in those cmnds*/
    }
    /*# as operand------*/
    if(*lineCut == IS_DATA)
    {
        printf(NOT_LEGAL_CHAR_MSG, IS_DATA);
        return NULL; /*src - 0 illigal in those cmnds*/
    }
    if(isReg(&s) > NOT_REG)
    {
        printf(MISPLACED_OPERAND_MSG);
        return NULL; /*src - 3 illigal in those cmnds*/
    }
    while(*lineCut != VALID_CHAR && !isspace(*lineCut ))
    {
        lineCut++;
    }
    *lineCut = END_OF_ARR;
    lineCut++;
    jumpKind(data, p, commandLine, IC, DIRECT_SORT, IMMEDIATE_SORT,
             LABEL_JUMP, FALSE);
    return lineCut;
}


/**
 * @brief this function deal with the commands that use the 1/3op sort receive a string and check if
 * its legal and put its data in the line arr and in the code arr
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * commandLine - the command line, firstOp - indicates if the operand is the first or the second,
 * IC - indicates on the next line number, lineCut - holds the line to check
 * @return  return NULL if its illegal sort, or the rest of the line if its legal sort
 */
char *op13(char *lineCut, int *IC, const int *commandLine, int firstOp, Data *data, char *retVal)
{
    char *hash = lineCut;
    char *str = lineCut;
    int regNum = 0;
    if (*lineCut == END_OF_ARR)
    {
        printf(MISSING_PARAMETERS);
        return NULL;
    }
    if (op13ValCheck(&lineCut) == FAILURE)
    {
        return NULL;
    }
    /* is operand---------------------*/
    if(*lineCut == POSSIBLY_REGISTER)
    {
        regNum = isReg(&lineCut);
        if (regNum != NOT_REG)
        {
            return regToMem(data, hash, regNum, firstOp, commandLine);
        }
    }
    
    if(*lineCut != END_OF_ARR)
    {
    	str = lineCut;
    }
    while(*lineCut != VALID_CHAR && isspace(*lineCut ) && (*lineCut != END_OF_ROW))
    {
        lineCut++;
    }
    while(*str != END_OF_ROW && !isspace(*str))
    {
        if(*str == VALID_CHAR)
        {
            printf(TOO_MANY_OP);
            return NULL;
        }
        str++;
    }
    strcpy(retVal, str);
    *str = END_OF_ARR;
    strcpy((data->line[data->lab].name), lineCut);
    data->line[data->lab].addres = (*IC);
    data->line[data->lab].lineNum = data->lineCounter;
    data->lab++;
    enterToMemo(data, DIRECT_SORT, IMMEDIATE_SORT, commandLine, firstOp);
    (*IC)++;
    str = lineCut;
    str++;
    lineCut = END_OF_ARR_STR;
    lineCut++;
    return retVal;
}


/**
 * @brief this function deal with the commands that use the 0/1/3 op sort recive a string and check if its
 * legal and put its data in the line arr and in the code arr
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * commandLine - the command line, firstOp - indicates if the operand is the first or the second,
 * IC - indicates on the next line number, lineCut - holds the line to check
 * @return  return NULL if its illegal sort or the rest of the line if its legal sort
 */
char *op013(char *lineCut, int *IC, int *commandLine, int firstOp, Data *data, char *retVal)
{
    char *hash = lineCut, *str = lineCut, *p = lineCut;
    int regNum = 0;
    if (op013Validity(&lineCut) == FALSE)
    {
        return NULL;
    }
    
    /*# as operand------*/
    if(*lineCut == IS_DATA)
    {
        return isOpData(data, lineCut, &hash, str, &IC, firstOp, commandLine);

    }
    /* is operand---------------------*/
    str = lineCut;
    if(*lineCut == POSSIBLY_REGISTER)
    {
        regNum = isReg(&lineCut);
        if (regNum != NOT_REG)
        {
            return regToMem(data, hash, regNum, firstOp, commandLine);
        }
    }
    lineCut = str;
    /*label as operand ----------direct*/
    while((*lineCut != VALID_CHAR || isspace(*lineCut)) && firstOp)
    {
        if (*lineCut == END_OF_ROW && firstOp == TRUE)
        {
            printf(MISSING_PARAMETERS);
            return NULL;
        }
        lineCut++;
    }
    p = str;
    while((*p != VALID_CHAR) && (!isspace(*p)) && *p != END_OF_ROW)
    {
        p++;
    }
    if (*p == END_OF_ROW)
    {
        strcpy(retVal, p);
    }
    *p = END_OF_ARR;
    if(firstOp == TRUE)
    {
        jumpKind(data, str, commandLine, IC, DIRECT_SORT, IMMEDIATE_SORT, LABEL_JUMP, FALSE);
    }
    else
    {
        jumpKind(data, str, commandLine, IC, DIRECT_SORT, IMMEDIATE_SORT, LABEL_JUMP, TRUE);
    }
    str = lineCut;
    str++;
    lineCut = END_OF_ARR_STR;
    if (firstOp)
    {
        return str;
    }
    return retVal;
}
