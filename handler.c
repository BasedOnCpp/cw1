#include "handler.h"
#include "process.h"
#include "logger.h"
#include "reader.h"
#include "text.h"
#include <stdio.h>


void call_handler(struct Text *text) {
    struct Text command = create_empty_text();
    print_hint();
    while (true) {
        destroy_text(&command);
        command = read_user_command();
        if (is_empty_text(&command)) {
            continue;
        }

        if (command.size == 1) {
            if (!wcscmp(command.data->data, L"h")) {
                print_hint();
                continue;
            }
            if (!wcscmp(command.data->data, L"q")) {
                destroy_text(&command);
                break;
            }
            else if (!wcscmp(command.data->data, L"p")) {
                print_text(text);
            }
            else if (!wcscmp(command.data->data, L"1")) {
                int32_t count = print_anagrams(stdout, text);
                if (!count) {
                    printf("No anagrams in text\n");
                }
            }
            else if (!wcscmp(command.data->data, L"2")) {
                sort_text(text);
                print_text(text);
            }
            else if (!wcscmp(command.data->data, L"3")) {
                if (!replace_vowels(text)) {
                    print_out_of_memory_error();
                    destroy_text(&command);
                    break;
                }
                print_text(text);
            }
            else if (!wcscmp(command.data->data, L"5")) {
                sort_text_by_frequency(text);
                print_words();
            }
            else {
                print_incorrect_input_error();
            }
        }
        else if (command.size == 3) {
            if (!wcscmp(command.data->data, L"4")) {
                if (!is_correct_word(command.data + 1)) {
                    destroy_text(&command);
                    print_illegal_command_error();
                    continue;
                }
                if (!is_correct_word(command.data + 2)) {
                    destroy_text(&command);
                    print_illegal_command_error();
                    continue;
                }
                if (!replace_words(text, command.data + 1, command.data + 2)) {
                    destroy_text(&command);
                    print_out_of_memory_error();
                    break;
                }
                destroy_text(&command);
                print_text(text);
            }
            else {
                print_incorrect_input_error();
            }
        }
        else {
            print_incorrect_input_error();
        }

        destroy_text(&command);
    }
    destroy(&mapper);
    destroy_text(&command);
}
