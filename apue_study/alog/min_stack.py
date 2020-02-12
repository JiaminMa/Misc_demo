class Solution:

        def __init__(self):

            self.stack = []
            self.min_stack = []

        def push(self, node):

            self.stack.append(node)
            if len(self.min_stack) == 0 or node < self.min_stack[-1]:
                self.min_stack.append(node)

        def pop(self):
            tmp = self.stack.pop()
            if tmp == self.min_stack[-1]:
                self.min_stack.pop()
            return tmp

        def top(self):
            return self.stack[-1]

        def min(self):
            return self.min_stack[-1]

min_stack = Solution()
min_stack.push(10)
min_stack.push(1)
min_stack.push(4)
min_stack.push(3)
min_stack.push(6)

print min_stack.min(), min_stack.top()
print min_stack.pop()
print min_stack.min(), min_stack.top()

print min_stack.pop()
print min_stack.min(), min_stack.top()

print min_stack.pop()
print min_stack.min(), min_stack.top()

print min_stack.pop()
print min_stack.min(), min_stack.top()
