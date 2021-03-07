#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
size_t strlen(const char *s);
char *strcpy(char *destination, const char *source);
void *memcpy(void *dest, const void *src, size_t n);
char *strcat(char *destination, const char *source);
char *strncat(char *dest, const char *src, size_t n);
int strcmp(const char *str1, const char *str2);
int isalpha(int c);
int isdigit(int c);
int ispunct(int c);

void RemoveChars(char *s, char c);
int BasicValidation(char *s);
int AddZeros(char *s);
int FinalValidation(char *s);
char *subString(char *s, int startI, int endI);
char *ReplaceAns(char *s, char *lastAns);

int checkPrioRanking(char operator);
char *solve(char *s);

int main()
{
    int calculate = 0;
    int on = 1;
    char *ans = "0";

    //printf("%s", ans);

    char operation[128];
    printf("\n");
    printf("=================================================================== \n");
    printf("MINI C CALCULATOR\n");
    printf("<-->\n");
    printf("Operations Allowed: +  -  *  /  ^  ()\n");
    printf("Special Variables: ans[Stores The Output Of The Last Operation]\n");
    printf("Special Commands: cls[Clear Screen], stop[Stop Program]\n");
    printf("<-->\n");
    printf("=================================================================== \n");
    printf("\n \n");

    while (on == 1)
    {
        calculate = 0;
        while (calculate == 0)
        {
            printf("Enter a mathematical operation: \n");

            fgets(operation, 128, stdin);

            char space = ' ';
            RemoveChars(operation, space);

            if (strcmp(operation, "stop\n") == 0 || strcmp(operation, "exit\n") == 0 || strcmp(operation, "off\n") == 0 || strcmp(operation, "cancel\n") == 0 || strcmp(operation, "shutdown\n") == 0 || strcmp(operation, "no\n") == 0 || strcmp(operation, "break\n") == 0 || strcmp(operation, "close\n") == 0)
            {
                on = 0;
                break;
            }
            else if ((strcmp(operation, "cls\n") == 0 || strcmp(operation, "clear\n") == 0))
            {
                system("cls");
                break;
            }
            else if (strcmp(operation, "\n") == 0)
            {
                printf("ERROR: Empty Operation");
                printf("\n \t Please Enter A Valid Operation \n");
                printf("\n \n \n");
                continue;
            }
            strcpy(operation, ReplaceAns(operation, ans));
            //printf("Replacer: %s \n", operation);

            //printf("%s \n", operation);
            if (BasicValidation(operation) == 1 || AddZeros(operation) == 1 || FinalValidation(operation) == 1)
            {
                printf("\n \t Please Enter A Valid Operation \n");
                printf("\n \n \n");
                continue;
            }
            printf("= %s\n", operation);

            ans = solve(operation);
            printf("= %s \n", ans);
            calculate = 1;
            printf("\n");
        }
    }

    return 0;
}

void RemoveChars(char *s, char c)
{
    int writer = 0, reader = 0;

    while (s[reader])
    {
        if (s[reader] != c)
        {
            s[writer++] = s[reader];
        }

        reader++;
    }

    s[writer] = 0;
}

int BasicValidation(char *s)
{
    int openP = 0, closedP = 0;
    int reader = 0;
    while (s[reader])
    {
        if (isalpha(s[reader]))
        {
            printf("ERROR: Invalid Character: %c", s[reader]);
            return 1;
        }
        else if (ispunct(s[reader]))
        {
            if (s[reader] == '(')
            {
                openP++;
            }
            else if (s[reader] == ')')
            {
                closedP++;
            }
            else if (!(s[reader] == '.' || s[reader] == '+' || s[reader] == '-' || s[reader] == '*' || s[reader] == '/' || s[reader] == '^'))
            {
                printf("ERROR: Invalid Character: %c", s[reader]);
                return 1;
            }
        }
        reader++;
    };
    if (closedP > openP)
    {
        printf("ERROR: Missing: ( ");
        return 1;
    }
    else if (openP > closedP)
    {
        printf("ERROR: Missing: ) ");
        return 1;
    }
    return 0;
}
int AddZeros(char *s)
{ //And also *
    char orig[128];
    strcpy(orig, s);

    int length = strlen(orig) - 1;
    int rCount = 0;

    for (int i = -1; i < length; i++)
    {
        if (i == -1)
        {
            if (orig[i + 1] == '.')
            {
                s[0] = '0';
                rCount++;
            }
        }
        else if (i < length - 1 && orig[i + 1] == '.' && ispunct(orig[i]))
        {
            if (orig[i] == '.')
            {
                printf("ERROR: Invalid Set of characters: .. ");
                return 1;
            }
            else
            {
                s[i + rCount] = orig[i];
                s[i + ++rCount] = '0';
            }
        }
        else if (orig[i] == '.' && (i == length - 1 || ispunct(orig[i + 1])))
        {
            rCount--;
        }
        else if (i < length - 1 && orig[i + 1] == '(' && isdigit(orig[i]))
        {
            s[i + rCount] = orig[i];
            s[i + ++rCount] = '*';
        }
        else if (i < length - 1 && orig[i] == ')' && isdigit(orig[i + 1]))
        {
            s[i + rCount] = orig[i];
            s[i + ++rCount] = '*';
        }
        else
        {
            s[i + rCount] = orig[i];
        }
    }
    s[length + rCount] = '\0';

    return 0;
}
int FinalValidation(char *s)
{
    //Final check
    //a) 1.2.0 yes
    //b) *+, +*, /+,
    //c) )( yes
    //d) (/, (), yes
    //e) (, + yes

    int length = strlen(s);
    int dotChecker = -1;
    int parenCounter = 0;

    for (int i = 0; i < length; i++)
    {
        if (ispunct(s[i]) && s[i] != '.')
        {
            dotChecker = -1;
        }
        if (i == length - 1) //Invalid character at the end of the line
        {
            if (ispunct(s[i]) && s[i] != ')')
            {
                printf("ERROR: Missing Digits at the end of the operation");
                return 1;
            }
        }
        else if (i == 0 && ispunct(s[i]))
        {
            if (s[i] != '-' && s[i] != '(')
            {
                printf("ERROR: Invalid Punctuation At The Start Of The Operation");
                return 1;
            }
            else if (s[i] == '(')
            {
                if (ispunct(s[i + 1]) && s[i + 1] != '(' && s[i + 1] != '-')
                {
                    printf("ERROR: Two consecutive invalid punctiation characters: %c %c", s[i], s[i + 1]);
                    return 1;
                }
                else
                {
                    parenCounter++;
                }
            }
            else if (s[i] == '-')
            {
                if (ispunct(s[i + 1]))
                {
                    if (s[i + 1] != '(')
                    {
                        printf("ERROR: Two consecutive invalid punctiation characters: %c %c", s[i], s[i + 1]);
                        return 1;
                    }
                    /*
                    else if (s[i + 1] == '-')
                    {
                        if (i == length - 2 || ispunct(s[i + 2]))
                        {
                            printf("ERROR: Two consecutive punctiation characters: %c %c", s[i], s[i + 1]);
                        }
                    }*/
                }
            }
        }
        else if (s[i] == '(')
        {
            parenCounter++;
            if (ispunct(s[i + 1]) && s[i + 1] != '(' && s[i + 1] != '-')
            {
                printf("ERROR: Two consecutive punctiation characters: %c %c", s[i], s[i + 1]);
                return 1;
            }
        }
        else if (s[i] == ')')
        {
            parenCounter--;
            if (parenCounter < 0)
            {
                printf("ERROR: Invalid Parentheses: Closed Parenthese appears before Opened Parentheses");
                return 1;
            }
        }
        else if (s[i] == '.')
        {
            if (dotChecker == -1)
            {
                dotChecker = i;
            }
            else
            {
                char subString[128];
                int reader = dotChecker - 1;
                int writer = 0;
                while (s[reader])
                {
                    if (reader >= dotChecker - 1 && reader < i)
                    {
                        subString[writer++] = s[reader];
                    }
                    else if (reader == i)
                    {
                        subString[writer] = s[reader];
                        subString[++writer] = '\0';
                    }
                    reader++;
                }
                printf("ERROR: Invalid Number: %s", subString);
                return 1;
            }
        }
        else if (ispunct(s[i]))
        {
            if (ispunct(s[i + 1]))
            {
                if (s[i + 1] == '(')
                {
                    //let it slide
                }
                else if (s[i + 1] == '-')
                {
                    if (i == length - 2)
                    {
                        printf("ERROR: Missing Number At The End Of The Operation");
                        return 1;
                    }
                    else if (ispunct(s[i + 2]) && s[i + 2] != '(')
                    {
                        printf("ERROR: Three Invalid Consecutive Operators %c %c %c", s[i], s[i + 1], s[i + 2]);
                        return 1;
                    }
                }
                else
                {
                    printf("ERROR: Two consecutive invalid punctiation characters: %c %c", s[i], s[i + 1]);
                    return 1;
                }
            }
        }
    }

    return 0;
}

char *solve(char *s)
{
    //start scanning;
    int opPos = -1;
    int cpPos = -1;
    int parenCounter = 0;

    int highestPrio = -1;
    int savedOp = -1;
    int savedF = -1;
    int savedE = -1;

    int countF = 0;
    int saveNext = 0;
    int countOp = -1;

    int i = 0;
    while (s[i])
    {
        if (ispunct(s[i]) && s[i] != '.')
        {
            if (s[i] == '(')
            {
                parenCounter++;
                if (opPos == -1)
                {
                    opPos = i;
                }
            }
            else if (s[i] == ')')
            {
                parenCounter--;
                if (parenCounter == 0)
                {
                    cpPos = i;
                    break;
                }
            }
            else
            {
                //check if it is just a negative num;
                if (s[i] == '-' && (i == 0 || (ispunct(s[i - 1]) && s[i - 1] != ')')))
                {
                    i++;
                    continue;
                }
                //Saving if it has to be saved
                if (saveNext == 1)
                {
                    savedE = i - 1;
                    saveNext = 0;
                }
                //Check if you need to save next num
                if (checkPrioRanking(s[i]) > highestPrio)
                {
                    saveNext = 1;
                    highestPrio = checkPrioRanking(s[i]);
                    savedOp = i;
                    savedF = countF;
                }
                //Save temp
                countF = i + 1;
            }
        }
        i++;
    }
    if (saveNext == 1)
    {
        savedE = strlen(s) - 1;
    }
    //printf("PStart: %d, PEnd: %d \n", opPos, cpPos);
    //printf("Start: %d, Operator: %d, End: %d \n", savedF, savedOp, savedE);

    if (cpPos != -1) //Handle parentheses
    {
        char *starBread;
        if (opPos != 0)
        {
            starBread = subString(s, 0, opPos - 1);
        }
        char *enBread;
        if (cpPos != strlen(s) - 1)
        {
            enBread = subString(s, cpPos + 1, strlen(s) - 1);
        }
        //printf("Start: %s, End: %s \n", startBread, endBread);
        char *sandwichedStr = solve(subString(s, opPos + 1, cpPos - 1));
        //int sandwichLength = strlen(sandwichedStr);
        sandwichedStr[strlen(sandwichedStr)] = '\0';
        //printf("\nsw: %s", sandwichedStr);

        char resuStr[128];
        resuStr[0] = '\0';

        if (opPos != 0)
        {
            strncat(resuStr, starBread, 128);
        }

        strncat(resuStr, sandwichedStr, 128);
        //resuStr[strlen(resuStr)] = '\0';
        //printf("\nRes1: %s", resuStr);

        if (cpPos != strlen(s) - 1)
        {
            //printf("Executed");
            strncat(resuStr, enBread, 128);
        }
        //resuStr[strlen(resuStr)] = '\0';
        //printf("\nRes: %s \n", resuStr);

        return solve(resuStr);
    }
    else if (savedOp != -1) //Handle operations
    {
        char *startBread;
        if (savedF != 0)
        {
            startBread = subString(s, 0, savedF - 1);
        }
        char *endBread;
        if (savedE != strlen(s) - 1)
        {
            endBread = subString(s, savedE + 1, strlen(s) - 1);
        }
        //printf("Start: %s, End: %s \n", startBread, endBread);
        double digit1 = atof(subString(s, savedF, savedOp - 1));
        double digit2 = atof(subString(s, savedOp + 1, savedE));
        //printf("1: %f, 2: %f \n", digit1, digit2);
        double res;
        switch (s[savedOp])
        {
        case '^':
            res = pow(digit1, digit2);
            break;
        case '*':
            res = digit1 * digit2;
            break;
        case '/':
            res = digit1 / digit2;
            break;
        case '+':
            res = digit1 + digit2;
            break;
        case '-':
            res = digit1 - digit2;
            break;
        }
        char strRes[128];
        sprintf(strRes, "%.3f", res);
        //printf("\nResult of Calculation: %s", strRes);
        char finalStr[128] = "";
        if (savedF != 0)
        {
            strcat(finalStr, startBread);
        }
        strcat(finalStr, strRes);
        if (savedE != strlen(s) - 1)
        {
            strcat(finalStr, endBread);
        }

        finalStr[strlen(finalStr)] = '\0';
        //printf("\nOperation simplified: %s", finalStr);

        return solve(finalStr);
    }
    else
    {
        //printf("\nRecursion Bedrock: %s\n", s);
        char *reslt;
        reslt = malloc(sizeof(char) * 128);
        int i = strlen(s) - 1;
        int readr = 0;
        int hasDot = 0;

        while (i >= 0)
        {
            if (s[i] != '0')
            {
                if (s[i] == '.')
                {
                    i--;
                }
                break;
            }
            i--;
        }
        while (s[readr])
        {
            if (s[readr] == '.')
            {
                reslt = subString(s, 0, i);
                hasDot = 1;
            }
            readr++;
        }
        reslt[strlen(reslt)] = '\0';

        //printf("\nPolished Bedrock Result: %s, Index %d\n", reslt, i);
        if (hasDot == 1)
        {
            return reslt;
        }
        else
        {
            return s;
        }
    }
    return "Error";
}

int checkPrioRanking(char operator)
{
    switch (operator)
    {
    case '^':
        return 5;
        break;
    case '*':
        return 4;
        break;
    case '/':
        return 3;
        break;
    case '+':
        return 2;
        break;
    case '-':
        return 1;
        break;
    }
}

char *subString(char *s, int startI, int endI)
{
    char *result;
    result = malloc(sizeof(char) * 128);

    for (int i = startI; i <= endI; i++)
    {
        result[i - startI] = s[i];
    }
    result[endI - startI + 1] = '\0';

    return result;
}

char *ReplaceAns(char *s, char *lastAns)
{
    int ansCheck = 0;
    int ansStrLen = strlen(lastAns);

    int readerCount = 0;
    int writerCount = 0;
    int i = 0;

    char *result;
    result = malloc(sizeof(char) * 128);
    while (s[i + readerCount])
    {
        if (i + readerCount + 2 < strlen(s) && (s[i + readerCount] == 'a' && s[i + readerCount + 1] == 'n' && s[i + readerCount + 2] == 's'))
        {
            //printf("Found one %d\n", i);
            for (int j = 0; j < ansStrLen; j++)
            {
                //printf("%d\n", j);
                result[i + writerCount + j] = lastAns[j];
            }
            writerCount += ansStrLen - 1;
            readerCount += 2;
        }
        else
        {
            result[i + writerCount] = s[i + readerCount];
        }
        i++;
    }
    result[i + writerCount] = '\0';

    return result;
}