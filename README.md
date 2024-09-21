# AVL Tree Operations and Queries
#### This document explores the functionalities of AVL trees, detailing their operations and associated time complexities. AVL trees are a self-balancing binary search tree where the height difference between the left and right subtrees of any node is at most one, ensuring efficient operations.

### Operations

## 1) Insertion (x)
• Description: Adds a new node with key x to the AVL tree.

• Behavior: If a node with key x already exists, no changes are made.

• Time Complexity: 𝑂(log 𝑛), where n is the number of nodes in the tree.

## 2) Search (x)
• Description: Searches for a node with key x in the AVL tree.

• Returns: A pointer to the node with key x if it exists.

• Time Complexity: 𝑂(log 𝑛).

## 3) Delete (x)
• Description: Removes the node with key x from the AVL tree and releases the allocated memory.

• Behavior: No action is performed if a node with key x does not exist.

• Time Complexity: 𝑂(log⁡ 𝑛).

## 4) DeleteAll
• Description: Deletes all nodes in the AVL tree and releases the allocated memory.

• Time Complexity: 𝑂(𝑛), where n is the number of nodes in the tree.

## 5) Lowest Common Ancestor (y, x)
• Description: Finds the lowest common ancestor (LCA) of nodes with keys x and y.

• Returns: A pointer to the LCA of nodes with keys x and y. If one or both keys are missing, returns a pointer to the hypothetical LCA.

• Time Complexity: 𝑂(log 𝑛).

## 6) Range Sum (y, x)
• Description: Calculates the sum of all node keys within the inclusive range [x, y].

• Returns: The sum of keys within the specified range.

• Time Complexity: 𝑂(log⁡ 𝑛).

## 7) Merge (T1, T2)
• Description: Merges two AVL trees, T1 and T2, into a new AVL tree containing all the keys from both trees.

• Returns: A new AVL tree with the combined keys from T1 and T2.

• Time Complexity: 𝑂(𝑛), where n is the total number of nodes in both trees.

### Summary
## Time Complexity
• Insertion, Search, Lowest Common Ancestor, Range Sum: 𝑂(log 𝑛)

• DeleteAll, Merge: 𝑂(𝑛)
