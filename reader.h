#pragma once
#include <stdio.h>
#include "structures.h"

struct Sentence read_sentence(FILE* stream); //unit test ok
struct Text read_text(FILE* stream); //unit test ok
struct Text read_user_command();
