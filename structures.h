#pragma once
#define MAX_SYMBOL 12

typedef struct dll_list
{
    void *value;
    struct dll_list *head;
    struct dll_list *next, *prev;
} dll_list;

typedef struct card
{
    char symbol[MAX_SYMBOL];
    int number;
} card;