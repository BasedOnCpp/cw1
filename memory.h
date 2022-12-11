#pragma once
#include <stdbool.h>
#include "structures.h"

bool try_expand_sentence(struct Sentence* sentence, size_t extension);
bool try_expand_text(struct Text* text); //always +1