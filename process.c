#include "process.h"
#include "text.h"
#include "logger.h"
#include <stdlib.h>
#include <locale.h>

struct Info_arr mapper;

int cmp___(const void* lhs, const void* rhs)
{
    int l = ((struct Info*)lhs)->count;
    int r = ((struct Info*)rhs)->count;
    return r - l;
}

void update_frequency(struct Text* text)
{
    destroy(&mapper);
    mapper.data = malloc(sizeof(struct Info) * 1);
    mapper.size = 0;
    mapper.capacity = 1;
    bool found = false;
    for(size_t s = 0; s < text->size; ++s)
    {
        struct Sentence copy = create_sentence(text->data[s].data);
        struct Text txt = split(&copy, L"  .,\n\t\f");
        for(size_t k = 0; k < txt.size; ++k) {
            found = false;
            for (size_t c = 0; c < mapper.size; c++) {
                if (is_equal_sentences(mapper.data[c].data, txt.data + k))
                {
                    mapper.data[c].count++;
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                if(mapper.size >= mapper.capacity)
                {
                    mapper.capacity += 2;
                    struct Info* tmp = realloc(mapper.data, sizeof(struct Info) * mapper.capacity);
                    if(tmp != NULL)
                    {
                        mapper.data = tmp;
                    }
                    else
                    {
                        print_out_of_memory_error();
                        destroy_sentence(&copy);
                        destroy_text(&txt);
                        destroy(&mapper);
                        return;
                    }
                }
                mapper.data[mapper.size].data = malloc(sizeof(struct Sentence) *  (1));
                mapper.data[mapper.size].data->data = malloc(sizeof(wchar_t) *  ((txt.data + k)->size + 1));
                wcscpy(mapper.data[mapper.size].data->data, (txt.data + k)->data);
                mapper.data[mapper.size].count = 1;
                mapper.size++;
            }
        }
        destroy_sentence(&copy);
        destroy_text(&txt);
    }

    if(mapper.size != 0)
    {
        qsort(mapper.data, mapper.size, sizeof(struct Info), cmp___);
    }
}

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

int get_frequency(wchar_t* word) {
    int answer = 0;
    for (size_t i = 0; i < mapper.size; ++i)
    {
        if(!wcscmp(word, mapper.data[i].data->data))
        {
            answer = (int)mapper.data[i].count;
        }
    }
    return answer;
}

int cmp_frequency(const void* lhs, const void* rhs) {
    int l = get_frequency(((struct Sentence *) lhs)->data);
    int r = get_frequency(((struct Sentence *) rhs)->data);
    if (l > r) {
        return -1;
    } else if (l < r) {
        return 1;
    }
    return 0;
}

void sort_sentence_by_frequency(struct Sentence* sentence)
{
    struct Sentence new_sent = create_sentence(sentence->data);

    struct Text splited = split(&new_sent, L" .,\t");

    qsort(splited.data, splited.size, sizeof(struct Sentence), cmp_frequency);

    size_t offset = 0;
    for(size_t i = 0; i < splited.size; ++i)
    {
        wcscpy(sentence->data + offset, splited.data[i].data);
        offset += splited.data[i].size;
        sentence->data[offset] = L' ';
        offset++;
    }
    sentence->data[offset - 1] = L'.';
    sentence->data[offset] = L'\0';

    destroy_text(&splited);
    destroy_sentence(&new_sent);
}

void sort_text_by_frequency(struct Text* text)
{
    update_frequency(text);
    for(size_t i = 0; i < text->size; ++i)
    {
        sort_sentence_by_frequency(text->data + i);
    }
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