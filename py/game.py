# -*- coding: utf-8 -*-
from copy import deepcopy
from random import randint
from threading import Thread
from time import time


action_space = [0, 0, 0, 0]
action_space[0] = [(i, j) for i in range(6)
                   for j in range(6) if i != j]
action_space[1] = [[act1, act2]
                   for act1 in action_space[0]
                   for act2 in action_space[0]]
action_space[2] = [[act1, act2, act3]
                   for act1 in action_space[0]
                   for act2 in action_space[0]
                   for act3 in action_space[0]]
action_space[3] = [[act1, act2, act3, act4]
                   for act1 in action_space[0]
                   for act2 in action_space[0]
                   for act3 in action_space[0]
                   for act4 in action_space[0]]

# print(action_space[0])


class MyThread(Thread):

    game = None
    acts = None
    k = 0
    rets = None
    nnn = 0
    last_inv = None

    def __init__(self, game, act, k):
        Thread.__init__(self)
        self.game = deepcopy(game)
        self.act = act
        self.k = k
        self.nnn = len(action_space[k-2])
        self.rets = [None for i in range(self.nnn)]

    def run(self):
        a = time()
        ret = self.game.step(self.act)
        if ret == "INVALID" or ret == "DEAD":
            self.rets = [ret for _ in range(self.nnn)]
            b = time()
            print(b-a)
            return

        # self.game.Drop()

        a = time()
        actions = action_space[self.k-2]
        i = 0
        while i < len(actions):
            g_ = deepcopy(self.game)
            acts = actions[i]
            for j in range(len(acts)):
                ret = g_.step(acts[j])
                if ret == "INVALID" or ret == "DEAD":
                    if len(acts)-j == 3:
                        self.rets[i:i+900] = [ret for _ in range(900)]
                        i += 900
                    elif len(acts)-j == 2:
                        self.rets[i:i+30] = [ret for _ in range(30)]
                        i += 30
                    elif len(acts)-j == 1:
                        self.rets[i] = ret
                        i += 1
                    break
            if ret == "INVALID" or ret == "DEAD":
                continue
            g_.value()
            self.rets[i] = g_
            i += 1

        b = time()
        print(b-a)
        return

    def get_results(self):
        return self.rets


class Game():

    m, n = 7, 6
    state = None
    drop = None
    score = 0
    v = 0

    def __init__(self):
        pass

    def reset(self):
        self.state = [[1, 1] for _ in range(self.n)]
        self.score = 0

    def move(self, act):
        fr, to = self.state[act[0]], self.state[act[1]]
        if len(fr) >= 2:
            for _ in range((fr[-1] == fr[-2])+1):
                to.append(fr.pop(-1))
        elif len(fr) == 1:
            to.append(fr.pop(-1))
        else:
            # print("Invalid Move.")
            return "INVALID"

    def comb(self, i):
        to = self.state[i]
        if len(to) >= 4:
            if to[-1] == to[-2] and to[-1] == to[-3] and to[-1] == to[-4]:
                to.pop(-1)
                to.pop(-1)
                to.pop(-1)
                to.append(to.pop(-1)+1)
            else:
                pass
        if len(to) >= 3:
            if to[-1] == to[-2] and to[-1] == to[-3]:
                to.pop(-1)
                to.pop(-1)
                to.append(to.pop(-1)+1)
        if len(to) >= 3:
            if to[-1] == to[-2] and to[-1] == to[-3]:
                to.pop(-1)
                to.pop(-1)
                to.append(to.pop(-1)+1)
        if len(to) >= 3:
            if to[-1] == to[-2] and to[-1] == to[-3]:
                to.pop(-1)
                to.pop(-1)
                to.append(to.pop(-1)+1)
        if to[-1] == 9:
            to.pop(-1)
            self.score += 1

    def isDead(self, i):
        to = self.state[i]
        if len(to) > self.m:
            # print("Game Over.")
            return "DEAD"

    def step(self, act):
        # move
        ret = self.move(act)
        if ret == "INVALID":
            return "INVALID"

        # Comb
        self.comb(act[1])

        # Dead
        ret = self.isDead(act[1])
        if ret == "DEAD":
            return "DEAD"

        return "VALID"

    def setDrop(self, c):
        self.drop = c

    def Drop(self):
        for i in range(self.n):
            self.state[i].append(self.drop[i])
            self.comb(i)
            ret = self.isDead(i)
            if ret == "DEAD":
                return "DEAD"
        return "Safe"

    def genDrop(self):
        mm = []
        for x in self.state:
            if len(x) > 0:
                mm.append(max(x))
            else:
                mm.append(0)
        mm = max(mm)
        drop = [randint(mm-1, mm+1) for _ in range(self.n)]
        for i in range(self.n):
            drop[i] = min(max(drop[i], 1), 7)
        return drop

    def print_state(self):
        tmp = [[0 for _ in range(self.n)] for _ in range(self.m)]
        for i in range(self.n):
            for j in range(len(self.state[i])):
                tmp[j][i] = self.state[i][j]
        print("Score: ", self.score)
        print("State: ")
        for i in range(self.m-1, -1, -1):
            if sum(tmp[i]) > 0:
                print(tmp[i])

    def value(self):
        # 剩余个数
        def f1(g):
            return sum([len(x) for x in g.state])

        # 剩余最高高度
        def f2(g):
            return max([len(x) for x in g.state])

        # 最大珠子数
        def f3(g):
            return max([max(x) for x in g.state if len(x) > 0])

        # 非空杆数
        def f4(g):
            return sum([len(x) != 0 for x in g.state])
        g = self
        self.v = f1(g), f2(g), f4(g)

    def solveK_v1(self, k):
        a = time()
        actions = action_space[k-1]
        rets = [0 for _ in range(len(actions))]
        for i in range(len(actions)):
            acts = actions[i]
            g_ = deepcopy(self)
            rets[i] = g_.sim(acts)
        b = time()
        print(b-a)
        best = None
        best_v = 0
        best_i = 0
        for i in range(len(rets)):
            ret = rets[i]
            if ret not in ["INVALID", "DEAD"]:
                if best is None or ret.v < best_v:
                    best = ret
                    best_v = best.v
                    best_i = i
        return actions[best_i], best

    def solveK_v2(self, k):
        actions = action_space[0]
        nnn = len(actions)
        ths = [None for _ in range(nnn)]
        rets = []
        th1 = [0, 1, 2, 3, 4, 5, 6, 7]
        th2 = [8, 9, 10, 11, 12, 13, 14, 15]
        th3 = [16, 17, 18, 19, 20, 21, 22, 23]
        th4 = [24, 25, 26, 27, 28, 29]
        for i in th1:
            ths[i] = MyThread(self, actions[i], k)
            ths[i].start()
        for i in th1:
            ths[i].join()
            rets.extend(ths[i].get_results())
        for i in th2:
            ths[i] = MyThread(self, actions[i], k)
            ths[i].start()
        for i in th2:
            ths[i].join()
            rets.extend(ths[i].get_results())
        for i in th3:
            ths[i] = MyThread(self, actions[i], k)
            ths[i].start()
        for i in th3:
            ths[i].join()
            rets.extend(ths[i].get_results())
        for i in th4:
            ths[i] = MyThread(self, actions[i], k)
            ths[i].start()
        for i in th4:
            ths[i].join()
            rets.extend(ths[i].get_results())

        best = None
        best_v = 0
        best_i = 0
        for i in range(len(rets)):
            ret = rets[i]
            if ret not in ["INVALID", "DEAD"]:
                if best is None or ret.v < best_v:
                    best = ret
                    best_v = best.v
                    best_i = i
        return action_space[k-1][best_i], best, rets

    def solveK(self, k):
        # return self.solveK_v1(k)
        a = time()
        ret2 = self.solveK_v1(k)
        b = time()
        print("total time: ", b-a)

        return ret2[0], ret2[1]

    def sim(self, acts):
        for act in acts:
            ret = self.step(act)
            if ret == "INVALID":
                return "INVALID"
            elif ret == "DEAD":
                return "DEAD"
        self.value()
        return self
