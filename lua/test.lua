require("GameFunc")
require("tools")

Game = {
    score = 0,
    state = {}
}
Drop = nil
Actions = {}

local function test1(game)
    game.set_state({{5,5},{7,7,6,6,5,5},{},{},{},{},{}})
    game.print()
    game.step({1,2})
    game.print()
    game.value()
    GameFunc.print_array(game.v)
end

local function test2(game)
    game.reset()
    game.print()
    game.step({1,2})
    game.print()
    game.step({3,2})
    game.print()
    game.step({4,2})
    game.print()
    game.step({5,2})
    game.print()
    game.step({6,2})
    game.print()
    Drop = {4,2,3,3,2,2}
    game.drop()
    game.print()
    game.value()
    GameFunc.print_array(game.v)
end

local function test3(game)
    GameFunc.set_state(game, {{4},{3,2},{3},{3},{2},{2}})
    GameFunc.print_state(game)
    GameFunc.solveK(game,4)
end

local function test4(game)
    GameFunc.set_state(game, {{},{3},{},{},{},{}})
    GameFunc.print_state(game)
    Drop = {4,2,3,3,2,2}
    local ret = GameFunc.solveK(game,4)

    game = ret[1]
    Drop = {4,4,5,5,3,4}
    local ret = GameFunc.solveK(game,4)
    -- print_state(game)

end

local game = tools.deepcopy(Game)
tools.get_actions()
test4(game)

test = {}
test.Game = Game
test.Drop = Drop
test.Actions = Actions
test.test = test4
return test