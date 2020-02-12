#!/usr/bin/python

from tree_node import TreeNode

class Solution:

    def traverse_tree(self, pRoot1, pRoot2):

        if pRoot2 == None:
            return True
        elif pRoot1 == None:
            return False

        if pRoot1.val == pRoot2.val:
            return self.traverse_tree(pRoot1.left, pRoot2.left) and \
                self.traverse_tree(pRoot1.right, pRoot2.right)
        else:
            return False

    def HasSubtree(self, pRoot1, pRoot2):

        if pRoot1 == None or pRoot2 == None:
            return False

        return self.traverse_tree(pRoot1, pRoot2) or self.HasSubtree(pRoot1.left, pRoot2)  \
            or self.HasSubtree(pRoot1.right, pRoot2)

t1 = TreeNode(1)
t2 = TreeNode(2)
t3 = TreeNode(3)
t4 = TreeNode(4)
t5 = TreeNode(5)

t1.left = t2
t1.right = t3
t2.left = t4
t2.right = t5

n1 = TreeNode(1)
n2 = TreeNode(2)
n3 = TreeNode(3)

n1.left = n2
n1.right = n3

print Solution().HasSubtree(t1, n1)
