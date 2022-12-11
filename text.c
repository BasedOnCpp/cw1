#include "memory.h"
#include "text.h"
#include <stdbool.h>
#include <stdlib.h>
#include <wctype.h>
#include <wchar.h>
#include <string.h>
#include <stdarg.h>

//stores information of replacement vowels
//even idx -> vowel, even idx + 1 -> replacement
//only lower case
struct Text dict;

//init dictionary for vowels replacement
bool init_dict()
{
	return create_text(&dict, 8 * 4,
			L"a", L"bc", L"e", L"fg", L"i", L"jk", L"o", L"pq",
			L"u", L"vw", L"y", L"za", L"а", L"бв", L"е", L"ёж",
			L"ё", L"жз", L"и", L"йл", L"о", L"пр", L"у", L"фх",
			L"ы", L"ьэ", L"э", L"юя", L"ю", L"яа", L"я", L"аб"
	);
}

struct Sentence create_empty_sentence()
{
	return (struct Sentence){ NULL, 0, 0 };
}

struct Text create_empty_text()
{
	return (struct Text){ NULL, 0, 0 };
}
//creates sentence from string literal
struct Sentence create_sentence(const wchar_t* content)
{
	struct Sentence result = create_empty_sentence();
	if (!try_expand_sentence(&result, wcslen(content) + 1))
	{
		destroy_sentence(&result);
		return result;
	}
	wcscpy(result.data, content);
	result.size = wcslen(result.data);
	return result;
}
//creates text from string literals
bool create_text(struct Text* result, size_t n, ...)
{
	*result = create_empty_text();
	va_list factor;
	va_start(factor, n);
	struct Sentence cur_sentence = create_empty_sentence();
	for (size_t i = 0; i < n; ++i)
	{
		cur_sentence = create_sentence(va_arg(factor, const wchar_t*));
		if (!add_to_text(result, &cur_sentence))
		{
			destroy_text(result);
			return false;
		}
	}
	va_end(factor);
	return true;
}

//removes sentence from text
bool remove_sentence(struct Text* text, size_t idx)
{
	if (is_empty_text(text) || idx >= text->size)
	{

		return false;
	}
	destroy_sentence(text->data + idx);
	memmove(text->data + idx, text->data + idx + 1, sizeof(struct Sentence) * (text->size - 1 - idx));
	text->data[text->size - 1] = create_empty_sentence();
	text->size--;
	return true;
}
//adds symbol to sentence
bool add_symbol(struct Sentence* sentence, wchar_t symbol)
{
	if (!try_expand_sentence(sentence, 2))
	{
		return false;
	}
	sentence->data[sentence->size++] = symbol;
	sentence->data[sentence->size] = L'\0';
	sentence->size = wcslen(sentence->data);
	return true;
}
//adds sentence to text
//we copy structure to text (i.e. there is no reason to free sentence)
bool add_to_text(struct Text* text, const struct Sentence* sentence)
{
	if (is_empty_sentence(sentence) || text == NULL)
	{
		return false;
	}
	if (!try_expand_text(text))
	{
		return false;
	}
	memcpy(text->data + text->size, sentence, sizeof(struct Sentence));
	text->size++;
	return true;
}
//erases count elements from position
bool erase(struct Sentence* self, size_t position, size_t count)
{
	if (is_empty_sentence(self))
	{
		return false;
	}
	if (position >= self->size)
	{
		return false;
	}
	if (position + count >= self->size)
	{
		self->data[position] = L'\0';
	}
	else
	{
		memmove(self->data + position, self->data + position + count,
				sizeof(wchar_t) * (wcslen(self->data + position + count) + 1));
	}
	self->size = wcslen(self->data);
	return true;
}
//insert <insertion> from position
bool insert(size_t position, const struct Sentence* insertion, struct Sentence* self)
{
	if (is_empty_sentence(self) || is_empty_sentence(insertion))
	{
		return false;
	}
	if (position > self->size)
	{
		return false;
	}
	if (!try_expand_sentence(self, insertion->size))
	{
		return false;
	}
	memmove(self->data + position + insertion->size, self->data + position,
			sizeof(wchar_t) * (wcslen(self->data + position) + 1));
	memcpy(self->data + position, insertion->data, sizeof(wchar_t) * insertion->size);
	self->size = wcslen(self->data);
	return true;
}
//returns substr of self [begin, end)
struct Sentence substr(const struct Sentence* self, size_t begin, size_t end)
{
	struct Sentence result = create_empty_sentence();
	if (is_empty_sentence(self) || begin >= self->size || begin >= end)
	{
		return result;
	}
	for (size_t i = begin; i < end; ++i)
	{
		if (!add_symbol(&result, self->data[i]))
		{
			destroy_sentence(&result);
			return result;
		}
	}
	return result;
}
//replace every pattern with replacement
bool replace(struct Sentence* self, const struct Sentence* pattern, const struct Sentence* replacement)
{
	if (is_empty_sentence(replacement) || is_empty_sentence(pattern) || is_empty_sentence(self))
	{
		return false;
	}
	for (wchar_t* begin = self->data, * end = (self->data + self->size);
		 begin != end && begin + pattern->size <= end;)
	{
		if (!wcsncmp(begin, pattern->data, pattern->size))
		{
			erase(self, begin - self->data, pattern->size);
			insert(begin - self->data, replacement, self);
			begin += replacement->size;
		}
		else
		{
			begin++;
		}
	}
	return true;
}

struct Text split(struct Sentence *sentence)
{
	struct Text result = create_empty_text();
	if(is_empty_sentence(sentence))
	{
		return result;
	}
	struct Sentence new_sent = create_empty_sentence();
	for(wchar_t* state = NULL, *token = wcstok(sentence->data, L" \f\t\n\r\v", &state);
	token != NULL
	;token = wcstok(NULL, L" \f\t\n\r\v", &state))
	{
		new_sent = create_sentence(token);
		if(!add_to_text(&result, &new_sent))
		{
			destroy_sentence(&new_sent);
			return result;
		}
	}
	return result;
}

//appends lhs from left and rhs from right to word
bool prepare_word(struct Sentence* word, const struct Sentence* lhs, const struct Sentence* rhs)
{
	if(is_empty_sentence(word) || is_empty_sentence(lhs) || is_empty_sentence(rhs))
	{
		return false;
	}
	if(!insert(0, lhs, word))
	{
		return false;
	}
	if(!insert(word->size, rhs, word))
	{
		return false;
	}
	return true;
}
//erase one symbol from left and one from right in word
bool undo_changes(struct Sentence* word)
{
	if(is_empty_sentence(word) || word->size < 2)
	{
		return false;
	}
	if(!erase(word, 0, 1))
	{
		return false;
	}
	if(!erase(word, word->size - 1, 1))
	{
		return false;
	}
	return true;
}
//replaces word in sentence to new_word
bool replace_words_in_sentence(struct Sentence* self, struct Sentence* word, struct Sentence* new_word)
{
	if (is_empty_sentence(self) || is_empty_sentence(word) || is_empty_sentence(new_word))
	{
		return false;
	}
	struct Sentence delim = create_sentence(L" \f\t\n\r\v,.");
	struct Sentence space = substr(&delim, 0, 1);
	struct Sentence lhs = create_empty_sentence();
	struct Sentence rhs = create_empty_sentence();
	if (!insert(0, &space, self))
	{
		return false;
	}
	for (size_t i = 0; i < delim.size; ++i)
	{
		for (size_t j = 0; j < delim.size; ++j)
		{
			lhs = substr(&delim, i, i + 1);
			rhs = substr(&delim, j, j + 1);

			//prepare word and replacement
			if(!prepare_word(word, &lhs, &rhs))
			{
				goto end;
			}
			if(!prepare_word(new_word, &lhs, &rhs))
			{
				goto end;
			}

			//replace
			if(!replace(self, word, new_word))
			{
				goto end;
			}

			//revert changes in word and replacement
			if(!undo_changes(word))
			{
				goto end;
			}
			if(!undo_changes(new_word))
			{
				goto end;
			}

			destroy_sentence(&lhs);
			destroy_sentence(&rhs);
		}
	}

end:
	//release memory
	destroy_sentence(&delim);
	destroy_sentence(&space);
	destroy_sentence(&lhs);
	destroy_sentence(&rhs);
	if (!erase(self, 0, 1)) //erase first space
	{
		return false;
	}
	return true;
}
//replaces vowels in a sentence without self-intersection
bool replace_vowels_in_sentence(struct Sentence* sentence)
{
	for (size_t begin = 0, replaced = false;
		 begin < sentence->size;
		 begin += (replaced == true ? 0 : 1), replaced = false)
	{
		for (size_t idx = 0; idx < dict.size; idx += 2)
		{
			if (!wcsncasecmp(sentence->data + begin, dict.data[idx].data, 1))
			{
				//if we found vowel
				bool upper = iswupper(sentence->data[begin]);
				if (upper) //check case
				{
					invert_case(dict.data + idx + 1);
				}

				if (!erase(sentence, begin, 1))
				{
					if (upper) // restore case
					{
						invert_case(dict.data + idx + 1);
					}
					return false;
				}
				if (!insert(begin, dict.data + idx + 1, sentence))
				{
					if (upper) // restore case
					{
						invert_case(dict.data + idx + 1);
					}
					return false;
				}

				begin += 2;
				if (upper) //restore case
				{
					invert_case(dict.data + idx + 1);
				}
				replaced = true;
				break;
			}
		}
	}
	return true;
}
//inverts case
void invert_case(struct Sentence* self)
{
	if (is_empty_sentence(self))
	{
		return;
	}
	for (size_t o = 0; o < self->size; ++o)
	{
		if (iswupper(self->data[o]))
		{
			self->data[o] = (wchar_t)towlower(self->data[o]);
		}
		else
		{
			self->data[o] = (wchar_t)towupper(self->data[o]);
		}
	}
}
//completely clears sentence
void destroy_sentence(struct Sentence* sentence)
{
	if (sentence == NULL)
	{
		return;
	}
	if (sentence->data != NULL)
	{
		free(sentence->data);
	}
	sentence->data = NULL;
	sentence->size = sentence->capacity = 0;
}
//completely clears text
void destroy_text(struct Text* text)
{
	if (text == NULL)
	{
		return;
	}
	for (size_t i = 0; i < text->size; ++i)
	{
		destroy_sentence(text->data + i);
	}
	if (text->data != NULL)
	{
		free(text->data);
	}
	text->data = NULL;
	text->size = text->capacity = 0;
}
//completely clears dict
void destroy_dict()
{
	destroy_text(&dict);
}

//info functions

bool is_empty_text(const struct Text* text)
{
	return text == NULL || text->size == 0 || text->data == NULL || text->data->data == NULL;
}

bool is_empty_sentence(const struct Sentence* sentence)
{
	return sentence == NULL || sentence->capacity == 0 || sentence->data == NULL;
}
//compares by size and is_equal_sentences
bool is_equal_texts(const struct Text* lhs, const struct Text* rhs)
{
	if (is_empty_text(lhs) && is_empty_text(rhs))
	{
		return true;
	}
	if (lhs->size != rhs->size)
	{
		return false;
	}
	for (size_t i = 0; i < lhs->size; ++i)
	{
		if (!is_equal_sentences(lhs->data + i, rhs->data + i))
		{
			return false;
		}
	}
	return true;
}
//compares by wcscasecmp
bool is_equal_sentences_ignore_case(const struct Sentence* lhs, const struct Sentence* rhs)
{
	if (is_empty_sentence(lhs) || is_empty_sentence(rhs))
	{
		return false;
	}
	return !wcscasecmp(lhs->data, rhs->data);
}
//compares and wcscmp
bool is_equal_sentences(const struct Sentence* lhs, const struct Sentence* rhs)
{
	if (lhs->data == NULL && rhs->data == NULL)
	{
		return true;
	}
	if ((lhs->data == NULL && rhs->data != NULL) || (lhs->data != NULL && rhs->data == NULL))
	{
		return false;
	}
	return !wcscmp(lhs->data, rhs->data);
}
//checks if word is correct (no " ,." in word)
bool is_correct_word(struct Sentence* word)
{
	if(is_empty_sentence(word))
	{
		return false;
	}
	for(size_t i = 0; i < word->size; ++i)
	{
		if(iswspace(word->data[i]) || word->data[i] == L'.' || word->data[i] == L',')
		{
			return false;
		}
	}
	return true;
}

int32_t cmp_sentences(const void* lhs, const void* rhs)
{
	int64_t result = (int64_t)get_count_of_capital(lhs) - (int64_t)get_count_of_capital(rhs);
	if (result < 0)
	{
		result = -1;
	}
	else if (result > 0)
	{
		result = 1;
	}
	return (int32_t)result;
}

int32_t cmp_wchar_t(const void* lhs, const void* rhs)
{
	wchar_t result = *(wchar_t*)(lhs) - *(wchar_t*)(rhs);
	if (result < 0)
	{
		result = -1;
	}
	else if (result > 0)
	{
		result = 1;
	}
	return (int32_t)result;
}
//calculates count of capital letter
size_t get_count_of_capital(const struct Sentence* sentence)
{
	if (is_empty_sentence(sentence))
	{
		return 0;
	}
	size_t answer = 0;
	for (size_t i = 0; i < sentence->size; ++i)
	{
		answer += (iswupper(sentence->data[i]) ? 1 : 0);
	}
	return answer;
}
//return string of isalnum symbols
struct Sentence get_isalnum(const struct Sentence* sentence)
{
	struct Sentence result = create_empty_sentence();

	for (int32_t k = 0; k < sentence->size; ++k)
	{
		//copy if valid
		if (iswalnum(sentence->data[k]))
		{
			if (!add_symbol(&result, sentence->data[k]))
			{
				destroy_sentence(&result);
				break;
			}
		}
	}
	return result;
}
//check if 2 sentences are anagrams
bool is_anagrams(const struct Sentence* lhs, const struct Sentence* rhs)
{
	struct Sentence ll = get_isalnum(lhs);
	struct Sentence rr = get_isalnum(rhs);

	qsort(ll.data, ll.size, sizeof(wchar_t), cmp_wchar_t);
	qsort(rr.data, rr.size, sizeof(wchar_t), cmp_wchar_t);

	bool answer = !wcscmp(ll.data, rr.data);

	destroy_sentence(&ll);
	destroy_sentence(&rr);

	return answer;
}
