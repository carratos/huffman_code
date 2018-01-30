/*   Matt Vieyra
**   CS315 - Assignment 5
**   13 Nov 2016
**
**   Program takes frequency distribution of lexemes and creates a Huffman
**   code. Once user has finished entering character, an inorder and preorder
**   traversal is printed to terminal to confirm algorithm works.
*/
/* Include Statements */
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_MAX 10

/* definitions */
/*Structure that contains char lexeme and pointer to possible child nodes*/
typedef struct hufftree
{
  char lexeme;
  unsigned int lexeme_freq;      //Frequency is never less than 0
  struct hufftree *left, *right;
} HUFF_TREE;
/* Structure used to store treeItems */
typedef struct huffStack
{
  HUFF_TREE *treePtr;		/* Pointer to node in a tree */
  struct huffStack *nextTree; /* POINTS TO NEXT TREE IN STACK */
} HUFF_STACK;

/* Prototypes */
void huff_insertf(HUFF_TREE **, char);
void rpreorder (HUFF_TREE *);
void rinorder (HUFF_TREE *);
void constructf(HUFF_TREE **, HUFF_TREE **);

/* Stack Function Calls */
void push (HUFF_STACK **, HUFF_TREE *);
HUFF_STACK *pop (HUFF_STACK **);
int isEmpty (HUFF_STACK *);

/*Functions*/
int main ()
{
  char userChoice, findChar;
  HUFF_TREE *indexTree,		/* huff_tree element that tracks next empty position in array. */
  *mainTree = NULL;		/* points to root of binary tree */
  HUFF_TREE *heap_array[ARRAY_MAX];      /* Stores trees in almost perfect binary tree implemented in an array */
  if ((indexTree = malloc (sizeof (HUFF_TREE))) == NULL)	/* Checks if memory allocation successful */
    printf ("Unable to allocate memory, please try again\n");
  else
  {
    indexTree->lexeme = '@';      //initialize variables of indexTree
    indexTree->lexeme_freq = 1; // First element to be inserted in 1st element in array
    heap_array[0] = indexTree;
  }
  do
  {
    printf ("Enter a character (enter a '$' to quit entering characters): ");
    scanf (" %c", &userChoice);
    if (userChoice != '$')
      huff_insertf (heap_array, userChoice);
  }
  while (userChoice != '$');
  printf ("\nConstructing Huffman...\n");
  constructf (&mainTree, heap_array);
  printf ("The nodes of the Huffman tree in In-order are:\n");
  rinorder(mainTree);
  printf ("\n\nand in Pre-order are: \n");
  rpreorder(mainTree);
  printf ("\nProgram terminating...\n");
}
void huff_insertf (HUFF_TREE ** treeArray, char newLexeme)
{
  HUFF_TREE* newTree, //New tree element to be inserted into array
    *tempHolder; //Holds element to be swapped
  unsigned int childIndex, parentIndex, // Used for sorting
    greaterThanBool = 0; // Boolean value to check if child is greater than parent
  if (treeArray[0]->lexeme_freq == ARRAY_MAX)
    printf("Error: Tree is full see administrator about expanding tree limit/n");
  else //Tree limit not reached
  {
    if ((newTree = malloc (sizeof (HUFF_TREE))) == NULL)	/* Checks if memory allocation successful */
      printf ("Unable to allocate memory, please try again\n");
    else //Memory allocation successful
    {
        newTree->lexeme = newLexeme;
        printf ("Enter frequency for '%c': ", newTree->lexeme);
        scanf ("%d", &(newTree->lexeme_freq));
        treeArray[ treeArray[0]->lexeme_freq ] = newTree; //assign new tree to next available element
        treeArray[0]->lexeme_freq++; //increment index at 0th element in array
    }
  }
  // Sorting function for newly inserted element in array
  childIndex = (treeArray[0]->lexeme_freq) - 1;
  while (childIndex >= 2 && !greaterThanBool) //Don't swap with one element in array
  {
    parentIndex = childIndex/2;
    if ( (treeArray[childIndex]->lexeme_freq) < (treeArray[parentIndex]->lexeme_freq) )
    {
      tempHolder = treeArray[parentIndex]; //Store parent in temp
      treeArray[parentIndex] = treeArray[childIndex]; //swap child with parent
      treeArray[childIndex] = tempHolder; //store parent in childs place
      childIndex = parentIndex; //change childIndex to test its new parents freqency
    }
    else
      greaterThanBool = 1; //child larger or equal to parent stop comparison
  }
}
void constructf(HUFF_TREE** huff_root, HUFF_TREE** huff_array)
{
  unsigned int counter = 1;
  HUFF_TREE *temp_root; //tree created from two huffman trees removed from array

  while (counter < huff_array[0]->lexeme_freq)
  {
    if ( (temp_root = malloc (sizeof (HUFF_TREE))) == NULL)	/* Checks if memory allocation successful */
      printf ("Unable to allocate memory, please try again\n");
    else
    {
        temp_root->lexeme = '+';
        temp_root->lexeme_freq = (huff_array[counter]->lexeme_freq + huff_array[counter+1]->lexeme_freq);
        if ( huff_array[counter]->lexeme_freq < huff_array[counter+1]->lexeme_freq )
        {
          temp_root->left = huff_array[counter];
          temp_root->right = huff_array[counter+1];
          huff_array[counter+1] = temp_root; //New tree stored back in heap in second tree's place
        }
        else
        {
          temp_root->left = huff_array[counter+1];
          temp_root->right = huff_array[counter];
          huff_array[counter+1] = temp_root; //New tree stored back in heap in second tree's place
        }
        counter++;
    }
  }
  *huff_root = huff_array[ (huff_array[0]->lexeme_freq) - 1 ]; //final tree stored in last position in tree
}

/* Recursive Functions for Testing */
/*
 This function displays the tree in inorder fashion
*/
void rinorder (HUFF_TREE *temp)
{
  if (temp != NULL)
  {
    rinorder (temp->left);
    printf ("  %c\t%d\n", temp->lexeme, temp->lexeme_freq);
    rinorder (temp->right);
  }
}
/*
 This function displays the tree in preorder fashion
*/
void rpreorder (HUFF_TREE *temp)
{
  if (temp != NULL)
  {
    printf ("  %c\t%d\n", temp->lexeme, temp->lexeme_freq);
    rpreorder (temp->left);
    rpreorder (temp->right);
  }
}
