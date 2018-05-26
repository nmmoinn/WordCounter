/*
 * wordCounter.
 *
 *  Created on: Sep 25, 2017
 *  Author: Gunther Huebler
 *
 *  Program take in 2 arguments, a letter (C or W) and a file name. The file gets read
 *  and capitalizes and delimits non alphanumeric characters then prints the the words
 *  and the amount of times the words show up. Letter C means the count of the word is printed
 *  then the word is printed. W means the word is printed then the count of the word is printed
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>
#include "tsWordTree.h"
#include "wordCounter.h"

#define BUFFER_MAX (1000000)

static char wordBuffer[BUFFER_MAX][WORD_LENGTH+1];
static int bufferSize = 0;
pthread_mutex_t bufferUse;
bool fileProccessing = true;

int main(int argc, char* argv[]) {

	//ensure arguments exist and are valid
	if(argc != 3) {
		printf("Invalid arguments.\n"
				"Number of child threads (1 or greater)\n"
				"File Name - file path\n");
	}else {

		//list where words will be held
		struct tsWordTree tree;

		//initialize the wordList
		init_tswm(&tree);

		pthread_mutex_init(&bufferUse, NULL);

		pthread_t thread0;
		pthread_create(&thread0, NULL, &wordAddWait, &tree);

		pthread_t thread1;
		pthread_create(&thread1, NULL, &wordAddWait, &tree);

		pthread_t thread2;
		pthread_create(&thread2, NULL, &wordAddWait, &tree);

		//process the file, if successful continue, if not print "File not found."
		if(processFile(&tree, argv[2])) {

			//print words
			printTree(&tree);
		}
		else {
			//prints if proccessFile() was unsuccessful
			printf("File not found.\n");
		}

		fileProccessing = false;

		pthread_join(thread0, NULL);
		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);

		//cleanup_tswt(&tree);
	}
}

/**
 * Method take in word list and file name then fills word list
 * with words from file, delimits all non-alphanumeric characters
 * and capitalizes all letters
 * @param list - wordList for words
 * @param fileName - name of file to be read
 * @return - true if file was found and read, false if file wasnt found
 */
bool processFile(struct tsWordTree * tree, char * fileName) {

	//if list or fileName is null return false
	if(tree != NULL && fileName != NULL) {
		FILE * file;
		int c;

		//open file
		file = fopen(fileName, "r");

		//if file doesnt exist return false
		if(file != NULL) {

			char final[WORD_LENGTH+1];
			int index = 0;

			//read every character in file
			while((c = getc(file)) != EOF) {

				//if the character is a letter then capitalize it
				if((c >= 65 && c<=90) || (c>=97 && c<=122)) {
					final[index++] = toupper(c);
				}
				//if its not a letter and also not an apostrophe or dash then word is over
				else if(c != '\'' && c != '-'){

					//add null terminator
					final[index] = '\0';

					//add word to list if the word was exists
					if(index > 0) {
						addWord_tswm(tree, final);

						//if buffer if full add word yourself
						/*if(bufferSize == BUFFER_MAX) {
							//addWord_tswm(tree, final);
							while(bufferSize == BUFFER_MAX);

						//else send off to other guys
						}else {

							//lock buffer
							pthread_mutex_lock(&bufferUse);

							//increment size
							bufferSize++;

							//add word at size-1
							strcpy(&wordBuffer[bufferSize-1][0], final);

							//unlock buffer
							pthread_mutex_unlock(&bufferUse);
						}*/
					}

					//reset index to build next word
					index = 0;
				}
			}

			//close file and return true indicating success
			fclose(file);
			return true;
		}

		return false;
	}

	return false;
}

void * wordAddWait(struct tsWordTree* tree) {

	char wordSave[WORD_LENGTH+1];

	while(fileProccessing || bufferSize > 0) {

		if(bufferSize == 0) {
			continue;
		}

		//lock buffer
		pthread_mutex_lock(&bufferUse);

		if(bufferSize == 0) {
			pthread_mutex_unlock(&bufferUse);
			continue;
		}

		//copy word at size into
		strcpy(wordSave, &wordBuffer[bufferSize-1][0]);

		//decrement size
		bufferSize--;

		//unlock buffer
		pthread_mutex_unlock(&bufferUse);

		//add word
		addWord_tswm(tree, wordSave);
	}

	return NULL;

}
