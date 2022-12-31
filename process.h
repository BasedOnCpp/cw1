#pragma once
#include "structures.h"
#include <stdint.h>

extern struct Info_arr mapper;

bool prepare_text(struct Text* text);

void update_frequency(struct Text* text);

void sort_text_by_frequency(struct Text* text);

//recursion
void erase_repeated_sentences(struct Text* text); //unit tests ok

void sort_text(struct Text* text); //unit tests ok

int32_t print_anagrams(FILE* stream, struct Text* text);

bool replace_vowels(struct Text* text);

bool replace_words(struct Text* text, struct Sentence* word, struct Sentence* replacement); //unit tests ok

bool setup_program();

void clean_up_program();