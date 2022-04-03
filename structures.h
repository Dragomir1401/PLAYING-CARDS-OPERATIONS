// Copyright 2022 Dragomir Andrei

#pragma once
#define MAX_SYMBOL 12

// Structure used for deck list, card list and nodes in each
typedef struct dll_list
{
    void *value;
    struct dll_list *head;
    struct dll_list *next, *prev;
} dll_list;

// Structures which stores information about a card
typedef struct card
{
    char symbol[MAX_SYMBOL];
    int number;
} card;
