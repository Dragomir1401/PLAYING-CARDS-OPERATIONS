#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "headers.h"
#include "error_messages.h"
#define BUFFER_MAX 200

dll_list *deck_create()
{
    dll_list *list = malloc(sizeof(dll_list));
    if (!list)
        printf("Cant alloc memory for list.");
    list->head = NULL;
    return list;
}

dll_list *deck_get_nth_card(dll_list *list, unsigned int n)
{
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

void del_card(dll_list *list, unsigned int index_deck, unsigned int index_card)
{
    unsigned int size = dll_get_size(list);
    if (index_deck >= size)
    {
        printf(DECK_INDEX_OUT_OF_BOUNDS);
        return;
    }
    dll_list *aux = dll_deck_get_nth_deck(list, index_deck);

    unsigned int size_aux = dll_get_size(aux);
    if (index_card >= size_aux)
    {
        printf(CARD_INDEX_OUT_OF_BOUNDS, index_deck);
        return;
    }

    if (size_aux == 1)
    {
        del_deck(list, index_deck, 1);
        printf("The card was successfully deleted from deck %d.\n", index_deck);
        return;
    }

    dll_list *second = aux->head->next;
    // dll_list *l = aux->head;
    // dll_list *last;
    // while (l)
    // {
    //     if (!l->next)
    //         last = l;
    //     l = l->next;
    // }

    dll_list *removed = deck_remove_nth_card(aux, index_card);
    if (!index_card)
    {
        aux->head = second;
        aux->head->prev = NULL;
    }

    // if (index_card == size - 1)
    // {
    //     last->prev->next = NULL;
    // }
    free(removed->value);
    free(removed);

    printf("The card was successfully deleted from deck %d.\n", index_deck);
}

int card_is_valid(card card)
{
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
    token = strtok(NULL, "  ");
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);
    int check1, check2;
    check1 = atoi(token);

    token = strtok(NULL, "  ");
    if (!check_is_number(token))
        return;
    unsigned int index_card = atoi(token);
    check2 = atoi(token);

    if (invalid_command(token))
        return;

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

    del_card(list_decks, index_deck, index_card);
}

void add_additional_cards(dll_list *deck_list, char *token)
{
    token = strtok(NULL, "  ");
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);
    int check1, check2;
    check1 = atoi(token);

    token = strtok(NULL, "  ");
    if (!check_is_number(token))
        return;
    unsigned int nr_cards = atoi(token);
    check2 = atoi(token);

    if (invalid_command(token))
        return;

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
    token = strtok(NULL, " ");
    if (!check_is_number(token))
        return;
    unsigned int deck_index = atoi(token);

    if (invalid_command(token))
        return;

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
    while (aux)
    {
        dll_list *aux1 = (dll_list *)(aux->value);
        if (i == deck_index)
        {
            printf("The length of deck %d is %d.\n", deck_index, dll_get_size(aux1));
        }
        i++;
        aux = aux->next;
    }
}

void shuffle_deck(dll_list *deck_list, char *token)
{
    token = strtok(NULL, " ");
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);

    if (invalid_command(token))
        return;

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

    dll_list *mid = cards->head;
    int n = 0;
    while (n < size / 2 - 1)
    {
        mid = mid->next;
        n++;
    }

    dll_list *last = cards->head;
    n = 0;
    while (n < size - 1)
    {
        last = last->next;
        n++;
    }
    dll_list *first = cards->head;

    cards->head = mid->next;
    last->next = first;
    first->prev = last;
    mid->next->prev = NULL;
    mid->next = NULL;

    printf("The deck %d was successfully shuffled.\n", index_deck);
}

// void reverse_recursive(dll_list *list, dll_list *node)
// {
//     if (!node || !list)
//         return;

//     dll_list *aux_node = NULL;

//     if (!node->next)
//         list->head = node;
//     else
//         reverse_recursive(list, node->next);

//     aux_node = node->next;
//     node->next = node->prev;
//     node->prev = aux_node;
// }

void reverse_deck(dll_list *deck_list, char *token)
{
    token = strtok(NULL, "  ");
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);

    if (invalid_command(token))
        return;

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
    dll_list *deck = dll_deck_get_nth_deck(deck_list, index_deck);
    unsigned int size = dll_get_size(deck);

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
    token = strtok(NULL, "  ");
    if (!check_is_number(token))
        return;
    unsigned int index_deck = atoi(token);

    if (invalid_command(token))
        return;

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
    dll_list *deck = dll_deck_get_nth_deck(deck_list, index_deck);
    unsigned int size = dll_get_size(deck);

    if (size == 1)
    {
        printf("The deck %d was successfully sorted.\n", index_deck);
        return;
    }

    dll_list *i = deck->head, *j;
    card *tmp = malloc(sizeof(card));

    while (i->next)
    {
        j = i;
        while (j)
        {
            if (((card *)(i->value))->number > ((card *)(j->value))->number)
            {
                memcpy(tmp, i->value, sizeof(card));
                memcpy(i->value, j->value, sizeof(card));
                memcpy(j->value, tmp, sizeof(card));
            }
            else if (((card *)(i->value))->number == ((card *)(j->value))->number && !symbol_comparison(*((card *)(i->value)), *((card *)(j->value))))
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
    if (!is_number(token))
    {
        printf(INVALID_COMMAND);
        return 0;
    }
    return 1;
}

int check_card_is_number(char *token)
{
    if (!is_number(token))
    {
        printf(INVALID_CARD);
        return 0;
    }
    return 1;
}
// dll_list *aux1 = cards->head;
// int half;
// if (size % 2)
//     half = size / 2 + 1;
// else
//     half = size / 2;
// for (int i = 0; i < half; i++)
// {
//     change_nodes(aux1, aux);
//     printf("---%d %d---\n", ((card *)(aux1->value))->number, ((card *)(aux->value))->number);
//     if (aux->next)
//         aux = aux->next;
//     if (aux1->next)
//         aux1 = aux1->next;
// }
// void change_nodes(dll_list *a, dll_list *b)
// {
//     dll_list *left_first, *right_first, *left_second, *right_second;
//     int cases = 0;
//     if (a->prev != NULL)
//         left_first = a->prev;
//     else
//         cases = 1;

//     right_first = a->next;
//     left_second = b->prev;

//     if (b->next != NULL)
//         right_second = b->next;
//     else
//     {
//         if (cases == 1)
//             cases = 3;
//         else
//             cases = 2;
//     }

//     if (cases == 0 || cases == 2)
//     {
//         left_first->next = b;
//         b->prev = left_first;
//     }
//     else
//     {
//         b->prev = NULL;
//     }

//     b->next = right_first;
//     right_first->prev = b;

//     left_second->next = a;
//     a->prev = left_second;

//     if (cases == 0 || cases == 1)
//     {
//         right_second->prev = b;
//         b->next = right_second;
//     }
//     else
//     {
//         b->next = NULL;
//     }
// }