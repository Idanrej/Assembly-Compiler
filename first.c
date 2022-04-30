#include "asm.h"

const regs regArr[] =
        {
                {"r0", 0},
                {"r1", 1},
                {"r2", 2},
                {"r3", 3},
                {"r4", 4},
                {"r5", 5},
                {"r6", 6},
                {"r7", 7},
                {NULL}
        };
const cmd cmdArr[] =
        {  /*NAME, FUNC , OPCODE , SRC,dest*/
                {"mov", 0, 0, op013, op13},
                {"cmp", 0, 1, op013, op013},
                {"add", 1, 2, op013, op13},
                {"sub", 2, 2, op013, op13},
                {"lea", 0, 4, op1, op13},
                {"clr", 1, 5, op13},
                {"not", 2, 5, op13},
                {"inc", 3, 5, op13},
                {"dec", 4, 5, op13},
                {"jmp", 1, 9, op12},
                {"bne", 2, 9, op12},
                {"jsr", 3, 9, op12},
                {"red", 0, 12, op13},
                {"prn", 0, 13, op013},
                {"rts", 0, 14},
                {"stop", 0, 15},
                {NULL}
        };

const data dataArr[] =
        {
                {"string", stringCut},
                {"data", dataCut},
                {"extern", externCut},
                {"entry", entryCut},
                {NULL}
        };



int lengthCheck(char *lineCut, FILE *file, int *errCounter, Data *data, char *fileName);

/**
 * @brief this function checks if the line that it gets is a legit register
 * @param lineCut - holds a pointer to the line we want to check
 * @return return the reg number if it is register, and -1 if not register
 */
int isReg(char **lineCut)
{
    char *hash = *lineCut;
    int i = 0;
    if(**lineCut == POSSIBLY_REGISTER)
    {
        (*lineCut)++;
        if(isdigit(**lineCut))
        {
            hash = *lineCut;
            (*lineCut)++;
            if(isspace(**lineCut) || **lineCut == VALID_CHAR)
            {
                *lineCut = END_OF_ARR_STR;
                i = atoi(hash);
                if(i >= 0 && i < 8)
                {
                    return i;
                }
            }
        }
    }
    return NOT_REG;
}

/**
 * @brief This function updates all the arrays that related to the labels jumps
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * commandLine - the command line, firstSortKind - the first input to put in the array,
 * secondSortKind - the second input to put in the array, kind - the kind to put in the array,
 * flag - if its the first operand or not, IC - indicates on the next line number, lineCut - holds
 * the line to check
 * @return the line cut - the string for the next check
 */
char *jumpKind(Data *data, char *lineCut, const int *commandLine, int *IC, int firstSortKind,
               int secondSortKind, int kind, int flag)
{
    strcpy((data->line[data->lab].name), lineCut);
    data->line[data->lab].addres = (*IC);
    data->line[data->lab].kind = kind;
    data->line[data->lab].lineNum = data->lineCounter;
    data->lab++;
    enterToMemo(data, firstSortKind, secondSortKind, commandLine, !flag);
    (*IC)++;
    return lineCut;
}



/**
 * @brief this function check if its a valid value for 0/1/3 sorting
 * @param lineCut - holds a pointer for the the line to check
 * @return 1 - if legal, 0 - if illegal
 */
int op013Validity(char **lineCut)
{
    *lineCut = cutWhite(*lineCut);
    if (**lineCut == END_OF_ARR)
    {
        printf(MISSING_PARAMETERS);
        return FALSE;
    }
    if(**lineCut == IS_RELATIVE)
    {
        printf(NOT_LEGAL_CHAR_MSG, IS_RELATIVE);
        return FALSE; /*src - 2 illigal in those cmnds*/
    }
    return TRUE;
}

/**
 * @brief This function updates all the data in the array, in the correct line.
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * commandLine - the command line, firstOp - indicates if the operand is the first or the second,
 * IC - indicates on the next line number, lineCut - holds the line to check, hash - char array
 * that need to be updated fr the next checks, str - char array that need to be updated fr the next
 * checks
 * @return return NULL if its illegal sort or the rest of the line if its legal sort
 */
char * isOpData(Data *data, char *lineCut, char **hash, char *str, int **IC, int firstOp, int
                *commandLine)
{
    *hash = lineCut;
    (*hash)++;
    lineCut = *hash;
    if ( **hash == MINUS_CHAR || **hash == PLUS_CHAR)
    {
        (*hash)++;
    }
    while(isdigit(**hash))
    {
        (*hash)++;
    }
    if (**hash != SPACE && **hash != VALID_CHAR && **hash != END_OF_ARR && **hash != END_OF_ROW)
    {
        printf(INVALID_DATA_MSG);
        return NULL;
    }
    str = *hash;
    str++;
    **hash = END_OF_ARR;
    (*hash)++;
    data->binCommand[*(*IC) - IC_STARTING_POINT].valbits.imdbit.imidate = atoi(lineCut);
    data->binCommand[*(*IC) - IC_STARTING_POINT].valbits.imdbit.areim = ABSOLUTE; /*imdate 100*/
    data->binCommand[*(*IC) - IC_STARTING_POINT].line = *(*IC);
    enterToMemo(data, IMMEDIATE_SORT, IMMEDIATE_SORT, commandLine, firstOp);
    (**IC)++;
    return str;
}

/**
 * @brief This function receives the details to enter the code memory and put it in the
 * right bits and realloc more memory if needed for the line arr
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * commandLine - the command line, firstOp - indicates if the operand is the first or the second,
 * firstSortKind - the first input to put in the array, secondSortKind - the second input to put
 * in the array
 * @return void function
 */
void enterToMemo(Data *data, int firstSortType, int secondSortType, const int *commandLine, int firstOp)
{
    if(firstOp == TRUE)
    {
        data->binCommand[(*commandLine)].valbits.codebit.opsrcsort = firstSortType;
        data->binCommand[(*commandLine)].valbits.codebit.opsrc = secondSortType;
    }
    else
    {
        data->binCommand[(*commandLine)].valbits.codebit.opdstsort = firstSortType;
        data->binCommand[(*commandLine)].valbits.codebit.opdst = secondSortType;
    }
    if (data->lab == data->labMemoC)
    {
        data->labMemoC += MEMO_ALOC;
        data->line = (lineinfo*) realloc(data->line, sizeof(lineinfo) * data->labMemoC);
    }
}


/**
 * @brief this function check if its a valid value for 1/3 sorting
 * @param lineCut - holds a pointer for the the line to check
 * @return 1 - if legal, 0 - if illegal
 */
int op13ValCheck(char **lineCut)
{
    while(**lineCut == VALID_CHAR || isspace(**lineCut))
    {
        (*lineCut)++;
    }
    if(**lineCut == IS_DATA)
    {
        printf(INVALID_SECOND_OP_MSG, IS_DATA);
        return FAILURE;
    }
    if(**lineCut == IS_RELATIVE)
    {
        printf(INVALID_SECOND_OP_MSG, IS_RELATIVE);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief This function enter to the memory - to the array, if the command is register
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * commandLine - the command line, firstOp - indicates if the operand is the first or the second,
 * hash - char array that holds the command we want to enter, regNum - the register number
 * @return  return NULL if its illegal sort, or the rest of the line if its legal sort
 */
char *regToMem(Data *data, char *hash, int regNum, int firstOp, const int *commandLine)
{
    enterToMemo(data, DIRECT_REG_SORT, regNum, commandLine, firstOp);
    while(*hash != VALID_CHAR && *hash != END_OF_ROW && firstOp == TRUE)
    {
        hash++;
    }
    hash++;
    while (firstOp == FALSE && *hash != END_OF_ROW && *hash != END_OF_ARR && *hash != SPACE)
    {
        hash++;
    }
    return hash;
}

/**
 * @brief This function will skip entry declare on the first read
 * @return  returns 1 - valid run
 */
int entryCut()
{
    return 1;
}

/**
 * @brief This func recive part after the .extern it cuts the name of the extern allocates more
 * place in the symbol array and add the extern
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * lineCut - holds the line to check, symbolCount - counts the number of symbol
 * @return  returns 1 - if valid run, 0 - if invalid run
 */
int externCut(char *lineCut, int *symbolCount, Data *data)
{
    int c = (*symbolCount);
    char *p = lineCut;
    while(*p != END_OF_ROW && !isspace(*p))
    {
        p++;
    }
    p = cutWhite(p);
    if (*p != END_OF_ROW)
    {
        printf(EXTRA_CHARS_ERROR);
        return 0;
    }
    *p = END_OF_ARR;
    (*symbolCount)++;
    data->inCounter++;
    if(data->inCounter == (data->memoryCount - INITIAL_MEMO_DATA))
    {
        data->memoryCount += MEMO_ALOC;
        data->symbolArr = (labelinfo*)realloc(data->symbolArr, sizeof(labelinfo) *
                                                               (data->memoryCount));
        if (!(data->symbolArr))
        {
            printf(BAD_MEMO_ALLOC);
            exit(FAILURE);
        }
    }
    data->symbolArr[c].address = 0;
    data->symbolArr[c].isexternal = TRUE;
    strcpy(data->symbolArr[c].name, lineCut);
    return 1;
}

/**
 * @brief This functin receive part after the string part of the data ("abcd") the DC to count
 * the lines. and put the data in the DATA arr using realloc and add the data and its line
 * location to the array
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * DC - data counter, lineCut - holds the line to check
 * @return return 0 if its illegal command or 1 legal command
 */
int stringCut(char *lineCut, int *DC, Data *data)
{
    int i = 0;
    int c = *DC;
    if (*lineCut != INVERTED_COMMAS_CHAR || lineCut[strlen(lineCut) - INCREASING_NUM] != INVERTED_COMMAS_CHAR)
    {
        printf(INVERTED_COMMAS_MISSING);
        return 0;
    }
    
    lineCut++;
    while(i < strlen(lineCut) - 1)
    {
        i++;
    }
    data->binData = (bincmd*)realloc(data->binData, sizeof(bincmd) * (i + (*DC)) );
    if(data->binData == NULL)
    {
        printf(BAD_MEMO_ALLOC);
        exit(FAILURE);
    }
    i = INITIAL_INDICATOR;
    while(i < strlen(lineCut) - INCREASING_NUM)
    {
        dataToMemo(data, DC, &(lineCut[i]), &i, &c, NOT_DATA);
    }
    dataToMemo(data, DC, END_OF_ARR_STR, &i, &c, NOT_DATA);
    return 1;
}

/**
 * @brief This function enter to the memory - to the array, if the command is data
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * lineArg - the value to put in the line param, dataArg - the value to put in the data param,
 * memoCounter - counts the memory needed, DC - data count, kind - if the command is data or not
 * @return void function
 */
void dataToMemo(Data *data, int *lineArg, char *dataArg, int *memoCounter, int *DC, int kind)
{
    if (kind == DATA)
    {
        data->binData[*DC].valbits.databit.data = atoi(dataArg);

    }
    else
    {
        data->binData[*DC].valbits.databit.data = *dataArg;
    }
    data->binData[*DC].line = (*lineArg);
    (*memoCounter)++;
    (*DC)++;
    (*lineArg)++;
}

/**
 * @brief This function recive a string contain data the func allocate new space in the storing
 * array
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * lineCut - the line to check, DC - data count
 * @return 0 - if invalid, 1 - if valid
 */
int dataCut(char *lineCut, int *DC, Data *data)
{
    char *comaPlace = strchr(lineCut, VALID_CHAR);
    char *str = lineCut, *num = str;
    int memoCount = 0, c = *DC, digitFlag = FALSE;
    if ((!isdigit(*lineCut) && *lineCut != PLUS_CHAR && *lineCut != MINUS_CHAR) ||
        !isdigit(lineCut[strlen(lineCut) - INCREASING_NUM]))
    {
        printf(WRONG_COMA_AMOUNT);
        return 0;
    }
    
    if (memoCounter(&comaPlace, &memoCount) == FAILURE)
    {
        return 0;
    }
    memoCount++; /*for the first num always data = ',' amount + 1*/
    data->binData = (bincmd*)realloc(data->binData, sizeof(bincmd) * (memoCount + (*DC)) );
    if(data->binData == NULL)
    {
        printf(BAD_MEMO_ALLOC);
        exit(FAILURE);
    }
    memoCount = INITIAL_INDICATOR;
    while(*str != END_OF_ARR && *str != END_OF_ROW)
    {
        if(isspace(*str))
        {
            str = cutWhite(str);
            if (digitFlag == TRUE && *str != VALID_CHAR && *str != END_OF_ARR && *str != END_OF_ROW)
            {
                printf(WRONG_COMA_AMOUNT);
                return 0;
            }
        }
        if(*str != VALID_CHAR)
        {
            if (*str == DOT_SIGN)
            {
                printf(CANT_HAVE_DOT);
                return 0;
            }
            if(*str == PLUS_CHAR || *str == MINUS_CHAR || isdigit(*str))
            {
                if (isdigit(*str))
                {
                    digitFlag = TRUE;
                }
                else
                {
                    digitFlag = FALSE;
                }
                str++;
                memoCount++;

            }
        }
        else
        {
            digitFlag = FALSE;
            if(*str == SPACE)
            {
                str++;
            }
            else
            {
                *str = END_OF_ARR;
                str++;
                dataToMemo(data, DC, num, &memoCount, &c, DATA);
                num = str;
            }
        }
    }
    dataToMemo(data, DC, num, &memoCount, &c, DATA);
    return 1;
}

/**
 * @brief This function counts the amount f data to allocate
 * @param line - the line to check, memoCounter - the memory counter
 * @return 1 - if invalid, 0 - if valid
 */
int memoCounter(char **line, int *memoCounter)
{
    while(*line != NULL) /*count the amount of data to allocate*/
    {
        (*memoCounter)++;
        (*line)++;
        *line = cutWhite(*line);
        if (!isdigit(**line) && (**line != PLUS_CHAR) && (**line != MINUS_CHAR))
        {
            printf(WRONG_COMA_AMOUNT);
            return FAILURE;
        }
        *line = strchr(*line, VALID_CHAR);
    }
    return SUCCESS;
}

/**
 * @brief This func check for an empty line or a ; line if its a line to ignore
 * @param lineCut - the line to check
 * @return 1 - if its a line to ignore, 0 - else
 */
int isWhite(char *lineCut)
{

    if(*lineCut == COMMENT_SIGN)
    {
        return 1; /*line is comment*/
    }
    while(*lineCut && isspace(*lineCut) && *lineCut != END_OF_ARR)
    {
        lineCut++;
        if(*lineCut == COMMENT_SIGN)
        {
            return 1; /*line is comment*/
        }
    }
    if(*lineCut == END_OF_ARR)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief This func will look for a label. if the label is legal return 1 to the symbol flag
 * add label name to the symbol list
 * @param lineCut - the line to check, symbolFlag - indicates if the row is symbol,
 * symbolCount - counts the symbols, data - struct that holds all the arrays and parameters related
 * to the arrays
 * @return if label - the rest of the string after the label, if not label - return the string that
 * was send to the function
 */
char *isSymbol(char *lineCut, int *symbolFlag, const int *symbolCount, Data* data)
{
    char *label = strchr(lineCut, ':');
    char *space = strchr(lineCut, ' ');
    char *tab = strchr(lineCut, '	');
    char *ret = label;
    ret++;
    /*if string has ':' in it*/
    if(!label)
    {
        return lineCut;
    }
    else
    {
        if(space || tab)
        {
            if(((space < label) && space ) || ((tab < label) && tab))
            {
                return lineCut; /*return the string back - its not a label*/
            }
        }
        *label = END_OF_ARR;
        if (labelCheck(lineCut) == FALSE)
        {
            return NULL;
        }
    }
    if (repCheckName(data, lineCut, symbolCount) == FALSE)
    {
        return NULL;
    }
    strcpy(data->symbolArr[(*symbolCount)].name, lineCut);
    (*symbolFlag)++;
    return ret;
}

/**
 * @brief This function checks if the name is already in use
 * @param lineCut - the line to check, symbolCount - counts the symbols, data - struct that holds
 * all the arrays and parameters related to the arrays
 * @return 1 - if valid input, 0 - if invalid input
 */
int repCheckName(Data *data, char *lineCut, const int *symbolCount)
{
    int i = 0;
    while(i < (*symbolCount))
    {
        if(strcmp(data->symbolArr[i].name, lineCut) == EQUAL)
        {
            printf(LABEL_NAME_TAKEN);
            return FALSE;
        }
        i++;
    }
    i = INITIAL_INDICATOR;
    while(!regArr[i].name)
    {
        if(strcmp(regArr[i].name, lineCut) == EQUAL)
        {
            printf(NAME_TAKEN_BY_REG);
            return FALSE;
        }
        i++;
    }
    return TRUE;
}

/**
 * @brief This function checks if the command is label, and if it is label - checks if its a
 * valid label
 * @param lineCut - the line to check
 * @return 1 - if valid input, 0 - if invalid input
 */
int labelCheck(char *lineCut)
{
    int placeInCmd = 0;
    if(strlen(lineCut) > MAXLABEL)
    {
        printf(LONG_LABEL_MSG);
        return FALSE;
    }
    while(cmdArr[placeInCmd].name && (strcmp(lineCut, cmdArr[placeInCmd].name)) != EQUAL)
    {
        placeInCmd++;
    }
    if(cmdArr[placeInCmd].name)
    {
        printf(LABEL_NAME_MSG, cmdArr[placeInCmd].name);
        return FALSE;
    }
    placeInCmd = INITIAL_INDICATOR;
    while(lineCut[placeInCmd] != END_OF_ARR)
    {
        if(!(isdigit(lineCut[placeInCmd])) && !(isalpha(lineCut[placeInCmd])))
        {
            printf(ILLEGAL_CHAR_MSG, lineCut[placeInCmd]);
            return FALSE;
        }
        placeInCmd++;
    }
    placeInCmd = INITIAL_INDICATOR;
    while(dataArr[placeInCmd].name && (strcmp(lineCut, dataArr[placeInCmd].name)) != EQUAL)
    {
        placeInCmd++;
    }
    if(dataArr[placeInCmd].name)
    {
        printf(NAME_ERROR_MSG, dataArr[placeInCmd].name);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief This function recive a line and check if it is a data line - string/entry/external
 * @param lineCut - the line to check, DC - data counter, symbolCount - symbol counter,
 * symbolFlag - if the command is symbol, data - struct that holds all the arrays and parameters
 * related to the arrays
 * @return 1 - if valid input, 0 - if invalid input
 */
int isData(char *lineCut, int *DC, int *symbolCount, int symbolFlag, Data* data)
{
    int i = 0;
    char *ret = lineCut;
    /*cut the part after the type out */
    if (dataValidityCheck(lineCut, &ret) == FAILURE)
    {
        return 0;
    }
    while(dataArr[i].name && (strcmp(lineCut, dataArr[i].name)) != EQUAL)
    {
        i++;
    }
    if(dataArr[i].name)
    {
        if(strcmp(dataArr[3].name, lineCut) == EQUAL)/*is entry - skip*/
        {
            return 1;
        }
        if(strcmp(dataArr[2].name, lineCut) == EQUAL)/*is extern*/
        {
            ret = cutWhite(ret);
            if (dataArr[i].func(ret, symbolCount, data) == 0)
            {
                return 0;
            }
            return 1;
        }

        if(symbolFlag == TRUE)
        {
            /*give the label the addres of the DC*/
            data->symbolArr[*(symbolCount)].address = (*DC);
            data->symbolArr[*(symbolCount)].isdata = TRUE;
        }
        ret = cutWhite(ret);
        if (dataArr[i].func(ret, DC, data) == 0)
        {
            return 0;
        }
        return 1;
    }
    printf(ILLEGAL_DOT_MSG);
    return 0;
}

/**
 * @brief This function checks validity for a given line
 * @param lineCut - the line to check, ret - a pointer to the row to check that is also updated
 * in this function
 * @return 0 - if valid input, 1 - if invalid input
 */
int dataValidityCheck(char *lineCut, char **ret)
{
    char *dat = strchr(lineCut, SPACE);
    if (dat == NULL)
    {
        dat = strchr(lineCut,'	');
    }
    if (dat == NULL)
    {
        printf(NAME_MISSING_MSG, *ret);
        return FAILURE;
    }
    *ret = dat;
    (*ret)++;
    *dat = END_OF_ARR;
    return SUCCESS;
}

/**
 * @brief This function get a line that start with a letter the function will cut out every part
 * of the command will check if its legal and update the array of bynari num with the command
 * detail.
 * @param lineCut - the line to check, IC - line counter, symbolCount - symbol counter,
 * symbolFlag - if the command is symbol, data - struct that holds all the arrays and parameters
 * related to the arrays
 * @return 1 if the line has no errors and 0 if an error was found
 */
int isCommand(char *lineCut, int *IC, int symbolFlag, const int *symbolCount, Data *data)
{
    
    char *cmnd = lineCut, *coma = cmnd, retVal[MAXLINE];
    int firstOp = FALSE, i = 0;
    int cmdline = (*IC) - IC_STARTING_POINT;
    checkMemo(data);
    if (commandValidity(cmnd, &coma, &i) == FAILURE)
    {
        return 0;
    }
    updateSymbolAndCommand(data, IC, symbolFlag, symbolCount, cmdline, i);
    if(cmdArr[i].opcode < TWO_OPERAND_LIM)
    {
        firstOp = TRUE;
        coma = cmdArr[i].op1(coma, IC, &cmdline, firstOp, data, retVal);
        if(coma != NULL)
        {
            firstOp = INITIAL_INDICATOR;
            coma = cmdArr[i].op2(coma, IC, &cmdline, firstOp, data, retVal);
            if (coma == NULL)
            {
                return 0;
            }
            else if (checkRet(coma) == FAILURE)
            {
                return 0;
            }
            return 1;
        }
    }
    else
    {
        if(cmdArr[i].opcode < ONE_OPERAND_LIM)
        {
            if (*coma == END_OF_ARR)
            {
                printf(MISSING_PARAMETERS_MSG);
                return 0;
            }
            coma = cmdArr[i].op1(coma, IC, &cmdline, firstOp, data, retVal);
            if (coma == NULL)
            {
                return 0;
            }
            else if (checkRet(coma) == FAILURE)
            {
                return 0;
            }
            return 1;
        }
        else
        {
            coma = cutWhite(coma);
            if (*coma != END_OF_ARR)
            {
                printf(NO_OPPERAND_COMMEND_MSG);
                return 0;
            }
            return 1;
        }
    }
    return 0;
}
/**
 * @brief This function checks if the return value from the function is valid or not
 * @param coma - the return value to check
 * @return  0 - if valid, 1 - if not valid
 */
int checkRet(char *coma)
{
    coma = cutWhite(coma);
    if (*coma == END_OF_ARR || *coma == END_OF_ROW)
    {
        return SUCCESS;
    }
    printf(EXTRA_CHARS_ERROR);
    return FAILURE;
}

/**
 * @brief This function checks if the bincomand array needed more dynamic memory allocation, and
 * if needed - allocates more memory to the array
 * @param data - struct that holds all the arrays and parameters related to the arrays
 * @return void function
 */
void checkMemo(Data *data)
{
    if (data->commandArrC <= (data->commandArrM - MEMO_COMMAND))
    {
        data->commandArrM += MEMO_COMMAND;
        data->binCommand = (bincmd*) realloc (data->binCommand, sizeof(bincmd) * data->commandArrM);
        if(data->binCommand == NULL)
        {
            freeArrays(data);
            exit(FAILURE);
        }
    }
}

/**
 * @brief this function check if the command that given is valid
 * @param command - the command to check, i - the place in the cmdArr for the command, coma - a
 * pointer to the char array that the coma is in
 * @return 0 - if legal, 1 - if illegal
 */
int commandValidity(char *command, char **coma, int *i)
{
    while(isalpha(**coma))
    {
        (*coma)++;
    }
    **coma = END_OF_ARR;
    (*coma)++;
    while((cmdArr[*i].name) && ((strcmp(command, cmdArr[*i].name)) != EQUAL))
    {
        (*i)++;
    }
    if(!cmdArr[*i].name)
    {
        printf(COMMAND_NOT_EXISTS, command);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief This function updates the symbols and commands arrays - the symbolArr and the binCommand.
 * @param symbolCount - symbol counter, symbolFlag - if the command is symbol, data - struct that
 * holds all the arrays and parameters related to the arrays, cmdline - the place for the
 * bimncomnd array
 * @return void function
 */
void updateSymbolAndCommand(Data *data, int *IC, int symbolFlag, const int *symbolCount, int cmdline,
                            int placeInCmdarr)
{
    if(symbolFlag == TRUE) /*update symbol arr*/
    {
        data->symbolArr[*(symbolCount)].address = (*IC); /*update symbo adres*/
        data->symbolArr[*(symbolCount)].iscode = TRUE; /*symol is a code*/
    }
    /*UPDATE THE COMANDS AS BITS IN THE COMAND ARR*/
    data->binCommand[cmdline].valbits.databit.data = 0;
    data->binCommand[cmdline].line = (*IC);
    data->binCommand[cmdline].valbits.codebit.opcode = (cmdArr[placeInCmdarr].opcode);
    data->binCommand[cmdline].valbits.codebit.funct = (cmdArr[placeInCmdarr].funct);
    data->binCommand[cmdline].valbits.codebit.are = ABSOLUTE;
    (*IC)++;
}

/**
 * @brief This function receive a line (not empty) and check the line for its components. also, in
 * this function will update the symbol arr and realloc more room for it
 * @param lineCut - the line to check, DC - data counter, symbolCount - symbol counter,
 * IC - line counter, data - struct that holds all the arrays and parameters
 * related to the arrays
 * @return 0 - if line has no errors and 1 if error was found
 */
int readLine(char *lineCut, int *IC, int *DC, int *symbolCount, Data* data)
{
    int symbolFlag = 0;
    lineCut = cutWhite(lineCut);
    if((isdigit(*lineCut)) || (isalpha(*lineCut)))
    {
        lineCut = isSymbol(lineCut, &symbolFlag, symbolCount, data);
        if(lineCut == NULL)
        {
            return 1;
        }
        if(symbolFlag == TRUE) /*if = 1 -we have a label*/
        {
            lineCut = cutWhite(lineCut);
            if(*lineCut == DOT_SIGN)/*is it a data*/
            {
                if(isData(++lineCut, DC, symbolCount, symbolFlag, data) == 0)
                {
                    return 1;
                    /* data line is good*/
                }
            }
            else /*check for a command after the label*/
            {
                if(!isalpha(*lineCut) || !isCommand(lineCut, IC, symbolFlag, symbolCount, data))
                {
                    return 1;
                }
            }
            (*symbolCount)++;
            data->inCounter++;
            if(data->inCounter == (data->memoryCount - 1))
            {
                data->memoryCount += MEMO_ALOC;
                data->symbolArr = (labelinfo*)realloc(data->symbolArr, sizeof(labelinfo) *
                                                                       (data->memoryCount));
                if (data->symbolArr == NULL)
                {
                    freeArrays(data);
                    exit(FAILURE);
                }
            }
        }
        else/*if not a label its a comand*/
        {
            if(isCommand(lineCut, IC, symbolFlag, symbolCount, data))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    else if (isData(++lineCut, DC, symbolCount, symbolFlag, data) == 0) /*if first latter in line
 * is not a number or letter*/
    {
        return 1;
    }
    return 0;
}

/**
 * @brief This function read the lines of the program check if its a line and send the line to be
 * read
 * @param DC - data counter, IC - line counter,  fileName - the input file name, file - the input
 * file entered by the user
 * @return 1 - if the run was successful, 0 - if an error accord
 */
int firstRead(FILE *file, int *IC, int *DC, char *fileName)
{
    int symbolCount = 0, t, errCount = 0;
    char lineCut[MAXLINE + INCREASING_NUM];
    Data data;
    data.lab = INITIAL_INDICATOR, data.inCounter = 1, data.memoryCount = 5, data.commandArrC = 0;
    data.commandArrM = 6, data.labMemoC = MEMO_ALOC;
    data.binCommand = NULL, data.binData = NULL, data.symbolArr = NULL, data.line = NULL;
    initialMemo(&data);
    data.lineCounter = 1;
    while(fgets(lineCut, MAXLINE + INCREASING_NUM, file) != NULL)
    {
        if (lengthCheck(lineCut, file, &errCount, &data, fileName) == FAILURE)
        {
            continue;
        }
        if(isWhite(lineCut) == GOOD_LINE)
        {
            t = readLine(lineCut, IC, DC, &symbolCount, &data);
            if (t == FAILURE)
            {
                printf(FILE_LINE_MSG, fileName, data.lineCounter);
            }
            errCount = errCount + t;
        }
        data.lineCounter++;
    }
    if (errCount > EMPTY)
    {
        safeExit(&data, file);
        return FAILURE;
    }
    rewind(file);
    addIcToDc(data, symbolCount, IC);
    if (secondRead(file, &data, symbolCount) > EMPTY)
    {
        safeExit(&data, file);
        return FAILURE;
    }
    writeOBFile(&data, IC, DC, fileName);
    if (isEntry(&data, symbolCount) == SUCCESS)
    {
        writeENTFile(&data, fileName, symbolCount);
    }
    if (isExtern(&data, symbolCount) == SUCCESS)
    {
        writeEXTFile(&data, fileName, symbolCount);
    }
    freeArrays(&data);
    return SUCCESS;
}

/**
 * @brief This function add IC to the DC
 * @param symbolCount - symbol counter, data - struct that holds all the arrays and parameters
 * related to the arrays, IC - rows counter
 * @return void function
 */
void addIcToDc(Data data, int symbolCount, const int *IC)
{
    int t = INITIAL_INDICATOR;
    while(t < symbolCount)
    {
        if(data.symbolArr[t].isdata == TRUE)
        {
            data.symbolArr[t].address = data.symbolArr[t].address + (*IC);
        }
        t++;
    }
}

/**
 * @brief This function checks the validity of the file, if the FILE object that the function
 * gets in NULL - there's a problem in the file and the program will call the freeArrays function,
 * that free all the memory and then the program will exit with code 1.
 * @param file - the input file, data - struct that holds all the arrays and parameters
 * related to the arrays
 * @return void function
 */
void validityFileCheck(FILE *file, Data *data)
{
    if (file == NULL)
    {
        freeArrays(data);
        exit(FAILURE);
    }
}

/**
 * @brief this function checks if there is an extern in the program
 * @param  symbolCount - symbol counter, data - struct that holds all the arrays and parameters
 * related to the arrays
 * @return 0 - if found an extern, and else 1
 */
int isExtern(Data *data, int symbolCount)
{
    int i = 0;
    while (i < symbolCount)
    {
        if (data->symbolArr[i].isexternal == TRUE)
        {
            return SUCCESS;
        }
        i++;
    }
    return FAILURE;
}

/**
 * @brief this function checks if there is an ENTRY in the program
 * @param  symbolCount - symbol counter, data - struct that holds all the arrays and parameters
 * related to the arrays
 * @return 0 - if found an entry, else returns 1
 */
int isEntry(Data *data, int symbolCount)
{
    int i = 0;
    while (i < symbolCount)
    {
        if (data->symbolArr[i].isentry == TRUE)
        {
            return SUCCESS;
        }
        i++;
    }
    return FAILURE;
}

/**
 * @brief This function called when the program terminated. closes the input file that the
 * program reads in the same time, and calling freeArrays function, that free all the data arrays.
 * @param data - struct that holds all the arrays and parameters related to the arrays,
 * inputFile - the input file that we currently reading
 * @return void function
 */
void safeExit(Data *data, FILE *inputFile)
{
    
    freeArrays(data);
}

/**
 * @brief This function called when we need to allocates place in memory for all the arrays in the
 * data struct, allocate the arrays dynamically
 * @param data - struct that holds all the arrays and parameters related to the arrays
 * @return void function
 */
void initialMemo(Data *data)
{
    data->binCommand = (bincmd*)calloc(INITIAL_MEMO_CMD, sizeof(bincmd));
    data->binData = (bincmd*)calloc(INITIAL_MEMO_DATA, sizeof(bincmd));
    data->symbolArr = (labelinfo*)calloc(MEMO_ALOC, sizeof(labelinfo));
    data->line = (lineinfo*)calloc(MEMO_ALOC, sizeof(lineinfo));
    if (!(data->binCommand) || !(data->binData) || !(data->symbolArr) || !(data->line))
    {
        printf(BAD_MEMO_ALLOC);
        exit(FAILURE);
    }
}

/**
 * @brief This function called when we need to free all the dynamic memory allocated in the program
 * @param data - struct that holds all the arrays and parameters related to the arrays
 * @return void function
 */
void freeArrays(Data *data)
{
	
    free(data->binData);
    free(data->symbolArr);
    free(data->binCommand);
    free(data->line);
    data->binData = NULL, data->symbolArr = NULL, data->binCommand = NULL, data->line = NULL;
}
