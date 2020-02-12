from tree_node import TreeNode

class Solution:

    def Mirror(self, root):

        if root == None:
            return

        self.Mirror(root.left)
        self.Mirror(root.right)
        tmp = root.right
        root.right = root.left
        root.left = tmp

t1 = TreeNode(1)
t2 = TreeNode(2)
t3 = TreeNode(3)
t4 = TreeNode(4)
t5 = TreeNode(5)

t1.left = t2
t1.right = t3
t2.left = t4
t2.right = t5

Solution().Mirror(t1)

t1.traverse()
