#include "structures.h"
dll_list *deck_create();

dll_list *dll_deck_create();

unsigned int dll_get_size(dll_list *list);

void dll_show_all_decks(dll_list *list);

void deck_add_nth_card(dll_list *list, unsigned int n, const void *id);

dll_list *dll_deck_get_nth_card(dll_list *list, unsigned int n);

void dll_free(dll_list *pp_list);

void dll_show_deck(dll_list *list, int n);

dll_list *dll_deck_get_nth_deck(dll_list *list, unsigned int n);

void del_deck(dll_list *list, unsigned int index, int no_message);

dll_list *deck_get_nth_card(dll_list *list, unsigned int n);

void deck_free(dll_list *p_list);

dll_list *deck_remove_nth_card(dll_list *list, unsigned int n);

void dll_deck_add_nth_deck(dll_list *list, unsigned int n, const void *id);

void general_add_node(dll_list *list, dll_list *new_node, unsigned int n);

void del_card(dll_list *list, unsigned int index_deck, unsigned int index_card);

int card_is_valid(card card);

void del_certain_card(dll_list *list_decks, char *token);

void add_additional_deck(dll_list *list_decks, char *token);

void add_additional_cards(dll_list *deck_list, char *token);

void len_of_deck(dll_list *deck_list, char *token);

void shuffle_deck(dll_list *deck_list, char *token);

void split_deck(dll_list *deck_list, char *token);

void merge_decks(dll_list *deck_list, char *token);

void reverse_deck(dll_list *deck_list, char *token);

void sort_deck(dll_list *deck_list, char *token);
