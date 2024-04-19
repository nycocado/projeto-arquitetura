#pragma once

#include <stdbool.h> 
#include <stddef.h>

typedef struct List_* List;

List list_create();

void list_destroy(List list, void (*free_element)(void*));

bool list_is_empty(List list);

int list_size(List list);

void* list_get_first(List list);

void* list_get_last(List list);

void* list_get(List list, int position);

int list_find(List list, bool (*equal)(void*, void*), void* element);

void list_insert_first(List list, void* element);

void list_insert_last(List list, void* element);

void list_insert(List list, void* element, int position);

void* list_remove_first(List list);

void* list_remove_last(List list);

void* list_remove(List list, int position);

void list_make_empty(List list, void (*free_element)(void*));

void list_to_array(List list, void** out_array);

int list_count_all(List list, bool (*equal)(void*, void*), void* element);

int list_remove_all(List list, bool (*equal_element)(void*, void*), void (*free_element)(void*), void* element);

int list_remove_duplicates(List list, bool (*equal_element)(void*, void*), void (*free_element)(void*), void* element);

List list_join(List list1, List list2);

void list_print(List list, void (*print_element)(void* element));

List list_get_sublist_between(List list, int start_idx, int end_idx);

List list_get_sublist(List list, int indexes[], int count);

List list_map(List list, void* (*func)(void*));

List list_filter(List list, bool (*func)(void*));

void list_iterator_start(List list);

bool list_iterator_has_next(List list);

void* list_iterator_get_next(List list);