#include <stdio.h>
#include <stdlib.h>
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

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

int globalLevel = 1;
int globalPosition = 4;

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
void showTree(struct node *show, int position, int level);
void view();

int main(int argc, char *argv[]) {
	do {
		char pilihan; 
		int angka;
		view();
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

void showTree(struct node *show, int position, int level){
	char svalue1[100], svalue2[100];
	if(show == NULL) {
    	return; 
	}
	sprintf(svalue1, "%d", show->leftVal);
	if(show->rightVal != NULL){
		sprintf(svalue2, "%d", show->rightVal);
	}else{
		strcpy(svalue2, "x");
	}
  	
    showTree(show->leftChild, 0, level + 1); 
  
  	gotoxy(globalPosition, (level * 2) + 6);
  	struct node *tmp;
  	tmp = show;
  	int p = globalPosition;
  	if(tmp->midChild != NULL && tmp->midChild->leftChild != NULL){
  		while(tmp->midChild != NULL && tmp->midChild->leftChild != NULL){
		  	struct node *tmpMid;
	  		tmpMid = tmp->midChild;
	  		while(tmpMid->leftChild != NULL){
		  		char cvalue1[100], cvalue2[100];
		  		sprintf(cvalue1, "%d", tmpMid->leftChild->leftVal);
				if(tmpMid->leftChild ->rightVal != NULL){
					sprintf(cvalue2, "%d", tmpMid->leftChild->rightVal);
				}else{
					strcpy(cvalue2, "x");
				}
				p += strlen(cvalue1) + strlen(cvalue2) + 9;
				tmpMid = tmpMid->leftChild;
			}
			tmp = tmp->midChild;	
		}
  		gotoxy(p, (level * 2) + 6);
	}
	show->startPosition = p; 
	printf("| ");
	printf("%d", show->leftVal);
	printf(" - ");
	if(show->rightVal != NULL){
		printf("%d", show->rightVal);
	}else{
		printf("x");
	}
	printf(" |");
	
	if(show->leftChild != NULL){
    	char cvalue1[100], cvalue2[100];
  		sprintf(cvalue1, "%d", show->leftChild->leftVal);
		if(show->leftChild->rightVal != NULL){
			sprintf(cvalue2, "%d", show->leftChild->rightVal);
		}else{
			strcpy(cvalue2, "x");
		}
		gotoxy(show->leftChild->startPosition + ((strlen(cvalue1) + strlen(cvalue2) + 9) / 2), (level * 2) + 1 + 6);
		printf("/");
		gotoxy(show->leftChild->startPosition + ((strlen(cvalue1) + strlen(cvalue2) + 9) / 2) + 1, (level * 2) + 6);
		int i;
		for(i = show->leftChild->startPosition + ((strlen(cvalue1) + strlen(cvalue2) + 9) / 2) + 1; i < show->startPosition; i++){
			printf("_");
		}
	}
	if(show->midChild != NULL){ 
		gotoxy(show->startPosition + ((strlen(svalue1) + strlen(svalue2) + 9) / 2), (level * 2) + 1 + 6);
		printf("|");
	}
	
	
	if(show->midChild == NULL){
		globalPosition += strlen(svalue1) + strlen(svalue2) + 9;	
	}
    
    showTree(show->midChild, 0, level + 1);
    
    if(show->rightVal != NULL){
    	showTree(show->rightChild, 0, level + 1);
	}
	
	if(show->rightChild != NULL){
    	char cvalue1[100], cvalue2[100];
  		sprintf(cvalue1, "%d", show->rightChild->leftVal);
		if(show->rightChild->rightVal != NULL){
			sprintf(cvalue2, "%d", show->rightChild->rightVal);
		}else{
			strcpy(cvalue2, "x");
		}
		gotoxy(show->rightChild->startPosition + ((strlen(cvalue1) + strlen(cvalue2) + 9) / 2), (level * 2) + 1 + 6);
		printf("\\");
		gotoxy(show->startPosition + strlen(svalue1) + strlen(svalue2) + 7, (level * 2) + 6);
		int i;
		for(i = show->startPosition + strlen(svalue1) + strlen(svalue2) + 7; i < show->rightChild->startPosition + ((strlen(cvalue1) + strlen(cvalue2) + 9) / 2); i++){
			printf("_");
		}
	}
}

void view(){
	system("cls");
	globalPosition = 4;
	printf("====================================\n");
	printf("||        2-3 Search Tree         ||\n");
	printf("====================================\n\n");
	
	printf("===============================================================================================================\n");
	if(root==NULL){
	printf("\n\n     Tree saat ini masih kosong!");
	}else{
		printf("\n\n");
		showTree(root, 0, 1);
		
	}
	gotoxy(1, (globalLevel * 2) + 7 + 2);
	printf("===============================================================================================================\n");
	
	printf("===================\n");
	printf("||     Menu      ||\n");
	printf("===================\n");
	printf("|| Insertion | 1 ||\n");
	printf("|| Deletion  | 2 ||\n");
	printf("|| Exit      | 3 ||\n");
	printf("===================\n\n");
	
	printf("Pilihan anda : ");
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
