// Copyright 2022 Dragomir Andrei

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "headers.h"
#include "error_messages.h"
#define COMMAND_MAX 50
#define BUFFER_MAX 200
#define SYMBOL_MAX 15

int main(void)
{
    // Declare and alloc memory for a list of decks
    dll_list *list_decks;
    list_decks = dll_deck_create(sizeof(dll_list *));
    // Alloc memory for a buffer
    char *buff = malloc(BUFFER_MAX);
    while (1)
    {
        // Get size of the deck list
        unsigned int deck_size = dll_get_size(list_decks);
        fgets(buff, BUFFER_MAX, stdin);
        // Separate buffer into words
        char *token = strtok(buff, "  ");

        if (!strncmp(token, "ADD_DECK", 8))
            add_additional_deck(list_decks, token);

        else if (!strncmp(token, "SHOW_ALL", 8))
        {
            // Check validity of input
            if (invalid_command(token))
                continue;
            dll_show_all_decks(list_decks);
        }
        else if (!strncmp(token, "SHOW_DECK", 9))
        {
            token = strtok(NULL, "  ");
            // Check numericity of deck index
            if (!check_is_number(token))
                continue;
            unsigned int index = atoi(token);
            int index_int = atoi(token);
            // Check validity of input
            if (invalid_command(token))
                continue;
            // Check out of bounds error
            if (index_int < 0 || index >= deck_size)
                printf(DECK_INDEX_OUT_OF_BOUNDS);
            else
                dll_show_deck(list_decks, index);
        }
        else if (!strncmp(token, "EXIT", 4))
        {
            // Free all resources allcoated
            dll_free(list_decks);
            break;
        }
        else if (!strncmp(token, "DEL_DECK", 8))
        {
            token = strtok(NULL, "  ");
            unsigned int index = atoi(token);
            int index_int = atoi(token);
            // Check validity of input
            if (invalid_command(token))
                continue;
            // Check out of bounds error
            if (index_int < 0)
            {
                printf(DECK_INDEX_OUT_OF_BOUNDS);
                continue;
            }
            del_deck(list_decks, index, 0);
        }
        else if (!strncmp(token, "DEL_CARD", 8))
            del_certain_card(list_decks, token);

        else if (!strncmp(token, "ADD_CARDS", 9))
            add_additional_cards(list_decks, token);

        else if (!strncmp(token, "DECK_NUMBER", 11))
        {
            // Check validity of input  
            if (invalid_command(token))
                continue;
            printf("The number of decks is %d.\n", deck_size);
        }
        else if (!strncmp(token, "DECK_LEN", 8))
            len_of_deck(list_decks, token);

        else if (!strncmp(token, "SHUFFLE_DECK", 12))
            shuffle_deck(list_decks, token);

        else if (!strncmp(token, "MERGE_DECKS", 11))
            merge_decks(list_decks, token);

        else if (!strncmp(token, "SPLIT_DECK", 10))
            split_deck(list_decks, token);

        else if (!strncmp(token, "REVERSE_DECK", 12))
            reverse_deck(list_decks, token);

        else if (!strncmp(token, "SORT_DECK", 9))
            sort_deck(list_decks, token);
        else
            printf(INVALID_COMMAND);
    }
    // Free the buffer used
    free(buff);
    return 0;
}