#ifndef UTILS
#define UTILS

unsigned sizeof_2_strings(char *prefix, char *suffix);
void rand_string(char *word/*[n]*/, char *prefix, char *suffix, const unsigned n);
int ecc_compare(const void *d1, const void *d2);

#endif

