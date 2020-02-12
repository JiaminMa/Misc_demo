
class Solution:

    def IsPopOrder(self, pushV, popV):

        if not pushV or len(pushV) != len(popV):
            return False

        stack = []
        for i in range(0, len(pushV)):
            stack.append(pushV[i])
            while len(stack) and stack[-1] == popV[0]:
                stack.pop()
                popV.pop(0)

        if len(stack) != 0:
            return False
        return True


pushV = [1,2,3,4,5]
popV = [4,5,3,2,1]
popV2 = [4,3,5,1,2]
sol = Solution()

print sol.IsPopOrder(pushV, popV)
print sol.IsPopOrder(pushV, popV2)
