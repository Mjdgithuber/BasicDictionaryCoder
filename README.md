# Basic Dictionary Coder

This is a basic example of a dictionary coder written in C++.  This serves as a basic and rather unoptimized approach.  It works by simultaneously building the dictionary and writing the current 'word' (the col entry) offset in the dictionary.  The use of a hashtable is employed to allow for a fast check to see if the word is already in the dictionary and if so it returns its offset in the dictionary.  Due to this happening simultaneously, the encoded data is stored before the dictionary is written to the output file with a few bytes at the head of the file to give the offset to the start of the dictionary within the encoded file.

