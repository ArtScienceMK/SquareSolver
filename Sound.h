#ifndef SOUND_H
#define SOUND_H

/// @brief If filenames are too long, you may increase this value
const int MAX_LEN_COMMAND_LINE = 30;

const char* const CORRECT_SOUND = "sounds/correctSound.wav";  
const char* const INCORRECT_SOUND = "sounds/incorrectSound.wav";  
const char* const GREETINGS_SOUND = "sounds/greetingsSound.wav";

/// @brief Calls for mpv to playback sound
void myPlaySound(const char* const soundLine);

#endif /*SOUND_H*/