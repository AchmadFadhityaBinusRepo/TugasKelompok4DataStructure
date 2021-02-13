#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct node {
	struct node* parent;
	int leftVal;
	int midVal;
	int rightVal;
	struct node* leftChild;
	struct node* midChild;
	struct node* rightChild;
	struct node* neighborNode;
	int startPosition, parentPosition;
} *root; 


int search(int key);
int hasChild(struct node *curr);
struct node* createNode();
struct node *findLeaf(int key, struct node* curr);
void insert(int key, struct node* curr);
void raiseMidValueToParent(struct node *curr);
void separateNode(struct node *curr);
void attachChildToEachNode(struct node *curr, struct node *newNode);
void attachChildNeighborBecomeChild(struct node *curr);
void doBalancingAfterInsert(struct node * curr);
void doInsertion();

int main(int argc, char *argv[]) {
	do {
		char pilihan; 
		int angka;
		scanf("%c", &pilihan);
		fflush(stdin);
		printf("\n\n");
		switch(pilihan){
			case '1':
				case1:
					doInsertion();
			break;
			case '2':
				// case2:
				// 	printf("Masukkan angka : ");
				// 	scanf("%d", &angka);
				// 	fflush(stdin);
				// 	searchDelete(angka, root);
			break;
			case '3':
			break;
		}
	} while(1);
	return 0;
}

void doInsertion() {
	int value;
	printf("\nType value that will be insert into tree: ");
	scanf("%d", &value);
	
	struct node *leaf = findLeaf(value, root);
	insert(value, leaf);
//	doBalancingAfterInsert(leaf);
}

struct node* createNode() {
	return (struct node*)malloc(sizeof(struct node));
}

struct node *findLeaf(int key, struct node* curr) {
	if (curr == root) {
		return curr;
	}
	if (curr->rightChild == NULL & curr->leftChild == NULL && curr->midChild == NULL) {
		return curr;
	} else if (curr->leftVal != NULL && curr->rightVal != NULL) {
		if (curr->leftVal > key) {
			if (curr->leftChild == NULL) {
				return curr;
			}
			findLeaf(key, curr->leftChild);
		} else if (curr->leftVal < key && curr->rightVal > key) {
			if (curr->midChild == NULL) {
				return curr;	
			}
			findLeaf(key, curr->midChild);
		} else {
			if (curr->rightChild == NULL) {
				return curr;
			}
			findLeaf(key, curr->rightChild);
		}
	} else {
		if (curr->leftVal > key) {
			if(curr->leftChild == NULL) {
				return curr;
			}
			findLeaf(key, curr->leftChild);
		} else {
			if (curr->rightChild == NULL) {
				return curr;
			}
			findLeaf(key, curr->rightChild);
		}
	}
}

void addToAvailableNode(struct node *availableNode, int key) {
	if (availableNode->rightVal == NULL) {
		availableNode->rightVal = key;
		if (availableNode->leftVal >= availableNode->rightVal) {
			int temp = availableNode->leftVal;
			availableNode->rightVal = availableNode->leftVal;
			availableNode->leftVal = availableNode->rightVal;
		}
	} else {
		availableNode->midVal = key;
		if (availableNode->midVal < availableNode->leftVal) {
			int temp = availableNode->leftVal;
			availableNode->leftVal = availableNode->leftVal;
			availableNode->midVal = temp;
		} else if (availableNode->midVal > availableNode->rightVal) {
			int temp = availableNode->midVal;
			availableNode->midVal = availableNode->rightVal;
			availableNode->rightVal = temp;
		}
	}
}

void insert(int key, struct node* curr) {
	if (root == NULL) {
		curr = createNode();
		curr->leftVal = key;
		curr->rightVal = NULL;
		curr->midVal = NULL;
		curr->leftChild = NULL;
		curr->midChild = NULL;
		curr->rightChild = NULL;
	} else {
		addToAvailableNode(curr, key);
	}
}

int nodeNormal(struct node* curr) {
	if (curr->midVal != NULL) {
		return 0;
	} 
	return 1;
}

void doBalancingAfterInsert(struct node * curr) {
	if (nodeNormal(curr) && childHasNeighbor(curr)) {
		attachChildsNeighborBecomeChild(curr);
		return;
	}
	raiseMidValueToParent(curr);
	separateNode(curr);
	doBalancingAfterInsert(curr);
}

int childHasNeighbor(struct node* curr) {
	if (curr->leftChild->neighborNode != NULL || curr->rightChild->neighborNode != NULL || curr->leftChild->neighborNode != NULL) {
		return 1;
	}
}

void attachChildsNeighborBecomeChild(struct node *curr) {
	if (curr->leftChild->neighborNode != NULL) {
		curr->midChild = curr->leftChild->neighborNode;
	} else {
		curr->midChild = curr->rightChild;
		curr->rightChild = curr->rightChild->neighborNode;
	}
}

void separateNode(struct node *curr) {
	struct node *newNode = createNode();
	newNode->leftVal = curr->rightVal;
	curr->rightVal = NULL;
	newNode->parent = curr->parent;
	curr->neighborNode = newNode;
	if (hasChild(curr)) {
		attachChildToEachNode(curr, newNode);
	}
}

void attachChildToEachNode(struct node *curr, struct node *newNode) {
	if (curr->leftChild->neighborNode != NULL) {
		newNode->leftChild = curr->midChild;
		newNode->rightChild = curr->rightChild;
		curr->rightChild = curr->leftChild->neighborNode;
	} else if (curr->midChild->neighborNode != NULL) {
		newNode->leftChild = curr->midChild->neighborNode;
		newNode->rightChild = curr->rightChild;
		curr->rightChild = curr->midChild;
	} else {
		newNode->leftChild = curr->rightChild;
		newNode->rightChild = curr->rightChild->neighborNode;
		curr->rightChild = curr->midChild;
	}
	curr->midChild = NULL;
}

int hasChild(struct node *curr) {
	if (curr->leftChild != NULL && curr->midChild != NULL && curr->rightChild != NULL) {
		return 1;
	}
	return 0;
}

void raiseMidValueToParent(struct node *curr) {
	struct node *parentNode = curr->parent;
	
	if (parentNode == NULL) {
		struct node *newNode = createNode();
		newNode->leftVal = curr->midVal;
		newNode->rightVal = NULL;
		newNode->midVal = NULL;
		newNode->leftChild = curr;
		newNode->midChild = NULL;
		newNode->rightChild = NULL;	
	} else {
		addToAvailableNode(parentNode, curr->midVal);
	} 
}
