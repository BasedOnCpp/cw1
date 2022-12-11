#include "handler.h"
#include "reader.h"
#include "logger.h"
#include "text.h"
#include "process.h"
#include <stdio.h>

int main()
{
	if(!setup_program())
	{
		fprintf(stderr, "Error while init base structures\n");
		return 0; // change error code?
	}
	//run_unit_tests(); //uncomment to run unit tests

	printf("Enter your text:\n");
	struct Text text = read_text(stdin);
	if(!prepare_text(&text))
	{
		destroy_text(&text);
		fprintf(stderr, "Error: invalid text\n");
		return 0; // change error code?
	}
	printf("Your text:\n");
	print_text(&text);

	//start user interface
	call_handler(&text);

	//release memory and terminate
	destroy_text(&text);
	clean_up_program();
	return 0;
}
