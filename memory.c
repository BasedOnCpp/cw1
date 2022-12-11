#include "memory.h"
#include "text.h"
#include "logger.h"
#include <stdlib.h>

bool try_expand_sentence(struct Sentence* sentence, size_t extension)
{
	if (is_empty_sentence(sentence))
	{
		sentence->capacity = BUFFER_INCREASE + 1;
		sentence->data = (wchar_t*)malloc(sizeof(wchar_t) * sentence->capacity);
		if (sentence->data == NULL)
		{
            print_out_of_memory_error(); // out_of_memory
			return false;
		}
	}
	if (sentence->size + extension >= sentence->capacity)
	{
		sentence->capacity = sentence->size + extension + 1;
		wchar_t* tmp = (wchar_t*)realloc(sentence->data, sizeof(wchar_t) * sentence->capacity);
		if (tmp == NULL)
		{
            print_out_of_memory_error(); // out_of_memory
			return false;
		}
		sentence->data = tmp;
	}
	return true;
}

bool try_expand_text(struct Text* text)
{
	if (is_empty_text(text))
	{
		text->capacity = 1;
		text->data = (struct Sentence*)malloc(sizeof(struct Sentence) * text->capacity);
		if (text->data == NULL)
		{
            print_out_of_memory_error(); // out_of_memory
			return false;
		}
	}
	else
	{
		++text->capacity;
		struct Sentence* tmp = (struct Sentence*)realloc(text->data, sizeof(struct Sentence) * text->capacity);
		if (tmp == NULL)
		{
            print_out_of_memory_error(); // out_of_memory
			return false;
		}
		text->data = tmp;
	}
	return true;
}