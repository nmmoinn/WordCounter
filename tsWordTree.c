/*
 * tsWordTree.c
 *
 *  Created on: Nov 25, 2017
 *      Author: cs3841
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tsWordTree.h"

void init_tswm(struct tsWordTree* tree){

	if(tree != NULL) {
		tree->head = (struct tsTreeNode*)malloc(sizeof(struct tsTreeNode));
		memset(tree->head, 0, sizeof(struct tsTreeNode));
		pthread_mutex_init(&tree->head->nodeUse, NULL);
		tree->wordCount = 0;
	}
}

void addWord_tswm(struct tsWordTree* tree, char* word) {
	internalAdd(tree->head, word);
	tree->wordCount++;
}

void internalAdd(struct tsTreeNode* currentNode, char* word) {

	pthread_mutex_lock(&currentNode->nodeUse);

	//string doesnt exist means node has just been created, initialize it
	if(*currentNode->word == '\0') {
		//add to this node
		strcpy(currentNode->word, word);
		currentNode->count = 1;
		pthread_mutex_unlock(&currentNode->nodeUse);
	}
	//string is equal to the one at this node
	else if(strcmp(word, currentNode->word) == 0) {
		currentNode->count++;
		pthread_mutex_unlock(&currentNode->nodeUse);
	}
	//string is less than the one at this node
	else if(strcmp(word, currentNode->word) < 0) {

		//if no lesser node then create one
		if(currentNode->lesser == NULL) {
			//add new node
			currentNode->lesser = (struct tsTreeNode*)malloc(sizeof(struct tsTreeNode));
			memset(currentNode->lesser, 0, sizeof(struct tsTreeNode));
			pthread_mutex_init(&currentNode->lesser->nodeUse, NULL);
		}

		pthread_mutex_unlock(&currentNode->nodeUse);
		internalAdd(currentNode->lesser, word);
	}
	//string is greater than the one at this node
	else if(strcmp(word, currentNode->word) > 0) {

		//if no greater node add one
		if(currentNode->greater == NULL) {
			//add new node
			currentNode->greater = (struct tsTreeNode*)malloc(sizeof(struct tsTreeNode));
			memset(currentNode->greater, 0, sizeof(struct tsTreeNode));
			pthread_mutex_init(&currentNode->greater->nodeUse, NULL);
		}

		pthread_mutex_unlock(&currentNode->nodeUse);
		internalAdd(currentNode->greater, word);
	}
}

uint32_t getSize(struct tsTreeNode* currentNode) {

	uint32_t sizeL = 0;
	uint32_t sizeG = 0;

	if(currentNode->lesser == NULL && currentNode->greater == NULL) {
		return 0;
	}

	if(currentNode->lesser != NULL) {
		sizeL = getSize(currentNode->lesser);
	}

	if(currentNode->greater != NULL) {
		sizeG = getSize(currentNode->greater);
	}

	return 1 + max(sizeG, sizeL);
}

uint32_t max(uint32_t a, uint32_t b) {

	if(a > b) {
		return a;
	}else {
		return b;
	}
}

void printTree(struct tsWordTree* tree) {
	internalPrint(tree->head);
	printf("Total words: %d\n", tree->wordCount);
}

void internalPrint(struct tsTreeNode* currentNode) {
	printf("%d %s\n", currentNode->count, currentNode->word);

	if(currentNode->lesser != NULL) {
		internalPrint(currentNode->lesser);
	}

	if(currentNode->greater != NULL) {
		internalPrint(currentNode->greater);
	}
}

void cleanup_tswt(struct tsWordTree* tree) {

	if(tree != NULL && tree->head != NULL) {

		internalCleanup(tree->head);
		free(tree->head);

	}
}

void internalCleanup(struct tsTreeNode* currentNode) {

	if(currentNode->lesser != NULL) {
		internalCleanup(currentNode->lesser);
		free(currentNode->lesser);
	}

	if(currentNode->greater != NULL) {
		internalCleanup(currentNode->greater);
		free(currentNode->greater);
	}

}

