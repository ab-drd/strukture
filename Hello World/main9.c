#include <stdio.h>

struct treeNode;
typedef struct treeNode TreeNode;
typedef struct treeNode* Tree;

struct treeNode 
{
	int element;
	Tree left;
	Tree right;
};

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

	while(!shouldIStop)
	{
		printf("Opcije");
	
	}

	Add(3, &root);
	Add(2, &root);
	Add(7, &root);
	Add(1, &root);
	Add(8, &root);

	printf("BST Minimum: %d\n", FindMinimum(&root)->element);

	Print(&root);

	return 0;
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
		currentNode->left = Delete(value, currentNode);
	}
	else if(value > currentNode->element) 
	{
		currentNode->right = Delete(value, currentNode);
	}
	else 
	{
		if(currentNode->left == NULL) 
		{
			Tree returnNode = currentNode->right;
			return returnNode;
		}
		else if(currentNode->right == NULL)
		{
			Tree returnNode = currentNode->left;
			return returnNode;
		}

		Tree temporaryNode = FindMinimum(currentNode->right);

		currentNode->element = temporaryNode->element;

		currentNode->right = Delete(temporaryNode->element, currentNode->right);
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
		printf(" %d", currentNode->element);
		Print(currentNode->right);
	}
}