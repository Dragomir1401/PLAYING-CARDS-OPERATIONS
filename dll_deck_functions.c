// Copyright 2022 Dragomir Andrei

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "headers.h"
#include "error_messages.h"
#define BUFFER_MAX 200

dll_list *dll_deck_create()
{
    // Function which allocates memory for a certain deck
    dll_list *list = malloc(sizeof(dll_list));
    if (!list)
        printf("Cant alloc memory for list.");
    list->head = NULL;
    return list;
}

dll_list *dll_deck_get_nth_deck(dll_list *list, unsigned int n)
{
    // Function which gets a pointer to the deck on the position given
    if (!list)
        printf("List is NULL.");

    dll_list *aux = list->head;
    if (n >= dll_get_size(list))
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return NULL;
    }
    for (unsigned int i = 0; i < n; i++)
        aux = aux->next;

    return ((dll_list *)(aux->value));
}

void dll_deck_add_nth_deck(dll_list *list, unsigned int n, const void *id)
{
    // Functions which adds a new deck to the list of decks
    // Basically adds a node in the list of decks
    if (!list)
        printf("List is NULL.");

    dll_list *new_node = malloc(sizeof(dll_list));
    if (!new_node)
    {
        printf("Cant alloc memory for nodes.");
    }
    new_node->value = malloc(sizeof(dll_list));
    if (!new_node->value)
    {
        printf("Cant alloc memory for nodes id.");
    }
    memcpy(new_node->value, id, sizeof(dll_list));
    dll_list *to_free = (dll_list *)(id);
    free(to_free);
    general_add_node(list, new_node, n);
}

void dll_show_all_decks(dll_list *list)
{
    // Function which shows all the decks
    dll_list *aux = list->head;
    int i = 0;
    // Iteration through the list of decks
    while (aux)
    {
        printf("Deck %d:\n", i);
        // Checker is the deck of cards on position i
        dll_list *checker = ((dll_list *)(aux->value));
        dll_list *aux1 = checker->head;
        card *aux2;
        // Iteration through the cards of the deck on position i
        while (aux1)
        {
            // Aux2 is a certain card in the deck
            aux2 = (card *)(aux1->value);
            printf("\t%d %s", aux2->number, aux2->symbol);
            aux1 = aux1->next;
        }
        i++;
        aux = aux->next;
    }
}

void dll_free(dll_list *pp_list)
{
    // Functions whick frees the list of decks
    int n = dll_get_size(pp_list);
    for (int i = 0; i < n; i++)
    {
        dll_list *aux = deck_remove_nth_card(pp_list, 0);
        deck_free(aux->value);
        free(aux);
    }
    free(pp_list);
}

void deck_free(dll_list *pp_list)
{
    // Functions which frees a double linked list
    int n = dll_get_size(pp_list);
    for (int i = 0; i < n; i++)
    {
        dll_list *aux = deck_remove_nth_card(pp_list, 0);
        free(aux->value);
        free(aux);
    }
    free(pp_list);
}

unsigned int dll_get_size(dll_list *list)
{
    // Functions which calculates the size of a given list
    dll_list *aux = list->head;
    int i = 0;
    while (aux)
    {
        i++;
        aux = aux->next;
    }
    return i;
}

void dll_show_deck(dll_list *list, int n)
{
    // Function which shows the deck on position n
    dll_list *aux = list->head;
    int i = 0;
    // Iteration through the list of decks
    while (aux)
    {
        if (i == n)
        {
            printf("Deck %d:\n", i);
            // Aux1 is the head in the list of cards
            // This list of cards is represented by the deck on position i
            dll_list *aux1 = ((dll_list *)(aux->value))->head;
            card *aux2;
            // Iteration through the cards of the deck on position i
            while (aux1)
            {
                // Aux2 is a certain card in the deck
                aux2 = (card *)(aux1->value);
                printf("\t%d %s", aux2->number, aux2->symbol);
                aux1 = aux1->next;
            }
        }
        i++;
        aux = aux->next;
    }
}

void del_deck(dll_list *list, unsigned int index, int no_message)
{
    // Function which deletes the deck on a given index
    unsigned int size = dll_get_size(list);
    // Check out of bounds error
    if (index >= size)
        printf(DECK_INDEX_OUT_OF_BOUNDS);
    else
    {
        // Get the second node
        dll_list *second;
        if (size >= 1)
            second = list->head->next;
        else
            second = list->head;

        dll_list *removed = deck_remove_nth_card(list, index);
        // If we deleted the first deck --> move the head
        if (!index)
        {
            list->head = second;
            if (second)
                list->head->next = second->next;
            if (second)
                list->head->prev = NULL;
        }
        // Free the removed deck
        deck_free(removed->value);
        free(removed);
        // For operations which use the delete operation we dont have to
        // print a message, but for the operation itself yes
        if (!no_message)
            printf("The deck %d was successfully deleted.\n", index);
    }
}

void general_add_node(dll_list *list, dll_list *new_node, unsigned int n)
{
    // General function which adds a node in a double linked list
    unsigned int size = dll_get_size(list);
    if (!size)
    {
        new_node->next = NULL;
        new_node->prev = NULL;
        list->head = new_node;
        return;
    }

    if (n < size)
    {
        dll_list *aux;
        if (n)
        {
            aux = list->head;
            for (unsigned int i = 0; i < n - 1; i++)
                aux = aux->next;

            new_node->next = aux->next;
            aux->next = new_node;
            new_node->prev = aux;
            new_node->next->prev = new_node;
            return;
        }
        else
        {
            new_node->prev = NULL;
            new_node->next = list->head;
            ((dll_list *)list->head)->prev = new_node;
            list->head = new_node;
            return;
        }
    }

    dll_list *aux = list->head;
    for (unsigned int i = 0; i < size - 1; i++)
        aux = aux->next;
    aux->next = new_node;
    new_node->next = NULL;
    new_node->prev = aux;
}

void add_additional_deck(dll_list *list_decks, char *token)
{
    // Functions which handles input and calls the add_deck function
    token = strtok(NULL, "  ");
    // Check numericity of input
    if (!check_is_number(token))
        return;
    int nr_cards = atoi(token);
    // Check validity of input
    if (invalid_command(token))
        return;
    // Check out of bounds error
    if (nr_cards <= 0)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    // Create a new deck and put the cards from input in it
    dll_list *card_deck = dll_deck_create(sizeof(card));

    card card_id;
    int count = 0;
    char *buff = malloc(BUFFER_MAX);

    while (count < nr_cards)
    {
        fgets(buff, BUFFER_MAX, stdin);
        token = strtok(buff, "  ");
        // Check numericity of card number input
        if (!check_card_is_number(token))
            continue;
        card_id.number = atoi(token);
        token = strtok(NULL, "  ");
        strcpy(card_id.symbol, token);
        // Check invalid added card
        if (invalid_card_command(token))
            continue;
        // If card is valid add it to the deck
        if (card_is_valid(card_id))
        {
            deck_add_nth_card(card_deck, count, &card_id);
            count++;
        }
        else
            printf(INVALID_CARD);
    }

    free(buff);
    // Call the add_deck function
    dll_deck_add_nth_deck(list_decks, dll_get_size(list_decks), card_deck);
    printf("The deck was successfully created with %d cards.\n", nr_cards);
}

void merge_decks(dll_list *deck_list, char *token)
{
    // Function which merges 2 decks by getting linear pairs from each one
    token = strtok(NULL, "  ");
    // Check numericity of input
    if (!check_is_number(token))
        return;
    unsigned int index_deck1 = atoi(token);
    int check1, check2;
    check1 = atoi(token);
    // Check numericity of input
    token = strtok(NULL, "  ");
    if (!check_is_number(token))
        return;
    unsigned int index_deck2 = atoi(token);
    check2 = atoi(token);
    // Check validity of input
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
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    unsigned int size = dll_get_size(deck_list);
    if (index_deck1 >= size || index_deck2 >= size)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    // Create 1 new deck from the 2 decks and add it to end of list
    dll_list *deck1 = dll_deck_get_nth_deck(deck_list, index_deck1);
    dll_list *deck2 = dll_deck_get_nth_deck(deck_list, index_deck2);
    dll_list *new_deck;
    new_deck = dll_deck_create(sizeof(card));

    dll_list *aux1 = deck1->head;
    dll_list *aux2 = deck2->head;

    card *card_id;
    int count = 0;
    while (aux1 && aux2)
    {
        card_id = (card *)(aux1->value);
        deck_add_nth_card(new_deck, count, card_id);
        count++;
        card_id = (card *)(aux2->value);
        deck_add_nth_card(new_deck, count, card_id);
        count++;

        aux1 = aux1->next;
        aux2 = aux2->next;
    }

    if (!aux1)
    {
        while (aux2)
        {
            card *card_id = (card *)(aux2->value);
            deck_add_nth_card(new_deck, count, card_id);
            count++;

            aux2 = aux2->next;
        }
    }

    if (!aux2)
    {
        while (aux1)
        {
            card *card_id = (card *)(aux1->value);
            deck_add_nth_card(new_deck, count, card_id);
            count++;

            aux1 = aux1->next;
        }
    }
    // Delete the 2 old decks from the list
    // By checking which index is lower we can know if the deletion
    // of one deck influences the position of the other
    if (index_deck1 < index_deck2)
    {
        del_deck(deck_list, index_deck1, 1);
        del_deck(deck_list, index_deck2 - 1, 1);
    }
    else
    {
        del_deck(deck_list, index_deck2, 1);
        del_deck(deck_list, index_deck1 - 1, 1);
    }
    // Add the new merged deck
    dll_deck_add_nth_deck(deck_list, size, new_deck);
    printf("The deck %d and the deck %d were successfully merged.\n",
           index_deck1, index_deck2);
}

void split_deck(dll_list *deck_list, char *token)
{
    // Function which splits a deck in 2 by a index
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
    unsigned int index_split = atoi(token);
    check2 = atoi(token);
    // Check validity of input
    if (invalid_command(token))
        return;

    // Check out of bounds cases
    if (check1 < 0)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    if (index_deck >= dll_get_size(deck_list))
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }

    if (check2 < 0)
    {
        printf(CARD_INDEX_OUT_OF_BOUNDS, index_deck);
        return;
    }

    dll_list *deck = dll_deck_get_nth_deck(deck_list, index_deck);

    if (index_split >= dll_get_size(deck))
    {
        printf(CARD_INDEX_OUT_OF_BOUNDS, index_deck);
        return;
    }

    // Alloc memory for 2 separated decks
    dll_list *left_deck, *right_deck;
    left_deck = dll_deck_create(sizeof(card));
    right_deck = dll_deck_create(sizeof(card));

    dll_list *aux = deck->head;

    card *card_id;
    unsigned int count_left = 0, i = 0, count_right = 0;
    // Split the initial deck in 2
    while (aux)
    {
        if (i < index_split)
        {
            card_id = (card *)(aux->value);
            deck_add_nth_card(left_deck, count_left, card_id);
            count_left++;
        }
        else
        {
            card_id = (card *)(aux->value);
            deck_add_nth_card(right_deck, count_right, card_id);
            count_right++;
        }
        i++;
        aux = aux->next;
    }
    // Delete the initial deck
    del_deck(deck_list, index_deck, 1);

    // Add the left deck in the list if it is not empty
    if (count_left)
        dll_deck_add_nth_deck(deck_list, index_deck, left_deck);
    else
        free(left_deck);

    // Add the right deck on a position based on if we added the left one or not
    // If it is empty do not add it
    if (count_right && count_left)
        dll_deck_add_nth_deck(deck_list, index_deck + 1, right_deck);
    else if (count_right && !count_left)
        dll_deck_add_nth_deck(deck_list, index_deck, right_deck);
    else
        free(right_deck);

    printf("The deck %d was successfully split by index %d.\n",
           index_deck, index_split);
}

int invalid_command(char *token)
{
    // Function which tells if input has more parameters than neccessary
    token = strtok(NULL, " ");
    while (token)
    {
        if (token)
        {
            printf(INVALID_COMMAND);
            return 1;
        }
        token = strtok(NULL, " ");
    }
    return 0;
}

int invalid_card_command(char *token)
{
    // Function that tells if a card input has more info than neccessary
    token = strtok(NULL, " ");
    while (token)
    {
        if (token)
        {
            printf(INVALID_CARD);
            return 1;
        }
        token = strtok(NULL, " ");
    }
    return 0;
}
