/*
 * tsWordtree.h
 *
 *  Created on: Nov 25, 2017
 *      Author: cs3841
 */

#ifndef TSWORDtree_H_
#define TSWORDtree_H_

#include <stdint.h>
#include <pthread.h>

#define WORD_LENGTH (32) //max length allowed for word

struct tsTreeNode {
	char word[WORD_LENGTH+1];
	uint32_t count;
	struct tsTreeNode* lesser;
	struct tsTreeNode* greater;
	pthread_mutex_t nodeUse;
};

struct tsWordTree {
	struct tsTreeNode* head;
	uint32_t wordCount;
};

void init_tswm(struct tsWordTree*);

void addWord_tswm(struct tsWordTree*, char *);

void internalAdd(struct tsTreeNode*, char *);

uint32_t max(uint32_t, uint32_t);

void printTree(struct tsWordTree*);

void internalPrint(struct tsTreeNode*);

void cleanup_tswt(struct tsWordTree*);

void internalCleanup(struct tsTreeNode*);

#endif /* TSWORDtree_H_ */
