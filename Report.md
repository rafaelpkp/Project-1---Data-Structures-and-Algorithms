# Project 1: Gator AVL — Report

**Name:** Rafael Penhas

## 1. Time Complexity (worst case)

Variables:
- **n** = number of nodes (students) currently in the tree.
- **k** = number of students whose name matches the searched name (k ≤ n).

Because the tree is an AVL tree, its height is always O(log n).

| Command | Method(s) called | Worst case | Justification |
|---|---|---|---|
| `insert NAME ID` | `isValidName`, `isValidID`, `AVL::insert` → `insertHelper`, `rebalance` | **O(log n)** | Validating the name is O(length of name) which is bounded by the command length (≤ 1000), so it is constant with respect to n. `insertHelper` walks one root-to-leaf path of height O(log n). On the way back up, each node on that path updates its height and possibly rotates, and each rotation is O(1). |
| `remove ID` | `AVL::remove` → `removeHelper`, `rebalance` | **O(log n)** | Finding the node follows one path of height O(log n). In the two-children case, finding the inorder successor goes down the right subtree, and removing it continues on the same path, so it is still O(log n) total. Height updates/rotations on the way back up are O(1) per node. |
| `search ID` | `AVL::searchID` → `findID` | **O(log n)** | Standard BST search: one comparison per level, and the height is O(log n). |
| `search NAME` | `AVL::searchName` → `searchNameHelper`, then formatting output | **O(n)** | The tree is sorted by ID, not name, so every node must be visited in a preorder traversal. Building the output string is O(k), and k ≤ n. |
| `printInorder` | `AVL::inorderNames` → `inorderHelper`, `namesOf`, `joinNames` | **O(n)** | Each node is visited exactly once, then each name is copied into a vector and joined into one string, each O(n) (names have bounded length). |
| `printPreorder` | `AVL::preorderNames` → `preorderHelper` | **O(n)** | Same reasoning as inorder. |
| `printPostorder` | `AVL::postorderNames` → `postorderHelper` | **O(n)** | Same reasoning as inorder. |
| `printLevelCount` | `AVL::levelCount` | **O(1)** | Each node stores its height, so the level count is just the root's stored height. |
| `removeInorder N` | `AVL::removeInorder` → `inorderHelper`, `remove` | **O(n)** | Checking N against the stored node count is O(1). Then a full inorder traversal O(n) collects the nodes to find the Nth one, followed by a `remove` which is O(log n). O(n) + O(log n) = O(n). |

## 2. Reflection

_TODO (write this yourself): What did you learn from this assignment, and what would you do differently if you started over?_
