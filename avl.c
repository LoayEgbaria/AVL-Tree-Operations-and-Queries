#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

/****
  Student1 name: Loai Saadi
  Student2 name: Loay Egbaria

  Student1 ID: *********
  Student2 ID: *********
****/

// a helper functions that we used in the operations and queries 
int CalHeight(AVLNodePtr root);
int CalSize(AVLNodePtr root);
int CalSumTree(AVLNodePtr root);
AVLNodePtr FindMin(AVLNodePtr root);
AVLNodePtr rightRotate(AVLNodePtr y);
AVLNodePtr leftRotate(AVLNodePtr x);
int getBalance(AVLNodePtr root);
int* merge(int arr1[], int arr2[], int m, int n);
void storeInorder(AVLNodePtr root, int inorder[], int* index_ptr);
AVLNodePtr sortedArrayToAVL(int arr[], int start, int end);
AVLNodePtr CreateSingleAVL(AVLNodePtr root1, AVLNodePtr root2);
int removeDuplicates(int arr[], int n);


// Return the maximum between x and y
int util_max(int x, int y) {
    return (x < y) ? y : x;
}

// Return the POSITIVE value of x 
int util_abs(int x) {
    return (x < 0) ? -x : x;
}

// Seacrh a node
// Complexity: O(log n)
AVLNodePtr avl_search(AVLNodePtr root, int x)
{
    if (root == NULL)
        return NULL;
    if (x == root->key) {
        return root;
    }
    else if (x < root->key) {
        return avl_search(root->child[LEFT], x);
    }
    else {
        return avl_search(root->child[RIGHT], x);
    }
}

// Insert node
// Complexity: O(log n)
AVLNodePtr avl_insert(AVLNodePtr root, int x)
{
    // Find the correct position to insert the node and insert it
    if (root == NULL)
        return (new_avl_node(x));
    if (x < root->key)
        root->child[LEFT] = avl_insert(root->child[LEFT], x);
    else if (x > root->key)
        root->child[RIGHT] = avl_insert(root->child[RIGHT], x);
    else if (x == root->key)
        return root;
    else
        return root;
    // Update the balance factor of each node and
    // Balance the tree
    root->height = 1 + util_max(CalHeight(root->child[LEFT]), CalHeight(root->child[RIGHT]));
    root->tree_size = root->tree_size + 1;
    root->tree_sum = root->key + CalSumTree(root->child[LEFT]) + CalSumTree(root->child[RIGHT]);

    int balance = getBalance(root);
    if (balance > 1 && x < root->child[LEFT]->key)
        return rightRotate(root);
    if (balance < -1 && x > root->child[RIGHT]->key)
        return leftRotate(root);
    if (balance > 1 && x > root->child[LEFT]->key) {
        root->child[LEFT] = leftRotate(root->child[LEFT]);
        return rightRotate(root);
    }
    if (balance < -1 && x < root->child[RIGHT]->key) {
        root->child[RIGHT] = rightRotate(root->child[RIGHT]);
        return leftRotate(root);
    }
    return root;
}

// Delete a node
// Complexity: O(log n)
AVLNodePtr avl_delete(AVLNodePtr root, int x)
{
    // Find the node and delete it
    if (root == NULL)
        return root;
    if (x < root->key)
        root->child[LEFT] = avl_delete(root->child[LEFT], x);
    else if (x > root->key)
        root->child[RIGHT] = avl_delete(root->child[RIGHT], x);
    else {
        if ((root->child[LEFT] == NULL) || (root->child[RIGHT] == NULL)) {
            root->tree_size -= 1;

            AVLNodePtr temp = root->child[LEFT] ? root->child[LEFT] : root->child[RIGHT];
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else {
            root->tree_size -= 1;
            AVLNodePtr temp = FindMin(root->child[RIGHT]); // change it with the successor of the node
            root->key = temp->key;
            root->child[RIGHT] = avl_delete(root->child[RIGHT], temp->key);
        }
    }
    if (root == NULL)
        return root;
    // Update the balance factor of each node and
    // balance the tree
    root->height = 1 + util_max(CalHeight(root->child[LEFT]), CalHeight(root->child[RIGHT]));
    root->tree_size = CalSize(root->child[LEFT]) + CalSize(root->child[RIGHT]) + 1;
    root->tree_sum = root->key + CalSumTree(root->child[LEFT]) + CalSumTree(root->child[RIGHT]);

    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->child[LEFT]) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->child[LEFT]) < 0) {
        root->child[LEFT] = leftRotate(root->child[LEFT]);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->child[RIGHT]) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->child[RIGHT]) > 0) {
        root->child[RIGHT] = rightRotate(root->child[RIGHT]);
        return leftRotate(root);
    }

    return root;
}

// a helper function that calculate the height of root
// Complexity: O(1)
int CalHeight(AVLNodePtr root)
{
    if (root == NULL)
        return -1;
    return root->height;
}

// a helper function that Calculate the size of root
// Complexity: O(1)
int CalSize(AVLNodePtr root)
{
    if (root == NULL)
        return 0;
    return root->tree_size;
}

// a helper function that calculate the sum of root
// Complexity: O(1)
int CalSumTree(AVLNodePtr root)
{
    if (root == NULL)
        return 0;
    return root->tree_sum;
}

// Right rotate
// Complexity: O(1)
AVLNodePtr rightRotate(AVLNodePtr y)
{
    AVLNodePtr x = y->child[LEFT];
    AVLNodePtr T2 = x->child[RIGHT];
    x->child[RIGHT] = y;
    y->child[LEFT] = T2;
    y->height = util_max(CalHeight(y->child[LEFT]), CalHeight(y->child[RIGHT])) + 1;
    x->height = util_max(CalHeight(x->child[LEFT]), CalHeight(x->child[RIGHT])) + 1;
    y->tree_size = CalSize(y->child[LEFT]) + CalSize(y->child[RIGHT]) + 1;
    x->tree_size = CalSize(x->child[LEFT]) + CalSize(x->child[RIGHT]) + 1;
    y->tree_sum = y->key + CalSumTree(y->child[LEFT]) + CalSumTree(y->child[RIGHT]);
    x->tree_sum = x->key + CalSumTree(x->child[LEFT]) + CalSumTree(x->child[RIGHT]);
    return x;
}

// Left rotate
// Complexity: O(1)
AVLNodePtr leftRotate(AVLNodePtr x)
{
    AVLNodePtr y = x->child[RIGHT];
    AVLNodePtr T2 = y->child[LEFT];
    y->child[LEFT] = x;
    x->child[RIGHT] = T2;
    x->height = util_max(CalHeight(x->child[LEFT]), CalHeight(x->child[RIGHT])) + 1;
    y->height = util_max(CalHeight(y->child[LEFT]), CalHeight(y->child[RIGHT])) + 1;
    x->tree_size = CalSize(x->child[LEFT]) + CalSize(x->child[RIGHT]) + 1;
    y->tree_size = CalSize(y->child[LEFT]) + CalSize(y->child[RIGHT]) + 1;
    x->tree_sum = x->key + CalSumTree(x->child[LEFT]) + CalSumTree(x->child[RIGHT]);
    y->tree_sum = y->key + CalSumTree(y->child[LEFT]) + CalSumTree(y->child[RIGHT]);
    return y;
}

// a helper function that find Min in that subtree, return pointer to that node
// Complexity: O(log n)
AVLNodePtr FindMin(AVLNodePtr root)
{
    AVLNodePtr min = root;
    while (min->child[LEFT] != NULL)
        min = min->child[LEFT];
    return min;
}

// a helper function that get the balance between left and right child
// Complexity: O(1)
int getBalance(AVLNodePtr root)
{
    if (root == NULL)
        return 0;
    return (CalHeight(root->child[LEFT]) - CalHeight(root->child[RIGHT]));
}

// avl_LCA returns a pointer to the lowest common ancestor of x and y.
// Complexity: O(log n)
AVLNodePtr avl_LCA(AVLNodePtr root, int x, int y)
{
    if (root == NULL)
        return NULL;
    // We check if x is on the route of y, or the opposite: then we return root itself
    if ((x == root->key) || (y == root->key))
        return root;
    // We check if (x < root.key < y): then we did successfully found the LCA, so we return it
    if ((x < root->key) && (root->key < y))
        return root;

    // now, we will go on the route of x (we suppose that x is in the tree): then we call recursivley on every node on that route 
    if (x < root->key)
        return avl_LCA(root->child[LEFT], x, y);
    if (x > root->key)
        return avl_LCA(root->child[RIGHT], x, y);
}

// Create new node
// Complexity: O(1)
AVLNodePtr new_avl_node(int x)
{
    AVLNodePtr xNode;
    if ((xNode = (AVLNodePtr)malloc(sizeof(AVLNode))) != NULL)
    {
        xNode->child[LEFT] = xNode->child[RIGHT] = NULL;
        xNode->height = 0;
        xNode->tree_size = 1;
        xNode->key = xNode->tree_sum = x;
    }
    return xNode;
}

// Delete all the tree
// Complexity: O(n)
void avl_delete_all(AVLNodePtr root)
{
    if (root == NULL)
        return;
    avl_delete_all(root->child[LEFT]);
    avl_delete_all(root->child[RIGHT]);
    free(root);
}

// avl_sum returns the sum of the keys in the tree, which are in [x,y].
// Complexity: O(log n)
int avl_sum(AVLNodePtr root, int x, int y)
{
    if (root == NULL)
        return 0;
    AVLNodePtr LCA = avl_LCA(root, x, y);
    if (LCA == NULL)
        return 0;
    AVLNodePtr node = LCA;
    int flag = 1;
    int sum = 0;
    int yFlag = 0;
    int xFlag = 0;

    // here, we want to check if they x equal to y:
    if (x == y)
    {
        if (avl_search(root, x))
            return x;
        else
            return 0;
    }

    // bsaically, we will sum the keys of the nodes from the "node" (which equals to the LCA) to x, and from the "node" to y:

    // here, we will sum the keys of the nodes that do exist in the route from the LCA to x (with x and the key of LCA):
    while (flag == 1)
    {
        if (node != NULL)
            if (node->key == y)
                yFlag = 1;
        if (node == NULL)
            flag = 0;
        else if (node->key == x)
        {
            if (node != LCA)
            {
                if (x < y)
                    sum = sum + x + CalSumTree(node->child[RIGHT]);
                else
                    sum = sum + x + CalSumTree(node->child[LEFT]);
            }
            else
                sum = sum + x;
            flag = 0;
        }
        else if (node->key < x)
        {
            node = node->child[RIGHT];
        }
        else if (node->key > x)
        {
            if (node != LCA)
                sum = sum + node->key + CalSumTree(node->child[RIGHT]);
            else
                sum = sum + node->key;
            node = node->child[LEFT];
        }

    }

    // we want to check if yFlag is on: if yes that means we have visited the route from the LCA to y in our way to x, just return SUM, if no, continue
    // also we want to check that x<y so the numbers are flipped:
    if (yFlag == 1 && x < y)
        return sum;

    node = LCA;
    flag = 1;

    // here, we will sum the keys of the nodes that do exist in the route from the LCA to y (with y and the key of LCA):
    while (flag == 1)
    {
        if (node != NULL)
            if (node->key == x)
                xFlag = 1;
        if (node == NULL)
        {
            flag = 0;
        }

        else if (node->key == y)
        {
            if (node != LCA)
            {
                if (y < x)
                    sum = sum + y + CalSumTree(node->child[RIGHT]);
                else
                    sum = sum + y + CalSumTree(node->child[LEFT]);
            }
            else
                sum = sum + y;
            flag = 0;
        }
        else if (node->key < y)
        {
            if (node != LCA)
                sum = sum + node->key + CalSumTree(node->child[LEFT]);
            else
                sum = sum + node->key;
            node = node->child[RIGHT];
        }
        else if (node->key > y)
        {
            node = node->child[LEFT];
        }
    }

    // we want to check if xFlag is on: if yes that means we have visited the route from the LCA to x in our way to y, just return SUM, if no, continue
    // also we want to check that x>y so the numbers are flipped:
    if (xFlag == 1 && y < x)
        return sum;

    // now, we delete from the sum, the key of the LCA. why? bcz we have summed it twice: once in the route from the LCA to x, and another in the route from the LCA to y
    sum = sum - LCA->key;
    return sum;
}

// avl_merge merges the 2 trees into an avl tree.
// Returns a pointer to the root of the newly formed tree.
// Complexity: O(n)
AVLNodePtr avl_merge(AVLNodePtr root1, AVLNodePtr root2)
{
    // first, we want to check if both roots are NULL then we return NULL
    // or one of them is NULL then we build an AVL tree from the other tree (we build a new AVL tree):
    if (root1 == NULL && root2 == NULL)
        return NULL;

    if (root1 == NULL)
        return CreateSingleAVL(root2, root1);

    if (root2 == NULL)
        return CreateSingleAVL(root1, root2);
    // Store inorder traversal of  
    // first tree in an array arr1[] 



    int m = root1->tree_size;
    int* arr1 = (int*)malloc(m * sizeof(int));
    int i = 0;
    storeInorder(root1, arr1, &i);

    // Store inorder traversal of second 
    // tree in another array arr2[]  

    int n = root2->tree_size;
    int* arr2 = (int*)malloc(n * sizeof(int));
    int j = 0;
    storeInorder(root2, arr2, &j);

    // Merge the two sorted array into one  
    int* mergedArr = merge(arr1, arr2, m, n);

    // Free space 
    free(arr1);
    free(arr2);

    // Merge the two arrays into one array
    int MergedArrNewSize = removeDuplicates(mergedArr, m + n);

    // Construct a tree from the merged array and save it in res
    AVLNodePtr res = sortedArrayToAVL(mergedArr, 0, MergedArrNewSize - 1);

    // Free more space
    free(mergedArr);

    return res;
}

// a helper function that merge two sorted arrays into one  
// Complexity: O(m+n), which (m+n) equals to the size of both arrays
int* merge(int arr1[], int arr2[], int m, int n)
{
    // mergedArr[] is going to contain result  
    int* mergedArr = (int*)malloc((m + n) * sizeof(int));
    int i = 0, j = 0, k = 0;

    // Traverse through both arrays  
    while (i < m && j < n)
    {
        // Pick the smaler element and put it in mergedArr  
        if (arr1[i] < arr2[j])
        {
            mergedArr[k] = arr1[i];
            i++;
        }
        else
        {
            mergedArr[k] = arr2[j];
            j++;
        }
        k++;
    }

    // If there are more elements in first array  
    while (i < m)
    {
        mergedArr[k] = arr1[i];
        i++; k++;
    }

    // If there are more elements in second array  
    while (j < n)
    {
        mergedArr[k] = arr2[j];
        j++; k++;
    }

    return mergedArr;
}

// a helper function that stores inorder traversal of a tree rooted with node
// Complexity: O(n)
void storeInorder(AVLNodePtr root, int inorder[], int* index_ptr)
{
    if (root == NULL)
        return;

    /* first recur on left child */
    storeInorder(root->child[LEFT], inorder, index_ptr);

    inorder[*index_ptr] = root->key;
    (*index_ptr)++; // increase index for next entry  

    /* now recur on right child */
    storeInorder(root->child[RIGHT], inorder, index_ptr);
}

// A function that constructs AVL Tree from a sorted array
// Complexity: O(n)
AVLNodePtr sortedArrayToAVL(int arr[], int start, int end)
{
    /* Base Case */
    if (start > end)
        return NULL;

    /* Get the middle element and make it root */
    int mid = (start + end) / 2;
    AVLNodePtr root = new_avl_node(arr[mid]);

    /* Recursively construct the left subtree and make it
    left child of root */
    root->child[LEFT] = sortedArrayToAVL(arr, start, mid - 1);

    /* Recursively construct the right subtree and make it
    right child of root */
    root->child[RIGHT] = sortedArrayToAVL(arr, mid + 1, end);

    /* now, we will modify the root height, size and sum */
    root->height = util_max(CalHeight(root->child[LEFT]), CalHeight(root->child[RIGHT])) + 1;
    root->tree_size = CalSize(root->child[LEFT]) + CalSize(root->child[RIGHT]) + 1;
    root->tree_sum = root->key + CalSumTree(root->child[LEFT]) + CalSumTree(root->child[RIGHT]);
    return root;
}

// a helper function that copies the nodes from root1 to root2
// notice that root2 is NULL
// Complexity: O(n) as n equals to the size of root1
AVLNodePtr CreateSingleAVL(AVLNodePtr root1, AVLNodePtr root2)
{
    if (root1 != NULL)
    {
        root2 = new_avl_node(root1->key);
        root2->height = root1->height;
        root2->tree_size = root1->tree_size;
        root2->tree_sum = root1->tree_sum;
        root2->child[LEFT] = CreateSingleAVL(root1->child[LEFT], root2->child[LEFT]);
        root2->child[RIGHT] = CreateSingleAVL(root1->child[RIGHT], root2->child[RIGHT]);
        return root2;
    }
    else
        return NULL;
}

// a helper function that remove the duplicated elements in a sorted array, and return the new size of it. 
// Complexity: O(n) time and space
int removeDuplicates(int arr[], int n)
{
    // Return, if array is empty or contains a single
    // element
    if (n == 0 || n == 1)
        return n;

    int* temp = (int*)malloc(n * sizeof(int));

    // Start traversing elements
    int j = 0;
    // If current element is not equal to next element
    // then store that current element
    for (int i = 0; i < n - 1; i++)
        if (arr[i] != arr[i + 1])
            temp[j++] = arr[i];

    // Store the last element as whether it is unique or
    // repeated, it hasn't stored previously
    temp[j++] = arr[n - 1];

    // Modify original array
    for (int i = 0; i < j; i++)
        arr[i] = temp[i];
    free(temp);
    return j;
}