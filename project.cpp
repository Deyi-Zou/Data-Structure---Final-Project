/* FILE : project.cpp                                            */
/* PROJECT : Final project                                       */
/* PROGRAMMER : Deyi Zou                                         */
/* FIRST VERSION : 2024-08-11                                    */
/* DESCRIPTION : The objective of this assignment is to combine  */
/*               knowledge of Hash Table and Tree structure to   */
/*				 arrange large chunk of data in Hash Table array */
/*               Each array element holds the pointer to the     */
/*				 root element of the Tree structure.			 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS_NUMBER 127
#define BUFFER_LENGTH 5000

typedef struct Parcel 
{
	char* destination;
	int weight;
	float value;
	Parcel* left;
	Parcel* right;
};

typedef struct HashTable
{
	Parcel* hashTable[BUCKETS_NUMBER];
};

 
HashTable* initializeHashTable(void);
Parcel* creatNode(char* destination, int weight, float value);
int hashfunction(char* str);
void deleteRC(char* record);
void insertHashTable(HashTable* countries, char* destination, int weight, float value);
Parcel* insertTree(Parcel* root, Parcel* newNode);
void showNode(Parcel* root);
void showWholeTree(Parcel* root);
void showHigher(Parcel* root, int newWeight);
void showLower(Parcel* root, int newWeight);
int CaculatetotalLoad(Parcel* root);
float  CaculatetotalValutation(Parcel* root);
Parcel* findCheapest(Parcel* root);
Parcel* findExpensive(Parcel* root );
void findLightest(Parcel* root);
void freeTree(Parcel* root);

int main(void)
{
	HashTable* countries = initializeHashTable();
	char buffer[BUFFER_LENGTH] = {};
	FILE* ptr = NULL;
	char destination[BUFFER_LENGTH] = {};
	int weight = 0;
	float value = 0.0;
	int choice = 0;

	// file IO
	if (fopen_s(&ptr, "./couriers.txt", "r") != 0)
	{
		printf("Error of file Openning.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		while (!feof(ptr))
		{
			if (fgets(buffer, BUFFER_LENGTH, ptr) == NULL)
			{
				printf("Error of file Reading\n");
				exit(EXIT_FAILURE);
			}
			// extract datas:
			sscanf_s(buffer, "%[^,], %d, %f", destination, BUFFER_LENGTH, &weight, &value);
			deleteRC(destination);
			insertHashTable(countries, destination, weight, value);
		}
	}
	if (fclose(ptr) != 0)
	{
		printf("Error of file closing\n");
		exit(EXIT_FAILURE);
	}

	// User Interface
	
	//If the loop keeps repeating after input, it�s likely due to scanf_s left a '\n' in the input buffer, then it read by another scanf_s, then problem comes.
	//Clearing the buffer after each input should prevent the loop from repeating indefinitely.
	// Or just use fgets + sscanf.
	while (choice != 6)
	{		
		float inputWeight = 0.0;
		char countryName[BUFFER_LENGTH] = {};
		printf("1. Enter country name and display all the parcels details.\n");
		printf("2. Enter country and weight pair. Show parcels with higher and lower weight.\n");
		printf("3. Display the total parcel load and valuation for the country.\n");
		printf("4. Enter the country name and display cheapest and most expensive parcel's details.\n");
		printf("5. Enter the country name and display lightest and heaviest parcel for the country.\n");
		printf("6. Exit the application.\n");
		printf("Please choose from the menu:");
		fgets(buffer, BUFFER_LENGTH, stdin);
		sscanf_s(buffer, "%d", &choice);
		printf("No. is : %d \n", choice);

		switch (choice)
		{
		case 1:
			printf("Please enter country name: ");
			fgets(countryName, BUFFER_LENGTH, stdin);
			deleteRC(countryName);
			printf("\n");
			showWholeTree(countries->hashTable[hashfunction(countryName)]);
			break;
		case 2:
			printf("Please enter country name: ");
			fgets(countryName, BUFFER_LENGTH, stdin);
			deleteRC(countryName);
			printf("\n");
			printf("Please enter the weight: ");
			fgets(buffer, BUFFER_LENGTH, stdin);
			sscanf_s(buffer, "%f", &inputWeight);
			printf("\n");
			printf("they are: %f, %s \n", inputWeight, countryName);
			printf("\n");
			printf("Parcels have higher weight:\n\n");
			showHigher(countries->hashTable[hashfunction(countryName)], inputWeight);
			printf("\n");
			printf("Parcels have lower weight:\n\n");
			showLower(countries->hashTable[hashfunction(countryName)], inputWeight);
			break;
		case 3:
			printf("Please enter country name: ");
			fgets(countryName, BUFFER_LENGTH, stdin);
			deleteRC(countryName);
			printf("\n");
			printf("Total load is %dg.\n", CaculatetotalLoad(countries->hashTable[hashfunction(countryName)]));
			printf("Total valuation is $%f.\n", CaculatetotalValutation(countries->hashTable[hashfunction(countryName)]));
			break;
		case 4:
			printf("Please enter country name: ");
			fgets(countryName, BUFFER_LENGTH, stdin);
			deleteRC(countryName);
			printf("\n");
			printf("The most cheapest value is :\n");
			showNode(findCheapest(countries->hashTable[hashfunction(countryName)]));
			printf("The most expensive value is :\n"); 
			showNode(findExpensive(countries->hashTable[hashfunction(countryName)]));
			break;
		case 5:
			printf("Please enter country name: ");
			fgets(countryName, BUFFER_LENGTH, stdin);
			deleteRC(countryName);
			printf("\n");
			findLightest(countries->hashTable[hashfunction(countryName)]);
			printf("\n");
			break;
		case 6:
			printf("");
			break;
		default:
			printf("Wrong number, choose again please.\n");
			break;
		}
		printf("\n");
	}

	//free:
	for (int i = 0; i < BUCKETS_NUMBER; i++)
	{
		freeTree(countries->hashTable[i]);
	}
	free(countries);

	return 0;
}


/* ================================================================ */
/* FUNCTION : findLightest( root)                                   */
/* PURPOSE  : This function is to display lightest and heaviest     */
/*			  parcel for the country								*/					
/* INPUTS   : root - the root of the tree                           */
/* RETURNS  : None                                                  */
/* ================================================================ */
void findLightest(Parcel* root)
{
	Parcel* lightest = NULL;
	Parcel* heaviest = NULL;
	if (root == NULL)
	{
		printf("No parcel for this destation.\n");
		return;
	}
	Parcel* current = root;
	while (current->left != NULL)
	{
		current = current->left;
	}
	lightest = current;

	current = root;
	while (current->right != NULL)
	{
		current = current->right;
	}
	heaviest = current;

	printf("The lightest parcel is :\n");
	showNode(lightest);
	printf("The heaviest parcel is :\n");
	showNode(heaviest);
}

/* ================================================================ */
/* FUNCTION : findCheapest( root)									*/
/* PURPOSE  : This function is to find cheapest parcel�s details	*/
/* INPUTS   : root - the root of the tree                           */
/* RETURNS  : float - the cheapest value so far                     */
/* ================================================================ */
Parcel* findCheapest(Parcel* root )
{
	if (root == NULL)
	{
		return NULL;
	}

	Parcel* leftCheapest = findCheapest(root->left);
	Parcel* rightCheapest = findCheapest(root->right);

	Parcel* cheapest = root;

	if (leftCheapest != NULL &&
		leftCheapest->value <= cheapest->value)
	{
		cheapest = leftCheapest;
	}
	
	if (rightCheapest != NULL &&
		rightCheapest->value <= cheapest->value )
	{
		cheapest =  rightCheapest;
	}

	return cheapest;
}

/* ================================================================ */
/* FUNCTION : findExpensive( root)									*/
/* PURPOSE  : This function is to find most expensive parcel		*/
/* INPUTS   : root - the root of the tree                           */
/* RETURNS  : float - the expensive value so far                    */
/* ================================================================ */
Parcel* findExpensive(Parcel* root)
{
	if (root == NULL)
	{
		return NULL;
	}

	Parcel* leftExpensive = findExpensive(root->left);
	Parcel* rightExpensive = findExpensive(root->right);

	Parcel* expensive = root;

	if (leftExpensive != NULL &&
		leftExpensive->value >= expensive->value)
	{
		expensive = leftExpensive;
	}

	// important: if, rather than else if
	if (rightExpensive != NULL &&
		rightExpensive->value >= expensive->value)
	{
		expensive = rightExpensive;
	}

	return expensive;
}
 
/* ================================================================ */
/* FUNCTION : CaculatetotalValutation( root)                        */
/* PURPOSE  : This function is to cumulative total valuation of     */
/*			  all the parcels.										*/	
/* INPUTS   : root - the root of the tree                           */
/* RETURNS  : float - the total valuation			                */
/* ================================================================ */
float  CaculatetotalValutation(Parcel* root)
{
	if (root == NULL)
	{
		return 0;
	}
	float rootValuation = root->value;
	float leftValuation = CaculatetotalValutation(root->left);
	float rightValuation = CaculatetotalValutation(root->right);

	return rootValuation + leftValuation + rightValuation;
}

/* ================================================================ */
/* FUNCTION : CaculatetotalLoad( root)							    */
/* PURPOSE  : This function is to cumulative total  load of		    */
/*			  all the parcels.										*/
/* INPUTS   : root - the root of the tree                           */
/* RETURNS  : int - the total load					                */
/* ================================================================ */
int CaculatetotalLoad(Parcel* root)
{
	if (root == NULL)
	{
		return 0;
	}
	int rootLoad = root->weight;
	int totalLeft = CaculatetotalLoad(root->left);
	int totalRight = CaculatetotalLoad(root->right);
	return rootLoad + totalLeft + totalRight;
}

/* ================================================================ */
/* FUNCTION : showLower( root, newWeight)						    */
/* PURPOSE  : This function is to display all the parcel for given  */
/*			   country whose weight is lower than weight entered.	*/
/* INPUTS   : root - the root of the tree                           */
/*			  newWeight - user input weight							*/
/* RETURNS  : None									                */
/* ================================================================ */
void showLower(Parcel* root, int newWeight)
{
	if (root == NULL)
	{
		return;
	}

	if (root->weight < newWeight)
	{
		showNode(root);
		showWholeTree(root->left);
		showLower(root->right, newWeight);
	}
	else if (root->weight > newWeight)
	{
		showLower(root->left, newWeight);
	}
	else
	{
		showWholeTree(root->left);
	}
}

/* ================================================================ */
/* FUNCTION : showHigher( root, newWeight)						    */
/* PURPOSE  : This function is to display all the parcel for given  */
/*			   country whose weight is higher than weight entered.	*/
/* INPUTS   : root - the root of the tree                           */
/*			  newWeight - user input weight							*/
/* RETURNS  : None									                */
/* ================================================================ */
void showHigher(Parcel* root, int newWeight)
{
	if (root == NULL)
	{
		return;
	}

	if (root->weight < newWeight)
	{		
		showHigher(root->right, newWeight);
	}
	else if (root->weight > newWeight)
	{
		showNode(root);
		showWholeTree(root->right);
		showHigher(root->left, newWeight);
	}
	else
	{
		showWholeTree(root->right);
	}
}

/* ================================================================ */
/* FUNCTION : showNode( root)									    */
/* PURPOSE  : This function is to display the node					*/
/* INPUTS   : root - the root of the tree                           */
/* RETURNS  : None									                */
/* ================================================================ */
void showNode(Parcel * root)
{
	printf("Destination: %s, ", root->destination);
	printf("Weight: %dg, ", root->weight);
	printf("Value: $%f\n", root->value);
}

/* ================================================================ */
/* FUNCTION : showWholeTree( root)								    */
/* PURPOSE  : This function is to display the tree, pre-order		*/
/* INPUTS   : root - the root of the tree                           */
/* RETURNS  : None									                */
/* ================================================================ */
void showWholeTree(Parcel* root)
{
	if (root == NULL)
	{
		return;
	}
	showNode(root);

	showWholeTree(root->left);
	showWholeTree(root->right);
}

/* ================================================================ */
/* FUNCTION : insertTree( root, newNode)						    */
/* PURPOSE  : This function is to insert new node to the tree		*/
/* INPUTS   : root - the root of the tree                           */
/*			  newNode - the new node								*/
/* RETURNS  : Parcel* the pointer to the root			            */
/* ================================================================ */
Parcel* insertTree(Parcel* root, Parcel* newNode)
{
	// insert root
	if (root == NULL)
	{
		root = newNode;
		return root;
	}

	// insert left subtree
	if (root->weight > newNode->weight )
	{
		// important: assiged to the child
		root->left = insertTree(root->left, newNode);
		// important: don't return here, if you want the right tree to grow correctly
	}
	// insert right subtree
	if (root->weight < newNode->weight)
	{
		root->right = insertTree(root->right, newNode);		
	}
	return root;
}

/* ================================================================ */
/* FUNCTION : insertHashTable( countries, destination, weight,value)*/
/* PURPOSE  : This function is to creat new node and insert it to	*/
/*			  the tree in hastable									*/
/* INPUTS   : countries - the hash table							*/
/*			  destination - the country								*/
/*			  weight - the weight of the parcel						*/
/*			  value - the value of the parcel						*/
/* RETURNS  : None										            */
/* ================================================================ */
void insertHashTable(HashTable* countries, char* destination, int weight, float value)
{
	int hashNumber = 0;
	Parcel* newNode = creatNode(destination, weight, value);
	hashNumber = hashfunction(destination);

	// important to insert the root !!!
	if (countries->hashTable[hashNumber] == NULL)
	{
		countries->hashTable[hashNumber] = newNode;
		return;
	}

	Parcel* root = countries->hashTable[hashNumber];
	
	insertTree(root, newNode);

}

/* ================================================================ */
/* FUNCTION : creatNode( destination, weight,value)				    */
/* PURPOSE  : This function is to creat new node					*/
/* INPUTS   : destination - the country							    */
/*			  weight - the weight of the parcel						*/
/*			  value - the value of the parcel						*/
/* RETURNS  : Parcel* the pointer to the new node 		            */
/* ================================================================ */
Parcel* creatNode(char* destination, int weight, float value)
{
	Parcel* newNode = (Parcel*)malloc(sizeof(Parcel));
	if (newNode == NULL)
	{
		printf("Error of memory.\n");
		exit(EXIT_FAILURE);
	}

	newNode->destination = (char*)malloc(strlen(destination) + 1);
	if (newNode->destination == NULL)
	{
		printf("Error of memory.\n");
		exit(EXIT_FAILURE);
	}

	strcpy_s(newNode->destination, strlen(destination) + 1, destination);
	newNode->value = value;
	newNode->weight = weight;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

/* ================================================================ */
/* FUNCTION : initializeHashTable( void)						    */
/* PURPOSE  : This function is to initialize the HashTable			*/
/* INPUTS   : None												    */
/* RETURNS  : HashTable* the pointer to the HashTable 	            */
/* ================================================================ */
HashTable* initializeHashTable(void)
{
	HashTable* Hashtable = (HashTable * ) malloc(sizeof(HashTable));

	if (Hashtable == NULL)
	{
		printf("Error of memory.\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < BUCKETS_NUMBER; i++)
	{
		Hashtable->hashTable[i] = NULL;
	}
	return Hashtable;
}

/* ================================================================ */
/* FUNCTION : deleteRC( record)                                     */
/* PURPOSE  : This function is to delete the RC in the end of string*/
/* INPUTS   : record - the string                                   */
/* RETURNS  : None                                                  */
/* ================================================================ */
void deleteRC(char* record)
{
	char* whereRC = NULL;
	whereRC = strchr(record, '\n');
	if (whereRC)
	{
		*whereRC = '\0';
	}
}


/* ================================================================ */
/* FUNCTION : hashfunction( str)                                    */
/* PURPOSE  : use algorim to get a int between 0 and 127 from input */
/* INPUTS   : str - string to hash                                  */
/* RETURNS  : a int to use as index in a hashtable                  */
/* Author   : Dan Bernstein                                         */
/*            I got it from lecture                                 */
/* ================================================================ */
int hashfunction(char* str)
{
	unsigned long hash = 5381;
	int c;
	int result = 0;
	while (c = *str++)
	{
		hash = ((hash << 5) + hash) + c; // hash * 33 + c
	}
	result = (int)(hash % BUCKETS_NUMBER);
	return result;
}

/* ================================================================ */
/* FUNCTION : freeTree( root)										*/
/* PURPOSE  : free the memory that has been allocated				*/
/* INPUTS   : root - a pointer to the root of the tree			    */
/* RETURNS  : none									                */
/* ================================================================ */
void freeTree(Parcel* root)
{
	if (root == NULL)
	{
		return;
	}

	Parcel* left = root->left;
	Parcel* right = root->right;

	free(root->destination);
	free(root);

	freeTree(left);
	freeTree(right);
}