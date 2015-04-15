
/*

Author: Steven Miller
Partner: None 
Assignment: lab 3
Due:10/8/14
Description:
USED BINARY SEARCH TREE INSTEAD OF SINGLY LINKED LIST, got permission on piazza (public)

takes input book data from user. executes calculations on the data.
*/

/*include libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/*define constants*/
#define TRUE 1
#define FALSE 0
#define ZERO 0

/*initialize variables*/

/*boolean used to check things*/
int pass = FALSE;
char tempString[35];
/*counting integer*/
int i;

/*intitialize Book structure*/
struct Book {

	int stockNum;
	float wholesalePrice;
	float retailPrice;
	int wholesaleQuantity;
	int retailQuantity;
	char title[35];
	char author[35];
};

/*initialize Node structure: binary search tree*/
struct Node {
	//number of nodes in tree where this node is root. zero means this node has no data
	struct Book data;
	struct Node *right;
	struct Node *left;
};


struct Node *head;
/*used in BST functions*/
struct Node *root;
struct Node *temp;

/*takes in a pointer to a Node as a parameter and makes it point to a new Node, returning true if memory is successfully allocated, false otherwise*/
int/*boolean*/ pointToNewNode(struct Node* *newNodePtr) {
	
	*newNodePtr = (struct Node *) malloc(sizeof(struct Node));

	if(*newNodePtr != NULL){
		/*storage successfully allocated*/
		pass = TRUE;

		//this is for clarity: not actually needed
		(*newNodePtr)->right = NULL;
		(*newNodePtr)->left = NULL;

	}else{
		/*storage not successfully allocated!*/
		printf("storage was not successfully allocated!");
		pass = FALSE;
	}

	return(pass);
}


void addToTree(struct Node** addressOfRootPtr, struct Node* newNodePtr){
	
	//if the root is empty
	if(*addressOfRootPtr == NULL){
		
		*addressOfRootPtr = newNodePtr;

	}else{

		if(strncmp((*addressOfRootPtr)->data.title,newNodePtr->data.title,100) < ZERO){
			addToTree(&((*addressOfRootPtr)->right), newNodePtr);
		}else{
			addToTree(&((*addressOfRootPtr)->left), newNodePtr);
		}
	}
	
}

/*asks for the data for book(s) and makes a new node and adds it to the BST*/
void askAndAddBook(struct Node** headPtr){
	
	printf("Input the following data, pressing enter after each item.\n");
	printf("Terminate with \"#\".\n");
	printf("Book title\nAuthor's name\nBook stock number\nWholesale price\n");
	printf("Retail price\nWholesale quantity\nRetail quantity\n");
	
	//scans first item and checks if it's "#", and keeps going until it finds it.
	scanf(" %[^\n]s", tempString);

	while(tempString[0] != '#'){

		struct Node* newNodePtr;
		pointToNewNode(&newNodePtr);
		
		for(i = 0; i < 35; i++){
			newNodePtr->data.title[i] = tempString[i];
		}
		scanf(" %[^\n][",newNodePtr->data.author);	
		scanf(" %i",&(newNodePtr->data.stockNum));
		scanf(" %f",&(newNodePtr->data.wholesalePrice));
		scanf(" %f",&(newNodePtr->data.retailPrice));
		scanf(" %i",&(newNodePtr->data.wholesaleQuantity));
		scanf(" %i",&(newNodePtr->data.retailQuantity));
		
		addToTree(headPtr, newNodePtr);
		
		scanf(" %[^\n]s", tempString);
	}

}


/*requires **rootPtr != NULL*/
// removes the smallest (alphabetically by data.title) node from *root and returns a pointer to that node
struct Node* removeSmallest(struct Node** rootPtr){

	//recursively call left side
	if((*rootPtr)->left != NULL){
		return(removeSmallest(&((*rootPtr)->left)));
	//make the root point to the right sub tree and return the root
	}else if((*rootPtr)->right != NULL){
		temp = *rootPtr;
		*rootPtr = (*rootPtr)->right;
		temp->right = NULL;
		return(temp);
	//return the root and make root point to null
	}else{
		temp = *rootPtr;
		*rootPtr = NULL;
		return(temp);
	}
}

/*returns sum of all retailPrice*(retailQuantity)*/
float totalRevenue(struct Node* root){
	
	if(root != NULL){
		return(totalRevenue(root->right)+totalRevenue(root->left)+(root->data.retailPrice)*((float) root->data.retailQuantity));
	}else{
		return(0.0);
	}
}

// returns the sum of all wholesalePrice*(wholesaleQuantity)
float totalWholesaleCost(struct Node* root){
	
	if(root != NULL){
		return(totalWholesaleCost(root->right)+totalWholesaleCost(root->left)+(root->data.wholesalePrice)*((float) root->data.wholesaleQuantity));
	}else{
		return(0.0);
	}
}

// returns (total revenue) - (total wholesale cost)
float totalProfit(struct Node* root){
	
	return(totalRevenue(root)-totalWholesaleCost(root));
}

// returns the sum of retailQuantity from every node in the BST
int totalNumSales(struct Node* root){
		
	if(root != NULL){
		return(totalNumSales(root->right)+totalNumSales(root->left)+root->data.retailQuantity);
	}else{
		return(0);
	}
	
}

// returns (total profit)/(total number of sales)
float avgProfit(struct Node* root){

	return(totalProfit(root)/((float) totalNumSales(root)));
}

//prints an alphabetical list of books
void printBookList(struct Node* root){
	
	//4 possible cases: left and right empty, only left empty, only right empty, and both empty
	if(root->left != NULL && root->right != NULL){

		printBookList(root->left);
		printf("%s\n",root->data.title);
		printBookList(root->right);

	}else if(root->right != NULL){
	
		printf("%s\n",root->data.title);
		printBookList(root->right);

	}else if(root->left != NULL){

		printBookList(root->left);
		printf("%s\n", root->data.title);

	}else{
		printf("%s\n", root->data.title);
	}

}

//Deletes the root of the tree
void deleteRoot(struct Node** rootPtr){

	//If there are subtress left and right, then remove the smallest
	//from the right subtree, make that the root and return the old root
	if((*rootPtr)->left != NULL && (*rootPtr)->right != NULL){
		
		temp = removeSmallest(&((*rootPtr)->right));
		temp->right = (*rootPtr)->right;
		temp->left = (*rootPtr)->left;
		free(*rootPtr);
		*rootPtr = temp;
	
	//next two else ifs: if only one sub tree, make root point to that
	//then return the old root
	}else if ((*rootPtr)->left != NULL){
		
		temp = *rootPtr;
		*rootPtr = (*rootPtr)->left;
		free(temp);

	}else if ((*rootPtr)->right != NULL){

		temp = *rootPtr;
		*rootPtr = (*rootPtr)->right;
		free(temp);

	//If there are no subtrees, just make root point to null and return the old root
	}else{
		free(*rootPtr);
		*rootPtr = NULL;
	}
}

// attempts to remove a book from the BST, returning TRUE if successful and FALSE if the book is not in the BST
int/*boolean*/ deleteBook(struct Node** rootPtr, char tempString[]){

	//basic binary search algorithm
	//make result TRUE if we find match
	int/*boolean*/ result = FALSE;
	if(*rootPtr != NULL){

		if(strcmp((*rootPtr)->data.title, tempString) > 0){
			
			result = result || deleteBook(&((*rootPtr)->left), tempString);
				
		}else if (strcmp((*rootPtr)->data.title,tempString) < 0){

			result = result || deleteBook(&((*rootPtr)->right), tempString);

		}else{
			result = TRUE;
			deleteRoot(rootPtr);
		}
	}
	return(result);
}

//ask which calculation to run, then execute the calculation.
int doCalculations(struct Node** headPtr){

	// print out the options, checking if user input is 1 through 9, looping if it's not
	pass = 0;
	while(pass < 1 || pass > 9){
		printf("Type the number corresponding to the calculation you wish you execute, followed by Enter\n");
		printf("1. Determine and print total revenue from all books\n");
		printf("2. Determine and print total wholesale cost for all books\n");
		printf("3. Determine and print toal profit\n");
		printf("4. Determine and print total number of sales\n");
		printf("5. Determine and print average profit per sale\n");
		printf("6. Print book list\n");
		printf("7. Add book\n");
		printf("8. Delete book\n");
		printf("9. Exit Program\n");
		//scan to see what the user has selected
		scanf(" %i", &pass);
		//print message if user entered invalid number
		if(pass < 1 || pass > 9){
			printf("Invalid input! Enter an interger 1-9\n");
		}
	}

	// do the calculation corresponding to the integer 1 through 9 entered
	if(pass == 1){
		printf("the total revenue is: %.2f\n",totalRevenue(*headPtr));
	}else if(pass == 2){
		printf("the toal wholesale cost is: %.2f\n",totalWholesaleCost(*headPtr));
	}else if(pass ==3){
		printf("the total profit is: %.2f\n", totalProfit(*headPtr));
	}else if(pass == 4){
		printf("the total number of sales is: %i\n", totalNumSales(*headPtr));
	}else if(pass == 5){
		printf("the average profit per sale is: %.2f\n", avgProfit(*headPtr));
	}else if(pass == 6){
		printf("book list:\n");
		printBookList(*headPtr);
		printf("\n");
	}else if(pass == 7){
		askAndAddBook(headPtr);
	}else if(pass == 8){
		printf("What is the title of the book you wish to delete?\n");
		scanf(" %[^\n]",tempString);
		// attempt to delete, tell user if the book was deleted
		if(deleteBook(headPtr,tempString)){
			printf("book deleted.\n");
		}else{
			printf("book could not be found.\n");
		}
	}else{
		pass = FALSE;
	}
	return(pass);
	
}

//main function, argc / argv not needed, just there for good practice
int main(int argc, char *argv[]){
	
	//ask for initial book entries
	askAndAddBook(&head);
	
	//keep doing calculations until user wants to quit,
	//more books may be added within "doCalculations()"
	while(doCalculations(&head)){};
	
	return(ZERO);
}

