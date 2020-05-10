#ifndef CODE_DECODE_H
#define CODE_DECODE_H

#include <iostream>

using namespace std;

void char2bin(char chr, bool bin[], unsigned long long int pos);

void not_bin(bool bin[], unsigned long long int pos, unsigned int pass, unsigned int n);

void count_bin(bool bin[], unsigned long long int pos, unsigned int n, unsigned int &zeros, unsigned int &ones);

void code_method(bool bin[], unsigned long long int len, unsigned int n);

bool get_text(string file_name, string &text, unsigned long long int &len);

void text2bin(string text, unsigned long long int len, bool *&bin);

short int bin2dec(bool bin[], unsigned long long pos);

void bin2text(bool bin[], unsigned long long len, string &text);

void save_text(string file_name, string text, unsigned long long int len);

void decode_method(bool bin[], unsigned long long len, unsigned int n);

void code(string &text, unsigned long long int len, unsigned seed);

void decode(string &text, unsigned long long int len, unsigned seed);

//Estas dos funciones solo son para debug. No estarán en la entrega final.

bool code_file(string file_nat, string file_code, unsigned int seed);

bool decode_file(string file_code, string file_nat, unsigned int seed);

#endif // CODE_DECODE_H
