// Copyright 2022 Dragomir Andrei

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "headers.h"
#include "error_messages.h"
#define BUFFER_MAX 200

dll_list *deck_get_nth_card(dll_list *list, unsigned int n)
{
    // Function which gets a ceratin card out of a list
    if (!list)
        printf("List is NULL.");

    dll_list *aux = list->head;
    n = n % dll_get_size(list);
    for (unsigned int i = 0; i < n; i++)
        aux = aux->next;

    return aux;
}

void deck_add_nth_card(dll_list *list, unsigned int n, const void *id)
{
    // Function which adds a card in a list
    if (!list)
        printf("List is NULL.");

    dll_list *new_node = malloc(sizeof(dll_list));
    if (!new_node)
    {
        printf("Cant alloc memory for nodes.");
    }
    new_node->value = malloc(sizeof(card));
    if (!new_node->value)
    {
        printf("Cant alloc memory for nodes id.");
    }
    memcpy(new_node->value, id, sizeof(card));

    general_add_node(list, new_node, n);
}

dll_list *deck_remove_nth_card(dll_list *list, unsigned int n)
{
    // Function which can remove a card from a deck
    // It can also remove a deck from the deck list
    dll_list *behind;
    unsigned int size = dll_get_size(list);
    if (n >= size)
        printf(DECK_INDEX_OUT_OF_BOUNDS);

    if (!n)
    {
        dll_list *removed = list->head;
        list->head = list->head->next;
        if (size > 1)
            list->head->prev = NULL;
        return removed;
    }

    behind = deck_get_nth_card(list, n - 1);
    dll_list *removed = behind->next;
    behind->next = removed->next;
    if (n < size - 1)
        removed->next->prev = behind;
    removed->next = NULL;
    removed->prev = NULL;
    return removed;
}

void del_card(dll_list *list, unsigned int idx_deck, unsigned int index_card)
{
    // Function which deletes a card from a given position on a deck
    unsigned int size = dll_get_size(list);
    // Check out of bounds error
    if (idx_deck >= size)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }
    dll_list *aux = dll_deck_get_nth_deck(list, idx_deck);
    // Check out of bounds error for the card index
    unsigned int size_aux = dll_get_size(aux);
    if (index_card >= size_aux)
    {
        printf(CARD_INDEX_OUT_OF_BOUNDS, idx_deck);
        return;
    }

    // Case where is the last card from a deck we delete the whole deck
    if (size_aux == 1)
    {
        del_deck(list, idx_deck, 1);
        printf("The card was successfully deleted from deck %d.\n", idx_deck);
        return;
    }

    dll_list *second = aux->head->next;
    dll_list *removed = deck_remove_nth_card(aux, index_card);
    // If we deleted the first card we move the head
    if (!index_card)
    {
        aux->head = second;
        aux->head->prev = NULL;
    }
    // Free the removed card value
    free(removed->value);
    free(removed);
    printf("The card was successfully deleted from deck %d.\n", idx_deck);
}

int card_is_valid(card card)
{
    // Verifies if a card has correct information
    int valid = 0;
    if (!strncmp(card.symbol, "SPADE", 5))
    {
        valid = 1;
        if (strlen(card.symbol) > 6)
            valid = 0;
    }
    if (!strncmp(card.symbol, "HEART", 5))
    {
        valid = 1;
        if (strlen(card.symbol) > 6)
            valid = 0;
    }
    if (!strncmp(card.symbol, "CLUB", 4))
    {
        valid = 1;
        if (strlen(card.symbol) > 5)
            valid = 0;
    }
    if (!strncmp(card.symbol, "DIAMOND", 7))
    {
        valid = 1;
        if (strlen(card.symbol) > 8)
            valid = 0;
    }
    if (card.number > 14 || card.number < 0)
        valid = 0;

    return valid;
}

void del_certain_card(dll_list *list_decks, char *token)
{
    // Function which handles input and calls a delete card function
    token = strtok(NULL, "  ");
    // Check numericity of input
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);
    int check1, check2;
    check1 = atoi(token);

    token = strtok(NULL, "  ");
    // Check numericity of input
    if (!check_is_number(token))
        return;
    unsigned int index_card = atoi(token);
    check2 = atoi(token);
    // Check validity of command
    if (invalid_command(token))
        return;
    // Check out of bounds error
    if (check1 < 0)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    if (index_deck >= dll_get_size(list_decks))
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    if (check2 < 0)
    {
        printf(CARD_INDEX_OUT_OF_BOUNDS, index_deck);
        return;
    }
    // Call the delete card function
    del_card(list_decks, index_deck, index_card);
}

void add_additional_cards(dll_list *deck_list, char *token)
{
    // Functions which handles input and adds cards to a deck
    token = strtok(NULL, "  ");
    // Check numericity of index
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);
    int check1, check2;
    check1 = atoi(token);

    // Check numericity of index
    token = strtok(NULL, "  ");
    if (!check_is_number(token))
        return;
    unsigned int nr_cards = atoi(token);
    check2 = atoi(token);

    // Check validity of command
    if (invalid_command(token))
        return;
    // Check out of bounds error
    if (check1 < 0)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    if (check2 < 0)
    {
        printf(CARD_INDEX_OUT_OF_BOUNDS, index_deck);
        return;
    }

    if (index_deck >= dll_get_size(deck_list))
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    dll_list *card_deck = dll_deck_get_nth_deck(deck_list, index_deck);
    card card_id;
    unsigned int count = 0;
    char *buff = malloc(BUFFER_MAX);
    // Read each card one by one, then verify its validity, then add it to
    // the specified deck
    while (count < nr_cards)
    {
        fgets(buff, BUFFER_MAX, stdin);
        token = strtok(buff, "  ");
        if (!check_card_is_number(token))
            continue;
        card_id.number = atoi(token);
        token = strtok(NULL, "  ");
        strcpy(card_id.symbol, token);

        if (invalid_card_command(token))
            continue;

        if (card_is_valid(card_id))
        {
            deck_add_nth_card(card_deck, dll_get_size(card_deck), &card_id);
            count++;
        }
        else
            printf(INVALID_CARD);
    }
    free(buff);

    printf("The cards were successfully added to deck %d.\n", index_deck);
}

void len_of_deck(dll_list *deck_list, char *token)
{
    // Function which handles input and show the lenght of a given deck
    token = strtok(NULL, " ");
    // Check numericity of index
    if (!check_is_number(token))
        return;
    unsigned int deck_index = atoi(token);
    // Check validity of command
    if (invalid_command(token))
        return;
    // Check out of bounds error
    if (atoi(token) < 0)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    if (deck_index >= dll_get_size(deck_list))
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    dll_list *aux = deck_list->head;
    unsigned int i = 0;
    // Iteration through the decks
    while (aux)
    {
        // Aux1 si a pointer to the deck on position i
        dll_list *aux1 = (dll_list *)(aux->value);
        int aux1_s = dll_get_size(aux1);
        if (i == deck_index)
        {
            printf("The length of deck %d is %d.\n", deck_index, aux1_s);
            break;
        }
        i++;
        aux = aux->next;
    }
}

void shuffle_deck(dll_list *deck_list, char *token)
{
    // Function which realizes a switch of the first part of the deck
    // with the second part
    token = strtok(NULL, " ");
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);
    // Check validity of command
    if (invalid_command(token))
        return;
    // Check out of bounds error
    if (atoi(token) < 0)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    if (index_deck >= dll_get_size(deck_list))
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    dll_list *cards = dll_deck_get_nth_deck(deck_list, index_deck);
    int size;

    size = dll_get_size(cards);
    if (size == 1)
    {
        printf("The deck %d was successfully shuffled.\n", index_deck);
        return;
    }
    // Get a pointer to the middle of the deck
    dll_list *mid = cards->head;
    int n = 0;
    while (n < size / 2 - 1)
    {
        mid = mid->next;
        n++;
    }
    // Get a pointer to the last node
    dll_list *last = cards->head;
    n = 0;
    while (n < size - 1)
    {
        last = last->next;
        n++;
    }
    // Pointer to first node
    dll_list *first = cards->head;

    // Change the head on middle
    // Link last and first element
    // Break links between element in the middle and his anterior
    cards->head = mid->next;
    last->next = first;
    first->prev = last;
    mid->next->prev = NULL;
    mid->next = NULL;

    printf("The deck %d was successfully shuffled.\n", index_deck);
}

void reverse_deck(dll_list *deck_list, char *token)
{
    // Function which realizes a reversion of the deck
    token = strtok(NULL, "  ");
    // Check numericity of index
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);
    // Check validity of command
    if (invalid_command(token))
        return;
    // Check out of bounds error
    if (atoi(token) < 0)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    if (index_deck >= dll_get_size(deck_list))
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }
    // Get the deck we reverse
    dll_list *deck = dll_deck_get_nth_deck(deck_list, index_deck);
    unsigned int size = dll_get_size(deck);
    // If it is an one card deck we dont have to reverse it
    if (size == 1)
    {
        printf("The deck %d was successfully reversed.\n", index_deck);
        return;
    }

    dll_list *aux = deck->head, *last_node = deck->head;
    unsigned int i = 0;
    while (i < size - 1)
    {
        last_node = last_node->next;
        i++;
    }

    aux = deck->head;
    unsigned int half = size / 2;
    card *tmp = malloc(sizeof(card));
    // Switch info for the cards first <-> last, first+1 <-> last-1 etc
    while (half)
    {
        memcpy(tmp, aux->value, sizeof(card));
        memcpy(aux->value, last_node->value, sizeof(card));
        memcpy(last_node->value, tmp, sizeof(card));

        aux = aux->next;
        last_node = last_node->prev;
        half--;
    }
    free(tmp);
    printf("The deck %d was successfully reversed.\n", index_deck);
}

int symbol_comparison(card a, card b)
{
    // Functions which sets an order of priorities for the symbols
    if (a.symbol[0] == 'H')
        return 1;
    if (b.symbol[0] == 'H')
        return 0;

    if (a.symbol[0] == 'S')
        return 1;
    if (b.symbol[0] == 'S')
        return 0;

    if (a.symbol[0] == 'D')
        return 1;
    if (b.symbol[0] == 'D')
        return 0;

    if (a.symbol[0] == 'C')
        return 1;
    if (b.symbol[0] == 'C')
        return 0;

    return 0;
}

void sort_deck(dll_list *deck_list, char *token)
{
    // Functions which sorts a deck
    token = strtok(NULL, "  ");
    // Check numericity of index
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);
    // Check validity of command
    if (invalid_command(token))
        return;
    // Check out of bounds error
    if (atoi(token) < 0)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    if (index_deck >= dll_get_size(deck_list))
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }
    // Get the deck to sort
    dll_list *deck = dll_deck_get_nth_deck(deck_list, index_deck);
    unsigned int size = dll_get_size(deck);
    // If it is an one element deck it is sorted
    if (size == 1)
    {
        printf("The deck %d was successfully sorted.\n", index_deck);
        return;
    }

    dll_list *i = deck->head, *j;
    card *tmp = malloc(sizeof(card));
    // Change information of cards using a bubble sort algorithm
    while (i->next)
    {
        j = i;
        while (j)
        {
            int i_val = ((card *)(i->value))->number;
            int j_val = ((card *)(j->value))->number;

            if (i_val > j_val)
            {
                memcpy(tmp, i->value, sizeof(card));
                memcpy(i->value, j->value, sizeof(card));
                memcpy(j->value, tmp, sizeof(card));
            }
            else if (i_val == j_val &&
                     !symbol_comparison(*((card *)(i->value)),
                                        *((card *)(j->value))))
            {
                memcpy(tmp, i->value, sizeof(card));
                memcpy(i->value, j->value, sizeof(card));
                memcpy(j->value, tmp, sizeof(card));
            }
            j = j->next;
        }
        i = i->next;
    }

    free(tmp);
    printf("The deck %d was successfully sorted.\n", index_deck);
}

int is_number(char *token)
{
    // Verifies if input is number and not something else
    int sz = strlen(token);
    int start = 0;
    if (token[0] == '-')
        start = 1;
    for (int i = start; i < sz; i++)
        if ((token[i] < '0' || token[i] > '9') && token[i] != '\n')
            return 0;

    return 1;
}

int check_is_number(char *token)
{
    // Handles input and calls is_number function
    if (!is_number(token))
    {
        printf(INVALID_COMMAND);
        return 0;
    }
    return 1;
}

int check_card_is_number(char *token)
{
    // Checks if a card id is number with a different error from the index
    // not number error
    if (!is_number(token))
    {
        printf(INVALID_CARD);
        return 0;
    }
    return 1;
}
