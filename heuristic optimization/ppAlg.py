# 实现最基本的pp算法
import dataStructure
import random
from collections import deque


# 实现pp算法的类，其主要思想采用了Silver等人的CA*算法思想
class PPAlg:
    def __init__(self, pro: dataStructure.ProblemDefinition):
        self.pro = pro
        # 全局的预约表，假设如果不能在1w个单位时间内找到对应的解，就返回无解
        self.R = [[0 for i in range(10000)] for j in range(len(self.pro.G))]
        self.hasfound = [0 for i in range(len(self.pro.S))]

    # 为所有代理分配一个随机的优先级排序
    def random_priority(self) -> list:
        size = len(self.pro.S)
        ori = [i for i in range(size)]
        random.shuffle(ori)
        return ori

    # 通过BFS算法为当前代理寻找一条与已有代理路径不冲突的路径
    def BFS(self, agent, start, end):
        queue = deque()
        # 每个元素都是当前位置，当前时间，当前所走的路
        queue.append([start, 0, [start]])
        while not queue.__len__() == 0:
            elem = queue.popleft()
            curl, curt, curp = elem[0], elem[1], elem[2]
            if curl == end:
                return curp
            elif curt == 10000:
                break
            # print("current situation:" + str(curl) + "--" + str(curt) + "--" + str(curp))
            for neigh in self.pro.G[curl]:
                if self.R[neigh][curt + 1] == 1:
                    continue
                curp1 = [ele for ele in curp]
                curp1.append(neigh)
                queue.append([neigh, curt + 1, curp1])

            # 尝试等待
            curp1 = [ele for ele in curp]
            curp1.append(curl)
            queue.append([curl, curt + 1, curp1])

        return []

    # 在当前优先级下，分配一个代理的游走路径
    def planning(self, pri) -> list:
        ans = []
        for agent in pri:
            aPath = self.BFS(agent, self.pro.S[agent], self.pro.T[agent])
            if aPath is None:
                print("No Solution")
                return []
            ans.append(aPath)
            # 更新全局表R
            for i in range(len(aPath)):
                self.R[aPath[i]][i] = 1
            for j in range(len(aPath), 10000):
                self.R[aPath[-1]][j] = 1
        return ans

    # 主函数
    def main(self):
        print("R.size() = " + str(len(self.R)))
        pri = self.random_priority()
        ans = self.planning(pri)
        # 打印出结果
        for i in range(len(ans)):
            print("a" + str(pri[i]) + "'s path:" + str(ans[i]))
        # 计算flowtime
        times = [len(i) for i in ans]
        flowtime = sum(times)
        return flowtime