#include <stdio.h>
#include <stdlib.h>

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


// ------- Interfaces -----------------------
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
void inorderTraversal(struct node *treeNode);
void menu();
// --------- End of Interfaces ---------------

int main(int argc, char *argv[]) {
	do {
		inorderTraversal(root);
		int pilihan; 
		int angka;
		menu();
		scanf("%d", &pilihan);
		printf("\n\n");
		switch(pilihan){
			case 1:
				case1:
					doInsertion();
			break;
			case 2:
				// case2:
				// 	printf("Masukkan angka : ");
				// 	scanf("%d", &angka);
				// 	fflush(stdin);
				// 	searchDelete(angka, root);
			break;
			case 3:
			break;
		}
	} while(1);
	return 0;
}

void menu() {
	printf("\n1. Insert");
	printf("\n2. Delete");
	printf("\nPilih: ");
}

void inorderTraversal(struct node *treeNode) {
	if (treeNode == NULL) {
		return;
	}
	if (hasTwoValue(treeNode)) {
		inorderTraversal(treeNode->leftChild);
		printf("l-two-%d  ", treeNode->leftVal);
		inorderTraversal(treeNode->midChild);
		printf("r-two-%d  ", treeNode->rightVal);
		inorderTraversal(treeNode->rightChild);
	}
	if (hasOneValue(treeNode)) {
		inorderTraversal(treeNode->leftChild);
		printf("l-One-%d  ", treeNode->leftVal);
		inorderTraversal(treeNode->rightChild);
	}
}

int hasTwoValue(struct node *treeNode) {
	if (treeNode->leftVal != NULL && treeNode->rightVal != NULL) {
		return 1;
	} 
	return 0;
}

int hasOneValue(struct node *treeNode) {
	if (treeNode->leftVal != NULL && treeNode->rightVal == NULL) {
		return 1;
	} 
	return 0;
}

void doInsertion() {
	int value;
	printf("\nType value that will be insert into tree: ");
	scanf("%d", &value);
	
	struct node *leaf = findLeaf(value, root);
	insert(value, leaf);
	doBalancingAfterInsert(leaf != NULL ? leaf : root);
}

struct node* createNode() {
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	newNode->leftVal = NULL;
	newNode->rightVal = NULL;
	newNode->midVal = NULL;
	newNode->leftChild = NULL;
	newNode->midChild = NULL;
	newNode->rightChild = NULL;
	newNode->parent = NULL;
	newNode->neighborNode = NULL;
	return;
}

struct node *findLeaf(int key, struct node* curr) {
	if (curr == NULL) {
		return curr;
	}
	if (curr->rightChild == NULL && curr->leftChild == NULL && curr->midChild == NULL) {
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
			availableNode->leftVal = availableNode->rightVal;
			availableNode->rightVal = temp;
		}
	} else {
		availableNode->midVal = key;
		if (availableNode->midVal < availableNode->leftVal) {
			int temp = availableNode->leftVal;
			availableNode->leftVal = availableNode->midVal;
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
		root = curr;
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
	if (curr == NULL) {
		return;
	}
	if (nodeNormal(curr) && childHasNeighbor(curr)) {
		attachChildsNeighborBecomeChild(curr);
		return;
	}
	if (nodeNormal(curr)) {
		return;
	}
	raiseMidValueToParent(curr);
	separateNode(curr);
	doBalancingAfterInsert(curr->parent);
}

int childHasNeighbor(struct node* curr) {
	if (curr->leftChild == NULL && curr->rightChild == NULL && curr->midChild == NULL) {
		return 0;
	}
	if ((curr->leftChild != NULL && curr->leftChild->neighborNode != NULL) 
			|| (curr->midChild != NULL && curr->midChild->neighborNode != NULL) 
			|| (curr->rightChild != NULL && curr->rightChild->neighborNode != NULL)) {
		return 1;
	}
}

void attachChildsNeighborBecomeChild(struct node *curr) {
	if (hasOneValue(curr)) {
			curr->rightChild = curr->leftChild->neighborNode;
	} else if (hasTwoValue(curr)) {
		if (curr->leftChild->neighborNode != NULL) {
			curr->midChild = curr->leftChild->neighborNode;
			curr->leftChild->neighborNode = NULL;
		} else {
			curr->midChild = curr->rightChild;
			curr->rightChild = curr->rightChild->neighborNode;
			curr->rightChild->neighborNode = NULL;
		}
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
		curr->leftChild->neighborNode = NULL;
	} else if (curr->midChild->neighborNode != NULL) {
		newNode->leftChild = curr->midChild->neighborNode;
		newNode->rightChild = curr->rightChild;
		curr->midChild->neighborNode = NULL;
		curr->rightChild = curr->midChild;
	} else {
		newNode->leftChild = curr->rightChild;
		newNode->rightChild = curr->rightChild->neighborNode;
		curr->rightChild->neighborNode = NULL;
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
		curr->midVal = NULL;
		newNode->leftChild = curr;
		curr->parent = newNode;
		root = newNode;
	} else {
		addToAvailableNode(parentNode, curr->midVal);
		curr->midVal = NULL;
	} 
}
