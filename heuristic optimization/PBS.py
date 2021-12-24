import dataStructure
import random
from collections import deque
import ppAlg

# 与CBS算法和CBSw/P算法类似，减少了constraints
# plan 就类似于 solution
class Node:
    def __init__(self,plan,priorder,cost):
        self.plan = plan
        self.priorder = priorder
        self.cost = cost

class PBSAlg:
    def __init__(self,pro:dataStructure.ProblemDefinition):
        self.pro = pro
        self.STACK = dataStructure.Stack()
        self.R = [[0 for i in range(10000)] for j in range(len(self.pro.G))]
        self.hasfound = [0 for i in range(len(self.pro.S))]

    # 为所有代理分配一个随机的优先级排序
    def random_priority(self) -> list:
        size = len(self.pro.S)
        ori = [i for i in range(size)]
        random.shuffle(ori)
        return ori

    def UpdatePlan(self,N:Node,ai):
        List = self.random_priority()
        for j in List:
            if j == ai:
                ans = self.low_level([])
                return ans


    def HighlevelSearch(self):
        pp = ppAlg.PPAlg(self.pro)
        return pp.main()

    # 使用BFS找到所有代理的游走路径
    def low_level(self, constraints: list):
        ans = []
        for i in range(len(self.pro.S)):
            ans.append(self.BFS(i, constraints[i]))
        return ans

    # BFS
    def BFS(self, agent: int, constraint: dict):
        start, end = self.pro.S[agent], self.pro.T[agent]
        queue = deque()
        queue.append([start, 0, [start]])
        while not queue.__len__() == 0:
            elem = queue.popleft()
            curl, curt, curp = elem[0], elem[1], elem[2]
            if curl == end:
                return curp
            elif curt == 10000:
                break
            for neigh in self.pro.G[curl]:
                if constraint[neigh].__contains__(curt + 1):
                    continue
                curp1 = curp.copy()
                curp1.append(neigh)
                queue.append([neigh, curt + 1, curp1])

            # 尝试等待
            if constraint[curl].__contains__(curt + 1):
                continue
            curp1 = curp.copy()
            curp1.append(curl)
            queue.append([curl, curt + 1, curp1])
        return []


