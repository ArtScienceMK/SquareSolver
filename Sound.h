#ifndef SOUND_H
#define SOUND_H

const int MAX_LEN_COMMAND_LINE = 30;

const char* const CORRECT_SOUND = "sounds/correctSound.wav";  
const char* const INCORRECT_SOUND = "sounds/incorrectSound.wav";  
const char* const GREETINGS_SOUND = "sounds/greetingsSound.wav";

void myPlaySound(const char* const sound);

#endif /*SOUND_H*/