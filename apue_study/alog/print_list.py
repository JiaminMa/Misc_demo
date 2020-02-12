#!/usr/bin/python

class ListNode:
    def __init__(self, x):
        self.val = x
        self.next = None

class Solution:
    
    def printRecursive(self, array, listNode):
        
        if listNode == None:
            return
        self.printRecursive(array, listNode.next)
        array.append(listNode.val)    

    def printListFromTailToHead(self, listNode):
        array = []
        self.printRecursive(array, listNode)
        return array

n0 = ListNode(0)
n1 = ListNode(1)
n2 = ListNode(2)
n3 = ListNode(3)
n0.next = n1
n1.next = n2
n2.next = n3
n3.next = None
print Solution().printListFromTailToHead(n0)
    
