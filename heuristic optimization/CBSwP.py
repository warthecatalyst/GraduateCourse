import dataStructure
import random
from collections import deque


# Node的实现本质上与CBS是相同的，就是多了一个偏序集合。
# 对于偏序集合的定义如下：一个二维list，然后list[ai]中是所有ai<aj的集合

class Node:
    def __init__(self, constraints: list, solution: list, cost: int,priorder:list):
        self.constraints = constraints
        self.solution = solution
        self.cost = cost
        self.priorder = priorder

    # 查找solution的每一条路径，如果出现有两个节点在同一时间出现在同一地点就返回true
    def hasConflict(self):
        m = len(self.solution)
        for i in range(m):
            curipath = self.solution[i]
            for j in range(i + 1, m):
                curjpath = self.solution[j]
                longer = max(len(curipath), len(curjpath))
                for k in range(longer):
                    curi, curj = 0, 1
                    if k >= len(curipath):
                        curi = curipath[-1]
                    else:
                        curi = curipath[k]
                    if k >= len(curjpath):
                        curj = curjpath[-1]
                    else:
                        curj = curjpath[k]
                    if curi == curj:
                        return True
        return False

    # 如果有冲突，找出最先出现的冲突
    def findFirstConflict(self):
        conflicts = []
        m = len(self.solution)
        for i in range(m):
            curipath = self.solution[i]
            for j in range(i + 1, m):
                curjpath = self.solution[j]
                longer = max(len(curipath), len(curjpath))
                for k in range(longer):
                    curi, curj = 0, 1
                    if k >= len(curipath):
                        curi = curipath[-1]
                    else:
                        curi = curipath[k]
                    if k >= len(curjpath):
                        curj = curjpath[-1]
                    else:
                        curj = curjpath[k]
                    if curi == curj:
                        conflicts.append((i, j, curi, k))  # <ai,aj,v,t>
        conflicts.sort(key=lambda x: x[3])
        return conflicts[0]

    # 找到ai和aj是否存在着优先级排序，如果不存在返回False，ai<aj返回True
    def findaiaj(self,ai:int,aj:int):
        cur = self.priorder[ai]
        if cur.__contains__(aj):
            return True

# 与CBS类似,CBSw/P就是添加了相关的优先级,在Node中实现
class CBSwP:
    def __init__(self, pro: dataStructure.ProblemDefinition):
        self.pro = pro
        self.OPEN = []

    def findBestInOPEN(self):
        self.OPEN.sort(key=lambda x: x.cost)
        return self.OPEN.pop(0)

    def high_level(self):
        Root = Node([{} for i in range(len(self.pro.S))], [], 0)
        Root.solution = self.low_level(Root.constraints)
        time = [len(i) for i in Root.solution]
        Root.cost = sum(time)
        self.OPEN.append(Root)
        while not len(self.OPEN) == 0:
            P = self.findBestInOPEN()
            if not P.hasConflict():
                return P.solution

            C = P.findFirstConflict()
            for i in range(0, 1):
                ai, v, t = C[i], C[2], C[3]
                aj = C[1-i]
                A = Node(P.constraints, [], P.cost)
                A.constraints[ai][v].append(t)
                A.solution = self.low_level(A.constraints)
                time = [len(i) for i in A.solution]
                A.cost = sum(time)

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

    def main(self):
        answer = self.high_level()
        print("In CBS, the solution is:")
        for i in range(len(answer)):
            print("a"+str(i)+"'s path:"+str(answer[i]))
        times = [len(i) for i in answer]
        flowtime = sum(times)
        return flowtime