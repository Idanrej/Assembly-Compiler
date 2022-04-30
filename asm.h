#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#define MAXDATA 1000
#define MAXLINE 80
#define MAXLABEL 30
#define LINELEN 100
#define END_OF_ARR '\0'
#define FAILURE 1
#define SUCCESS 0
#define READ "r"
#define WRITE "w"
#define POSSIBLY_REGISTER 'r'
#define IS_DATA '#'
#define DOT_SIGN '.'
#define VALID_CHAR ','
#define IS_RELATIVE '&'
#define END_OF_ROW '\n'
#define COMMENT_SIGN ';'
#define NOT_LEGAL_CHAR_MSG " %c is not legal in this place\n"
#define INVALID_SECOND_OP_MSG "second operand cant be %c\n"
#define MISPLACED_OPERAND_MSG "cant have operand in this place\n"
#define BAD_MEMO_ALLOC "ERROR: bad memory allocation\n"
#define MISSING_PARAMETERS "ERROR: missing parameters\n"
#define OUTPUT_FILE_END_OB ".ob"
#define OUTPUT_FILE_END_ENT ".ent"
#define OUTPUT_FILE_END_EXT ".ext"
#define END_OF_ROW_STR "\n"
#define INCREASING_NUM 2
#define EQUAL 0
#define ONE_OPERAND_LIM 14
#define TWO_OPERAND_LIM 5
#define MISSING_PARAMETERS_MSG "ERROR: missing parameters\n"
#define CANT_HAVE_DOT "ERROR: cant have dot in data\n"
#define NO_OPPERAND_COMMEND_MSG "ERROR: this command has no operands\n"
#define END_OF_ARR_STR "\0"
#define ILLEGAL_DOT_MSG "illegal name .\n"
#define PRINT_TO_FILE_MSG "%s %07d\n"
#define SUFFIX_LENGTH 4
#define INITIAL_INDICATOR 0
#define GOOD_LINE 0
#define MEMO_ALOC 5
#define TRUE 1
#define FALSE 0
#define DIGITS_IN_ROW 6
#define NOT_REG -1
#define DIRECT_REG_SORT 3
#define RELATIVE_REG_SORT 2
#define DIRECT_SORT 1
#define IMMEDIATE_SORT 0
#define RELATIVE_JUMP 1
#define LABEL_JUMP 0
#define IC_STARTING_POINT 100
#define LONG_LABEL_MSG "ERROR : label is too long\n"
#define CMD_NOT_FOUND "line: %d, %s was not found\n"
#define LABEL_NAME_MSG "ERROR: label can't be called like command - %s\n"
#define ILLEGAL_CHAR_MSG "ERROR: %c can't be in a label name\n"
#define NAME_ERROR_MSG "ERROR: '%s' can't be a label name\n"
#define LABEL_NAME_TAKEN "ERROR: can't call two labels in the same name\n"
#define NAME_TAKEN_BY_REG "ERROR: label can't be called in register name\n"
#define COMMAND_NOT_EXISTS "ERROR: command %s doesnt exist\n"
#define WRONG_COMA_AMOUNT "ERROR: too many or wrong place for , \n"
#define INVALID_DATA_MSG "ERROR: invalid data\n"
#define NAME_MISSING_MSG "ERROR: missing name after %s\n"
#define INVERTED_COMMAS_MISSING "ERROR : missing \"\n"
#define TOO_MANY_OP "ERROR: too many operands\n"
#define FILE_LINE_MSG "file: %s, in line: %d\n"
#define NOT_FOUND_ERROR "line: %d, ERROR: %s was not found\n"
#define EXTRA_CHARS_ERROR "ERROR: extra chars after the line\n"
#define DATA 1
#define NOT_DATA 0
#define SPACE ' '
#define WRITE_FIRST_ROW "%d %d\n"
#define INVERTED_COMMAS_CHAR '"'
#define PRINT_SINGLE_CHAR "%c"
#define OUTPUT_FILE_NAME "%s%s"
#define PRINT_ROW_IN_FILE "%07d "
#define EMPTY 0
#define INITIAL_MEMO_CMD 6
#define INITIAL_MEMO_DATA 1
#define PLUS_CHAR '+'
#define MINUS_CHAR '-'
#define MEMO_COMMAND 3
#define EMPTY_STR ""
#define ENTRY ".entry"
#define ABSOLUTE 4

typedef struct
{	
	int address;        /*label addres*/
	char name[MAXLABEL];/*label name*/
	int iscode;	    /*if = 1 codeline*/
	int isentry;
	int isexternal;     /*if = 1 external*/
	int isdata;	    /*if - .data or.string*/
}labelinfo;

typedef struct
{
	char *name;
	int (*func)();
}data;

typedef struct
{
	char *name;
	int value;
}regs;

typedef struct
{
	unsigned  int line:24;
	unsigned  int :0;

	union
	{
		
		struct 
		{
		

			unsigned int are :3;
			unsigned int funct :5;
			unsigned int opdst :3;
			unsigned int opdstsort :2;
			unsigned int opsrc :3;
			unsigned int opsrcsort :2;
			unsigned int opcode :6;
		}codebit;
		struct
		{
		  unsigned int areim :3;
			unsigned int imidate :21;
			
		}imdbit;
		struct
		{
			unsigned int data :24;
		}databit;
		struct
		{
			unsigned int arerel :3;
			unsigned int relative :21;
			
		}relatbit;
	}valbits;

}bincmd;

typedef struct
{
	char *name;
	unsigned int funct :6;
	unsigned int opcode:6;
	char *(*op1)();
	char *(*op2)();
}cmd;
typedef struct
{
	char name[MAXLABEL];
	int addres;/*IC*/
	int kind;/*1-& 1-location*/
	int lineNum;
	
}lineinfo;

typedef struct Data
{
    labelinfo *symbolArr;
    lineinfo *line;
    bincmd *binCommand;
    bincmd *binData;
    int lineCounter, lab, inCounter, memoryCount, commandArrC, commandArrM, labMemoC;

} Data;

char *cutWhite(char *lineCut);
int stringCut(char *lineCut, int *DC, Data *data);
int dataCut(char *lineCut, int *DC, Data *data);
int externCut(char *lineCut, int *symbolCount, Data *data);
int entryCut();
char checkCase(int hex);
void initialMemo(Data *data);
void freeArrays(Data *data);
char *op013(char *lineCut, int *IC, int *commandLine, int firstOp, Data *data, char *retVal);
char *op13(char *lineCut, int *IC, const int *commandLine, int firstOp, Data *data, char *retVal);
char *op1(char *lineCut, int *IC, int *commandLine, int firstOp, Data *data);
char *op12(char *lineCut, int *IC, const int *commandLine, int firstOp, Data *data, char *retVal);
int secondRead(FILE *file, Data *data, int symbolArr);
void safeExit(Data *data, FILE *inputFile);
int isExtern(Data *data, int symbolCount);
int isEntry(Data *data, int symbolCount);
void writeOBFile(Data* data,const int *IC, const int *DC, char* fileName);
void writeENTFile(Data *data, char *filename, int symbolCount);
void validityFileCheck(FILE *file, Data *data);
void writeEXTFile(Data *data, char *filename, int symbolCount);

int op013Validity(char **lineCut);

int firstRead(FILE *file, int *IC, int *DC, char *fileName);

char *isOpData(Data *data, char *lineCut, char **hash, char *str, int **IC, int firstOp, int *commandLine);

void checkMemo(Data *data);

int isReg(char **lineCut);

char *regToMem(Data *data, char *hash, int regNum, int firstOp, const int *commandLine);

void enterToMemo(Data *data, int firstSortType, int secondSortType, const int *commandLine, int firstOp);

char *jumpKind(Data *data, char *lineCut, const int *commandLine, int *IC, int firstSortKind,
               int secondSortKind, int kind, int flag);
int labelCheck(char *lineCut);

int repCheckName(Data *data, char *lineCut, const int *symbolCount);

void updateSymbolAndCommand(Data *data, int *IC, int symbolFlag, const int *symbolCount, int cmdline,
                            int placeInCmdarr);

void dataToMemo(Data *data, int *lineArg, char *dataArg, int *memoCounter, int *DC, int kind);

int op13ValCheck(char **lineCut);

void addIcToDc(Data data, int symbolCount, const int *IC);

int commandValidity(char *command, char **coma, int *i);

int dataValidityCheck(char *lineCut, char **ret);

int checkRet(char *coma);

int memoCounter(char **line, int *memoCounter);
