#include "process.h"
#include "text.h"
#include "logger.h"
#include <stdlib.h>
#include <locale.h>

bool prepare_text(struct Text* text)
{
	if(is_empty_text(text))
	{
		return false;
	}
    for(int32_t i = (int)text->size - 1; i >= 0; --i)
    {
        //delete sentence if last symbol not '.'
        if(text->data[i].data[text->data[i].size - 1] != L'.')
        {
            remove_sentence(text, i);
        }
    }
	erase_repeated_sentences(text);
	if(is_empty_text(text))
	{
		return false;
	}
	return true;
}

void erase_repeated_sentences(struct Text* text)
{
	if (is_empty_text(text))
	{
		return;
	}
	//check every sentence
	for (size_t i = 0; i < text->size; ++i)
	{
		for (size_t j = i + 1; j < text->size; ++j)
		{
			if (is_equal_sentences_ignore_case(text->data + i, text->data + j))
			{
				if(!remove_sentence(text, j))
				{
                    print_error_while_delete_sentence(); //error
					return;
				}
				goto repeat;
			}
		}
	}
	return;

repeat:
	erase_repeated_sentences(text);
}
//more faster
/*
void erase_repeated_sentences(struct Text* text)
{
	if (is_empty_text(text))
	{
		return;
	}
	//check every sentence
	for (size_t i = 0; i < text->size; ++i)
	{
		for (size_t j = i + 1; j < text->size; ++j)
		{
			if (is_equal_sentences_ignore_case(text->data + i, text->data + j))
			{
                destroy_sentence(text->data + j);
			}
		}
	}

    delete:
    for(size_t i = 0; i < text->size; ++i)
    {
        if(is_empty_sentence(text->data + i))
        {
            remove_sentence(text, i);
            goto delete;
        }
    }
}*/

void sort_text(struct Text* text)
{
	qsort(text->data, text->size, sizeof(struct Sentence), cmp_sentences);
}

int32_t print_anagrams(FILE* stream, struct Text* text)
{
	int32_t count = 0;
	for (size_t i = 0; i < text->size; ++i)
	{
		for (size_t j = i + 1; j < text->size; ++j)
		{
			if (is_anagrams(text->data + i, text->data + j))
			{
				++count;
				fprintf(stream, "%ls is anagram to %ls\n", text->data[i].data, text->data[j].data);
			}
		}
	}
	return count;
}

bool replace_vowels(struct Text* text)
{
	if(is_empty_text(text))
	{
		return false;
	}
	for(size_t i = 0; i < text->size; ++i)
	{
		if(!replace_vowels_in_sentence(text->data + i))
		{
			//out of memory
			return false;
		}
	}
	return true;
}

bool replace_words(struct Text* text, struct Sentence* word, struct Sentence* replacement)
{
	if(is_empty_text(text))
	{
		return true;
	}
	if(!is_correct_word(word) || !is_correct_word(replacement))
	{
        print_illegal_command_error(); //incorrect word
		return true;
	}
	for(size_t i = 0; i < text->size; ++i)
	{
		if(!replace_words_in_sentence(text->data + i, word, replacement))
		{
			//out of memory
			return false;
		}
	}
	return true;
}

bool setup_program()
{
	setlocale(LC_ALL, "ru_RU.UTF-8");
	if (!init_dict())
	{
		return false;
	}
	return true;
}

void clean_up_program()
{
	destroy_dict();
}
