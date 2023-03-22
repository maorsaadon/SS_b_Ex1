#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "AdptArray.h"

// Define the AdptArray struct
typedef struct AdptArray_
{
	int ArrSize; // The current size of the array
	PElement* pElemArr; // An array of pointers to elements
	DEL_FUNC delFunc; // A function to delete an element
	COPY_FUNC copyFunc; // A function to copy an element
    PRINT_FUNC printFunc; // A function to print an element
}AdptArray;

// Create a new AdptArray
PAdptArray CreateAdptArray(COPY_FUNC copyFunc_, DEL_FUNC delFunc_, PRINT_FUNC printFunc_)
{
	// Allocate memory for the AdptArray struct
    PAdptArray pArr = (PAdptArray)malloc(sizeof(AdptArray));
	if (pArr == NULL) return NULL;
	// Initialize the fields of the AdptArray struct
    pArr->ArrSize = 0;
	pArr->pElemArr = NULL;
	pArr->delFunc = delFunc_;
	pArr->copyFunc = copyFunc_;
    pArr->printFunc = printFunc_;
	return pArr;
}

// Set an element at a given index in the AdptArray
Result SetAdptArrayAt(PAdptArray pArr, int idx, PElement pNewElem)
{
	if (idx < 0) return FAIL;
    PElement* newpElemArr;
    // Check if the AdptArray is NULL
    if (pArr == NULL) return FAIL;
    // If the index is larger than the current
	if (idx >= pArr->ArrSize)
	{
		// If the index is beyond the end of the array, we need to resize the array
		if ((newpElemArr = (PElement*)calloc((idx + 1), sizeof(PElement))) == NULL)
			return FAIL;
		// Copy the existing elements to the new array
        for(int i = 0; i < pArr->ArrSize; ++i)
            if((pArr->pElemArr)[i] != NULL)
                newpElemArr[i] = pArr->copyFunc((pArr->pElemArr)[i]);
		
        // Free the old array and set the new array
        free(pArr->pElemArr);
		pArr->pElemArr = newpElemArr;
	}
    else
    {
	    // If the index is within the current size of the array, delete the existing element at that index
        if((pArr->pElemArr)[idx] != NULL)
            pArr->delFunc((pArr->pElemArr)[idx]);
    }
	
    // Set the new element at the given index
    (pArr->pElemArr)[idx] = pArr->copyFunc(pNewElem);

    // Update the size of the array
	pArr->ArrSize = (idx >= pArr->ArrSize) ? (idx + 1) : pArr->ArrSize;
	return SUCCESS;
}

// This function deletes an adaptive array and all its elements
void DeleteAdptArray(PAdptArray pArr)
{
	// Check if the array is null
    if (pArr == NULL) return;
    // Delete each element in the array
	for(int i = 0; i < pArr->ArrSize; ++i)
		if((pArr->pElemArr)[i] != NULL)
            // Call the element's delete function
            pArr->delFunc((pArr->pElemArr)[i]);

	// Free the memory allocated for the array and the array structure
    free(pArr->pElemArr);
	free(pArr);
}

// This function returns a copy of the element at the specified index in the adaptive array
PElement GetAdptArrayAt(PAdptArray pArr, int idx){
    
    // Returns NULL if the array is null, index is out of range or the element at the index is null
    if(pArr == NULL || idx <0 || idx > pArr->ArrSize || (pArr->pElemArr)[idx] == NULL)
       return NULL;
    // Return a copy of the element using the array's copy function
    return pArr->copyFunc((pArr->pElemArr)[idx]); 

}

// This function returns the size of the adaptive array
int GetAdptArraySize(PAdptArray pArr){
    // Returns -1 if the array is null
    if (pArr == NULL) return -1;
    return pArr->ArrSize;

}

// This function prints all the elements in the adaptive array
void PrintDB(PAdptArray pArr){
    for(int i = 0; i < pArr->ArrSize; ++i)
		if((pArr->pElemArr)[i] != NULL)
        // Call the print function of each element
            pArr->printFunc((pArr->pElemArr)[i]);
}