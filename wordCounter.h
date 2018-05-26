/*
 * wordCounter.h
 *
 *  Created on: Sep 25, 2017
 *      Author: cs3841
 */

#ifndef WORDCOUNTER_H_
#define WORDCOUNTER_H_

/**
 * Method take in word list and file name then fills word list
 * with words from file, delimits all non-alphanumeric characters
 * and capitalizes all letters
 * @param list - wordList for words
 * @param fileName - name of file to be read
 * @return - true if file was found and read, false if file wasnt found
 */
bool processFile(struct tsWordTree *, char *);

void * wordAddWait(void*);

#endif /* WORDCOUNTER_H_ */
