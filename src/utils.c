#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/structures.h"

#define SIZE_64 6
#define SIZE_RAND 5

unsigned sizeof_2_strings(char *prefix, char *suffix){
	return (unsigned)( strlen(prefix) + strlen(suffix) );
}

void rand_string(char *word/*[n]*/, char *prefix, char *suffix, const unsigned n){
	unsigned rnd[SIZE_RAND+1];
	char signs[65], *word_rest;
	unsigned i, j, n_rest, s_index, bit0, bit_count;
	char c;
	const unsigned uint_size = 8*sizeof(unsigned), uint_max = (unsigned)(~0);

	srand((unsigned)time(NULL));
	memset(word, '\0', n*sizeof(char));

	i = strlen(prefix);
	if(i >= n){
		for(i = 0; i < n; i++) word[i] = prefix[i];
		return;
	}

	strcpy(word, prefix);

	word_rest = word + i;
	n_rest = n - i;
	i = strlen(suffix);
	if(i >= n_rest){
		for(i = 0; i < n_rest; i++) word_rest[i] = suffix[i];
		return;
	}

	j = n_rest - i;
	i = (SIZE_RAND*uint_size)/SIZE_64 + 1;
	n_rest = (j < i)?j:i;// min(j, i);


	for(i = j = 0; i < 10; i++, j++) signs[j] = (char)(i+'0');
	for(c = 'A'; c <= 'Z' && j < 64; c++, j++) signs[j] = c;
	for(c = 'a'; c <= 'z' && j < 64; c++, j++) signs[j] = c;
	if(j < 64) signs[j++] = '-';
	if(j < 64) signs[j++] = '_';
	signs[j] = '\0';

	for(i = 0; i < SIZE_RAND; i++)
		rnd[i] = (unsigned)rand();


	for(i = 0; i < n_rest; i++){
		j = (i*SIZE_64)/uint_size;
		if(j > SIZE_RAND) break;
		bit0 = i*SIZE_64 - j*uint_size;

		s_index = (bit0 > 0)?( (rnd[j])>>bit0 ):rnd[j];
		s_index = s_index & ( ~(uint_max<<SIZE_64) );// 32 - bit0  last bits;
		if(bit0 + SIZE_64 > uint_size && j <= SIZE_RAND){
			bit_count = bit0 + SIZE_64 - uint_size;
			bit0 = (rnd[j+1]) & ( ~(uint_max<<bit_count) );
			bit0 = bit0 << (SIZE_64 - bit_count);

			s_index = ( s_index & (~(uint_max<<(SIZE_64 - bit_count) ) ) )|bit0;
		}

		*word_rest = signs[s_index];
		word_rest++;
	}

	i = (unsigned)(word_rest - word);
	n_rest = n - i;
	if(n - i > strlen(suffix) ) strcat(word_rest, suffix);
	else {
		for(j = 0; j < n_rest; j++) word_rest[j] = suffix[j];
	}
}

/**
 * Comparator which provides the array of EC points to be sorted in order that
 *  lower-right points on EC curve are the first
 *  and upper-right points of EC are the last ones in array.
 * @param *d1: first pointer to element in array - struct point;
 * @param *d2: second pointer to element in array - struct point;
 * @returns {int}: a number indicating relative order of points (struct point) in sorted array.
 */
int ecc_compare(const void *d1, const void *d2){
	struct point p1, p2 = *(struct point*)d2;
	p1 = *(struct point*)d1;

	if(p1.y < 0.0 && p2.y >= 0.0)
		return -1;
	else if(p1.y >= 0.0 && p2.y < 0.0)
		return 1;
	else if(p1.y < 0.0){
		if(p1.x < p2.x) return 1;
		else if(p1.x > p2.x) return -1;
	} else {
		if(p1.x < p2.x) return -1;
		else if(p1.x > p2.x) return 1;
	}
	return 0;
}

