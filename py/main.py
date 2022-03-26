# -*- coding: utf-8 -*-
from game import Game
from ctypes import cdll


def test1():
    game = Game()
    game.reset()
    print(game.genDrop())
    game.print_state()
    game.step((0, 1))
    game.print_state()
    game.step((2, 1))
    game.print_state()
    game.step((3, 1))
    game.print_state()
    game.step((4, 1))
    game.print_state()
    game.step((5, 1))
    game.print_state()
# print(game.genDrop())


def test2():
    game = Game()
    game.reset()
    game.step((0, 1))
    game.step((2, 1))
    game.step((3, 1))
    game.step((4, 1))
    game.step((5, 1))
    c = [4, 2, 3, 3, 2, 2]
    game.setDrop(c)
    game.Drop()
    game.print_state()
    acts, g_ = game.solveK(4)
    print(acts)
    g_.print_state()


def test3():
    game = Game()
    game.reset()
    game.step((0, 1))
    game.step((2, 1))
    game.step((3, 1))
    game.step((4, 1))
    game.step((5, 1))
    c = [4, 2, 3, 3, 2, 2]
    game.setDrop(c)
    game.Drop()
    game.print_state()
    acts, g_ = game.solveK(4)
    print(acts)
    g_.print_state()

    game = g_
    # c = game.genDrop()
    # print(c)
    c = [4, 4, 5, 5, 3, 4]
    game.setDrop(c)
    game.Drop()
    game.print_state()
    acts, g_ = game.solveK(4)
    print(acts)
    g_.print_state()


def test4():
    solveK = cdll.LoadLibrary("./solveK.so")
    print(solveK.fab(4))
    # g = solveK.Game()
    # g.reset()
    # g.print_state()
    game = Game()
    game.reset()
    game.print_state()
    g = solveK.solve(game.state)
    g.print_state()


test3()
