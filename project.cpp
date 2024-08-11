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
float findCheapest(Parcel* root, float cheapest); 
float findExpensive(Parcel* root, float expensive);
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
	if (fopen_s(&ptr, "./countries.txt", "r") != 0)
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
	
	//If the loop keeps repeating after input, it’s likely due to scanf_s left a '\n' in the input buffer, then it read by another scanf_s, then problem comes.
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
			printf("Total load is %d.\n", CaculatetotalLoad(countries->hashTable[hashfunction(countryName)]));
			printf("Total valuation is %f.\n", CaculatetotalValutation(countries->hashTable[hashfunction(countryName)]));
			break;
		case 4:
			printf("Please enter country name: ");
			fgets(countryName, BUFFER_LENGTH, stdin);
			deleteRC(countryName);
			printf("\n");
			printf("The most cheapest value is %f.\n", findCheapest(countries->hashTable[hashfunction(countryName)], countries->hashTable[hashfunction(countryName)]->value));
			printf("The most expensive value is %f.\n", findExpensive(countries->hashTable[hashfunction(countryName)], countries->hashTable[hashfunction(countryName)]->value));
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
			break;
		}
		printf("\n");
	}

	//test:
	//showWholeTree(countries->hashTable[0]);
	// 
	//showLower(countries->hashTable[0], 5132);

	//int load = CaculatetotalLoad(countries->hashTable[0]);
	//float totalvalue = CaculatetotalValutation(countries->hashTable[0]);
	//printf("%d, %f", load, totalvalue);

	//printf("%f\n", findCheapest(countries->hashTable[0], 665.14));

	//findLightest(countries->hashTable[0]);


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
/* FUNCTION : findCheapest( root, cheapest)                         */
/* PURPOSE  : This function is to display cheapest parcel’s details */
/* INPUTS   : root - the root of the tree                           */
/*			  cheapest - the cheapest value so far					*/	
/* RETURNS  : float - the cheapest value so far                     */
/* ================================================================ */
float findCheapest(Parcel* root, float cheapest)
{
	if (root == NULL)
	{
		return cheapest;
	}

	if (root->value < cheapest)
	{
		cheapest = root->value;
	}

	float leftCheapest = findCheapest(root->left, cheapest);
	float rightCheapest = findCheapest(root->right, cheapest);

	if (cheapest <= leftCheapest &&
		cheapest <= rightCheapest)
	{
		return  cheapest;
	}
	else if (leftCheapest <= rightCheapest &&
			 leftCheapest <= cheapest)
	{
		return  leftCheapest;
	}
	else if (rightCheapest <= leftCheapest &&
		rightCheapest <= cheapest)
	{
		return  rightCheapest;
	}
}

/* ================================================================ */
/* FUNCTION : findExpensive( root, expensive)                        */
/* PURPOSE  : This function is to display most expensive parcel     */
/* INPUTS   : root - the root of the tree                           */
/*			  expensive - the most expensive value so far			*/
/* RETURNS  : float - the expensive value so far                    */
/* ================================================================ */
float findExpensive(Parcel* root, float expensive)
{
	if (root == NULL)
	{
		return expensive;
	}

	if (root->value > expensive)
	{
		expensive = root->value;
	}

	float leftExpensive = findExpensive(root->left, expensive);
	float rightExpensive = findExpensive(root->right, expensive);

	if (expensive >= leftExpensive &&
		expensive >= rightExpensive)
	{
		return  expensive;
	}
	else if (leftExpensive >= rightExpensive &&
		leftExpensive >= expensive)
	{
		return  leftExpensive;
	}
	else if (rightExpensive >= leftExpensive &&
		rightExpensive >= expensive)
	{
		return  rightExpensive;
	}
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
	float valuation = 0.0;
	if (root == NULL)
	{
		return 0;
	}
	valuation += root->value;
	valuation += CaculatetotalLoad(root->left);
	valuation += CaculatetotalLoad(root->right);
	return valuation;
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
	int totalLoad = 0;
	
	if (root == NULL)
	{
		return 0;
	}
	totalLoad += root->weight;
	totalLoad += CaculatetotalLoad(root->left);
	totalLoad += CaculatetotalLoad(root->right);
	return totalLoad;
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
/* INPUTS   : root - the root of the tree                      