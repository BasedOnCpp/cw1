#include "reader.h"
#include "text.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

struct Sentence read_sentence(FILE* stream)
{
	struct Sentence result = create_empty_sentence();
	int32_t endline_count = 0;
	for(    wchar_t symbol = (wchar_t)getwc(stream);;
			symbol = (wchar_t)getwc(stream) )
	{
		if(symbol == L'\n')
		{
            endline_count++;
			if(endline_count > 1)
			{
				//if \n\n exists it's an empty sentence
                destroy_sentence(&result);
				break;
			}
		}
		else
		{
            endline_count = 0;
		}
		if(!add_symbol(&result, symbol))
		{
			//reading failed
			destroy_sentence(&result);
			break;
		}
		//end of sentence
		if(symbol == '.')
		{
			break;
		}
	}
	return result;
}

struct Text read_text(FILE* stream)
{
	struct Text result = create_empty_text();
	struct Sentence sent = create_empty_sentence();

	while((sent = read_sentence(stream)).data != NULL)
	{
		if(!add_to_text(&result, &sent))
		{
			destroy_sentence(&sent);
			//failed to add sentence (out of memory)
			destroy_text(&result);
			break;
		}
	}
	return result;
}

struct Text read_user_command()
{
    struct Sentence command = create_empty_sentence();
    struct Text result = create_empty_text();

    if(!try_expand_sentence(&command, 5))
	{
            destroy_sentence(&command);
            return result;
	}
	*command.data = L'\0';
    wchar_t ch = L'\0';
    while((ch = getwchar()) != '\n')
    {
        if(!try_expand_sentence(&command, 1))
        {
            destroy_sentence(&command);
            return result;
        }
        add_symbol(&command, ch);
        command.size = wcslen(command.data);
    }
    command.size = wcslen(command.data);
    result = split(&command);
    destroy_sentence(&command);

    return result;
}
