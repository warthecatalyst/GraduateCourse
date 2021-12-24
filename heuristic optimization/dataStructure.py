# 在该文件中定义基本的数据结构类型，主要分为上层算法所需的数据结构和下层算法所需的数据结构
# 实现随机的图结构构造函数以及实现一些从文件中读取出基本数据结构的函数
from collections import deque
import random


# 问题本身的数据结构，存储一个图和代理的起点和终点
class ProblemDefinition:
    def __init__(self, G, S, T):
        # G为图，用邻接链表表示
        self.G = G
        self.S = S
        self.T = T

    def printPro(self):
        print("Graph vertex num = " + str(len(self.G)))
        print("Graph = " + str(self.G))
        print("S = " + str(self.S))
        print("T = " + str(self.T))


class Stack:
    def __init__(self):
        self.container = deque()

    def push(self, value):
        self.container.append(value)

    def pop(self):
        return self.container.pop()

    def length(self):
        return self.container.__len__()


def random_graph(index: int) -> list:
    if index == 0:
        # 随机生成一个数作为节点数
        graph = []
        ran = random.randint(100, 300)
        neimin, neimax = ran // 20, ran // 10
        for i in range(ran):
            curnei = []
            curnein = random.randint(neimin, neimax)
            while len(curnei) < curnein:
                rann = random.randint(0, ran - 1)
                if rann == i:
                    continue
                if not curnei.__contains__(rann):
                    curnei.append(rann)
            graph.append(curnei)
        return graph
    elif index == 1:
        # 生成一个20x20的网格
        graph = []
        for i in range(20):
            for j in range(20):
                curnei = []
                if not i == 0:
                    curnei.append((i - 1) * 20 + j)
                if not i == 19:
                    curnei.append((i + 1) * 20 + j)
                if not j == 0:
                    curnei.append(i * 20 + j - 1)
                if not j == 19:
                    curnei.append(i * 20 + j + 1)
                graph.append(curnei)
        # 随机放上障碍，随机去掉10-20个点
        delnum = random.randint(10, 20)
        delete = []
        while delete.__len__() < delnum:
            ran = random.randint(0, 399)
            if delete.__contains__(ran):
                continue
            graph[ran] = []
            if delete.__contains__(ran):
                continue
            delete.append(ran)
            i, j = ran // 20, ran % 20
            if i > 0:
                if not len(graph[(i-1)*20+j]) == 0:
                    graph[(i-1)*20+j].remove(ran)
            if i < 19:
                if not len(graph[(i + 1) * 20 + j]) == 0:
                    graph[(i+1)*20+j].remove(ran)
            if j > 0:
                if not len(graph[i * 20 + j - 1]) == 0:
                    graph[i*20+j-1].remove(ran)
            if j < 19:
                if not len(graph[i * 20 + j + 1]) == 0:
                    graph[i*20+j+1].remove(ran)
        return graph


def create_pro(index: int) -> ProblemDefinition:
    G = random_graph(index)
    vertex = len(G)
    agents = random.randint(vertex // 20, vertex // 10)
    S, T = [], []
    if index == 0:
        while len(S) < agents:
            ranS = random.randint(0, vertex)
            ranT = random.randint(0, vertex)
            while S.__contains__(ranS):
                ranS = random.randint(0, vertex)
            while T.__contains__(ranT):
                ranT = random.randint(0, vertex)
            S.append(ranS)
            T.append(ranT)
    elif index == 1:
        while len(S) < agents:
            ranS = random.randint(0, vertex)
            ranT = random.randint(0, vertex)
            while len(G[ranS]) == 0 or S.__contains__(ranS):
                ranS = random.randint(0, vertex)
            while len(G[ranT]) == 0 or T.__contains__(ranT):
                ranT = random.randint(0, vertex)
            S.append(ranS)
            T.append(ranT)
    return ProblemDefinition(G, S, T)
