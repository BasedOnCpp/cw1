#include "logger.h"
#include "text.h"

extern struct Info_arr mapper;

void print_out_of_memory_error()
{
	fprintf(stderr, "Error while allocation memory\n");
}

void print_illegal_command_error()
{
	fprintf(stderr, "warning: illegal command found\n");
}

void print_error_while_delete_sentence()
{
	fprintf(stderr, "Error while deleting sentence\n");
}

void print_incorrect_input_error()
{
    fprintf(stderr, "Incorrect input\n");
}

void print_text(const struct Text* text)
{
	if(text == NULL || is_empty_text(text))
	{
		return;
	}
	for(size_t i = 0; i < text->size; ++i)
	{
		printf("%ls", text->data[i].data);
	}
	printf("\n");
}


void print_text_content(FILE* stream, const struct Text* structure, int flags)
{
	for(size_t i = 0; i < structure->size; ++i)
	{
		print_sentence_content(stream, structure->data + i, flags);
		printf("\n");
	}
}

void print_sentence_content(FILE* stream, const struct Sentence* structure, int flags)
{
	if(flags & PRINT_PTR)
	{
		fprintf(stream, "Ptr: [%p] ", structure->data);
	}
	if(flags & PRINT_CONTENT)
	{
		fprintf(stream, "Content: [%ls] ", structure->data);
	}
	if(flags & PRINT_SIZE)
	{
		fprintf(stream, "size: [%zu] ", structure->size);
	}
	if(flags & PRINT_CAPACITY)
	{
		fprintf(stream, "capacity: [%zu] ", structure->capacity);
	}
}

void exception_different_texts(struct Text* expected, struct Text* cmp)
{
	if(expected->size != cmp->size)
	{
		fprintf(stderr, "expected this: %zu to match this: %zu\n", expected->size, cmp->size);
	}
	else
	{
		for (size_t i = 0; i < expected->size; ++i)
		{
			if (!is_equal_sentences(expected->data + i, cmp->data + i))
			{
				fprintf(stderr,"expected this: ");
				print_sentence_content(stderr, expected->data + i, PRINT_CONTENT | PRINT_SIZE);
				fprintf(stderr,"\nto match this: ");
				print_sentence_content(stderr, cmp->data + i, PRINT_CONTENT | PRINT_SIZE);
				fprintf(stderr,"\n");
			}
		}
	}
}

void print_hint()
{
    printf("--------------info--------------\n"
           "| q - terminates program       |\n"
           "| 1 - prints anagrams          |\n"
           "| 2 - sort sentences by count  |\n"
           "| of capital letters           |\n"
           "| 3 - replaces each vowel with |\n"
           "| the next two letters in the  |\n"
           "| alphabet                     |\n"
           "| 5 - sort text by frequency   |\n"
           "| 4 <word> <new_word> replaces |\n"
           "| each word with new_word      |\n"
           "| p - prints text              |\n"
           "| h - prints print_hint        |\n"
           "--------------------------------\n"
    );
}

void print_words()
{

    for(size_t i = 0; i < mapper.size; ++i)
    {
        printf("%ls ", mapper.data[i].data->data);
    }
    printf("\n");
}