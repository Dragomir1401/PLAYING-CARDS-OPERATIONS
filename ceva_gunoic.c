#include <stdio.h>
#include <string.h>

int is_number(char *token)
{
    int sz = strlen(token);
    int start = 0;
    if (token[0] == '-')
        start = 1;
    for (int i = start; i < sz; i++)
        if (token[i] < '0' || token[i] > '9')
            return 0;

    return 1;
}

int check_is_number(char *token)
{
    if (!is_number(token))
    {
        printf("INVALID_COMMAND");
        return 0;
    }
    return 1;
}

int main()
{
    check_is_number("2a");
    return 0;
}