#!/usr/bin/python
class TreeNode:

    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None

    def traverse_recursive(self, treenode):

        if treenode == None:
            return
        print treenode.val
        self.traverse_recursive(treenode.left)
        self.traverse_recursive(treenode.right)

    def traverse(self):
        self.traverse_recursive(self)
