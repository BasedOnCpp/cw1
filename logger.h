#pragma once
#define PRINT_CONTENT (1 << 1)
#define PRINT_SIZE (1 << 2)
#define PRINT_CAPACITY (1 << 3)
#define PRINT_PTR (1 << 4)

#include "structures.h"
#include <stdio.h>

//out of memory
void print_out_of_memory_error();

//incorrect word
void print_illegal_command_error();

void print_incorrect_input_error();

//unable to remove sentence
void print_error_while_delete_sentence();

void print_text(const struct Text* text);

void print_text_content(FILE* stream, const struct Text* structure, int flags);

void print_sentence_content(FILE* stream, const struct Sentence* structure, int flags);

void exception_different_texts(struct Text* expected, struct Text* cmp);

void print_hint();

void print_words();