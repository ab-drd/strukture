#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct treeNode;
typedef struct treeNode TreeNode;
typedef struct treeNode* Tree;

struct treeNode 
{
	int element;
	Tree left;
	Tree right;
};

int ValidIntInputAndCheck(int, int);

Tree ClearTree(Tree);
Tree Add(int, Tree);
Tree Delete(int, Tree);
Tree FindValue(int, Tree);
Tree FindMinimum(Tree);

void Print(Tree);

int main()
{
	int shouldIStop = 0;

	TreeNode root;
	root.left = NULL;
	root.right = NULL;

	int rootValue = 0;
	printf("Set root value of BST:\n");
	scanf("%d", &rootValue);

	root.element = rootValue;
	system("cls");
	while(!shouldIStop)
	{
		printf("Actions:\n<1>   Add desired element\n<2>   Print BST\n<3>   Find element of chosen value\n<4>   Delete desired element\n<0>   Exit\n");
		int actionChoice = ValidIntInputAndCheck(0, 4);

		switch(actionChoice)
		{
			case 1:
				printf("\nInput the number you want to add to your BST:\n");

				int inputInteger = 0;
				scanf("%d", &inputInteger);

				Add(inputInteger, &root);
				break;

			case 2:
				puts("");
				Print(&root);
				puts("");
				break;

			case 3:
				printf("\nInput the number you want to find in your BST:\n");

				int findInteger = 0;
				scanf("%d", &findInteger);

				printf("\nNumber is on (%d)", FindValue(findInteger, &root));

				break;

			case 4:
				printf("\nInput the number you want to delete from your BST:\n");

				int deleteInteger = 0;
				scanf("%d", &deleteInteger);

				Delete(deleteInteger, &root);
				break;

			case 0:
				shouldIStop = 1;
				break;

			default:
				printf("\nInvalid input\n");
				break;
		}

		printf("Press any key to continue...");
		getch();
		system("cls");
	}

	return 0;
}

int ValidIntInputAndCheck(int range1, int range2)
{
	int someInteger = 0;
	scanf("%d", &someInteger);

	while (someInteger < range1 || someInteger > range2) {
		printf("\nNumber is out of range %d - %d\nRepeat input:\n", range1, range2);
		scanf("%d", &someInteger);
	}

	return someInteger;
}

Tree ClearTree(Tree currentNode) 
{
	if (currentNode == NULL)
	{
		return NULL;
	}

	ClearTree(currentNode->left);
	ClearTree(currentNode->right);

	free(currentNode);

	return NULL;
}

Tree Add(int value, Tree currentNode)
{
	if(currentNode == NULL)
	{
		currentNode = (Tree)malloc(sizeof(TreeNode));

		currentNode->element = value;
		currentNode->left = NULL;
		currentNode->right = NULL;

		printf("Value %d has been added\n", value);
	}
	else if (value < currentNode->element)
	{
		currentNode->left = Add(value, currentNode->left);
	}
	else if (value > currentNode->element)
	{
		currentNode->right = Add(value, currentNode->right);
	}
	else 
	{
		printf("ERROR - Could not add value because value is already in BST");
		return currentNode;
	}

	return currentNode;
}

Tree Delete(int value, Tree currentNode)
{
	if(currentNode == NULL)
	{
		return currentNode;
	}

	if(value < currentNode->element) 
	{
		currentNode->left = Delete(value, currentNode->left);
	}
	else if(value > currentNode->element) 
	{
		currentNode->right = Delete(value, currentNode->right);
	}
	else 
	{
		if(currentNode->left == NULL) 
		{
			Tree returnNode = currentNode->right;
			free(currentNode);
			return returnNode;
		}
		else if(currentNode->right == NULL)
		{
			Tree returnNode = currentNode->left;
			free(currentNode);
			return returnNode;
		}

		Tree temporaryNode = FindMinimum(currentNode->right);

		currentNode->element = temporaryNode->element;

		currentNode->right = Delete(currentNode->element, currentNode->right);
	}

	return currentNode;
}

Tree FindValue(int value, Tree currentNode)
{
	if (currentNode == NULL)
	{
		return NULL;
	}
	else if(value == currentNode->element)
	{
		return currentNode;
	}
	else if(value < currentNode->element)
	{
		return FindValue(value, currentNode->left);
	}
	else 
	{
		return FindValue(value, currentNode->right);
	}

}

Tree FindMinimum(Tree currentNode)
{
	if(currentNode == NULL)
	{
		return NULL;
	}
	
	if(currentNode->left == NULL) 
	{
		return currentNode;
	}
	else 
	{
		return FindMinimum(currentNode->left);
	}
}

void Print(Tree currentNode)
{
	if(currentNode != NULL)
	{
		Print(currentNode->left);
		printf("%d ", currentNode->element);
		Print(currentNode->right);
	}
}