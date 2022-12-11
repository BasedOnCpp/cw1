#pragma once
#include "structures.h"
#include <stdint.h>

//stores information of replacement vowels
//even idx -> vowel, even idx + 1 -> replacement
//only lower case
extern struct Text dict;

//create functions
//init dictionary for vowels replacement
bool init_dict();

struct Sentence create_empty_sentence(); //unit tests ok

struct Text create_empty_text(); //unit tests ok

//creates sentence from string literal
struct Sentence create_sentence(const wchar_t* content); //unit tests ok

//creates text from string literals
bool create_text(struct Text* result, size_t n, ...); //unit tests ok


//process functions

//removes sentence from text
bool remove_sentence(struct Text* text, size_t idx); //unit tests ok

//adds symbol to sentence
bool add_symbol(struct Sentence* sentence, wchar_t symbol); //unit tests ok

//adds sentence to text
//we copy structure to text (i.e. there is no reason to free sentence)
bool add_to_text(struct Text* text, const struct Sentence* sentence); //unit tests ok

//erases count elements from position
bool erase(struct Sentence* self, size_t position, size_t count); //unit tests ok

//insert <insertion> from position
bool insert(size_t position, const struct Sentence* insertion, struct Sentence* self); //unit tests ok

//returns substr of self [begin, end)
struct Sentence substr(const struct Sentence* self, size_t begin, size_t end); //unit tests ??

//replace every pattern with replacement
bool replace(struct Sentence* self, const struct Sentence* pattern, const struct Sentence* replacement); //unit tests ok

struct Text split(struct Sentence *sentence);

//appends lhs from left and rhs from right to word
bool prepare_word(struct Sentence* word, const struct Sentence* lhs, const struct Sentence* rhs);//unit tests ok

//erase one symbol from left and one from right in word
bool undo_changes(struct Sentence* word);//unit test ok

//replaces word in sentence to new_word
bool replace_words_in_sentence(struct Sentence* self, struct Sentence* word, struct Sentence* new_word);//unit tests ok

//replaces vowels in a sentence without self-intersection
bool replace_vowels_in_sentence(struct Sentence* sentence); //unit tests ok

//inverts case
void invert_case(struct Sentence* self); //unit tests ok

//destroy functions
//completely clears sentence
void destroy_sentence(struct Sentence* sentence); //unit tests ok

//completely clears text
void destroy_text(struct Text* text); //unit tests ok

//completely clears dict
void destroy_dict();


//info functions

bool is_empty_text(const struct Text* text); //unit tests ok

bool is_empty_sentence(const struct Sentence* sentence); //unit tests ok

//compares by size and is_equal_sentences
bool is_equal_texts(const struct Text* lhs, const struct Text* rhs); //unit tests ok

//compares by wcscasecmp
bool is_equal_sentences_ignore_case(const struct Sentence* lhs, const struct Sentence* rhs); //unit tests ok

//compares by wcscmp
bool is_equal_sentences(const struct Sentence* lhs, const struct Sentence* rhs); //unit tests ok

//checks if word is correct (no " ,." in word)
bool is_correct_word(struct Sentence* word); //create unit tests ?

//comparison functions
int32_t cmp_sentences(const void*, const void*); //unit tests ok
int32_t cmp_wchar_t(const void* lhs, const void* rhs);//unit tests ok

//calculates count of capital letter
size_t get_count_of_capital(const struct Sentence* sentence); //unit test ok

//return string of isalnum symbols
struct Sentence get_isalnum(const struct Sentence* sentence); //unit test ok

//check if 2 sentences are anagrams
bool is_anagrams(const struct Sentence* lhs, const struct Sentence* rhs); //unit test ok
