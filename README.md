# Basic Dictionary Coder

This is a basic example of a dictionary coder written in C++.  This serves as a basic and rather unoptimized approach.  It works by simultaneously building the dictionary and writing the current 'word' (the col entry) offset in the dictionary.  The use of a hashtable is employed to allow for a fast check to see if the word is already in the dictionary and if so it returns its offset in the dictionary.  Due to this happening simultaneously, the encoded data is stored before the dictionary is written to the output file with a few bytes at the head of the file to give the offset to the start of the dictionary within the encoded file.

## Code Structure
The code for this simple project is consise and stored within a single file, [main.cpp](https://github.com/Mjdgithuber/BasicDictionaryCoder/blob/master/src/main.cpp).  The main functionality is split into three different templated functions encode, decode, and query named after their respective purposes.  Note the use of templates in this code is allow smaller or larger data types to be subbed in to increase the compression ratio.  Currently this feature can only be leveraged by switching the type at compile time, however it would be trivial to have this be adaptive to the input file (but would likely reduce readability).

### Encode
