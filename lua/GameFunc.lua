require("tools")

GameFunc = {}

local function reset(game)
    game.state = {}
    for i = 1,6 do
        game.state[i] = {1,1}
    end
end

local function print_state(game)
    local tmp = {{0,0,0,0,0,0},
                {0,0,0,0,0,0},
                {0,0,0,0,0,0},
                {0,0,0,0,0,0},
                {0,0,0,0,0,0},
                {0,0,0,0,0,0},
                {0,0,0,0,0,0}}
    for i = 1,6 do
        for j = 1,#game.state[i] do
            tmp[j][i] = game.state[i][j]
        end
        -- for j = 1,#Game.state[i] do
        --     s = s..Game.state[i][j].." "
        -- end
    end
    for i = 7,1,-1 do
        local ss = tools.sum(tmp[i])
        if ss>0 then
            tools.print_array(tmp[i])
        end 
    end
    print()
end

local function move(game, act)
    local fr, to = game.state[act[1]], game.state[act[2]]
    if (#fr == 0) then
        return "INVALID"
    elseif (#fr == 1) then
        to[#to+1],fr[#fr] = fr[#fr], nil
    else
        if fr[#fr] == fr[#fr-1] then
            to[#to+1], to[#to+2] = fr[#fr], fr[#fr]
            fr[#fr],fr[#fr-1] = nil, nil
        else
            to[#to+1],fr[#fr] = fr[#fr], nil
        end
    end
end

local function comb(game,i)
    local t = game.state[i]
    if #t >= 4 then
        if t[#t] == t[#t-1] and t[#t] == t[#t-2] and t[#t] == t[#t-3] then
            t[#t],t[#t-1],t[#t-2], t[#t-3] = nil, nil, nil, t[#t-3]+1
        end
    end
    if #t>=3 then
        if t[#t] == t[#t-1] and t[#t] == t[#t-2] then
            t[#t],t[#t-1],t[#t-2] = nil, nil, t[#t-2]+1
            if t[#t] == t[#t-1] and t[#t] == t[#t-2] then
                t[#t],t[#t-1],t[#t-2] = nil, nil, t[#t-2]+1
                if t[#t] == t[#t-1] and t[#t] == t[#t-2] then
                    t[#t],t[#t-1],t[#t-2] = nil, nil, t[#t-2]+1
                end
            end
        end
    end
    if t[#t] == 9 then
        game.score = game.score + 1
        t[#t] = nil
    end
end

local function step(game,act)
    local ret = move(game,act)
    if (ret=="INVALID") then
        return "INVALID"
    end

    comb(game, act[2])

    if #game.state[act[2]] > 7 then
        return "DEAD"
    end
end

local function set_state(game, state)
    game.state = state
end

local function drop(game)
    for i = 1,6 do
        game.state[i][#game.state[i]+1] = Drop[i]
        comb(game, i)
        if #game.state[i] == 7 then
            return "DEAD"
        end
    end
end

local function value(game)
    local len = {}
    for i = 1,6 do
        len[i] = #game.state[i]
    end
    local empty = {}
    for i = 1,6 do
        if #game.state[i] ~= 0 then
            empty[i] = 1
        else
            empty[i] = 0
        end
    end
    return {tools.sum(len),tools.max(len), tools.sum(empty)}
end

local function sim(game,acts)
    for i, act in ipairs(acts) do
        local ret = step(game, act)
        if ret == "INVALID" or ret == "DEAD" then
            return ret
        end
    end
    return "Valid"
end

local function solve3(game)
    local best = nil
    local best_acts = nil
    local best_v = nil
    for i = 1,#Actions[1] do
        for j = 1,#Actions[1] do
            for k = 1,#Actions[1] do
                local game_ = tools.deepcopy(game)
                local acts = {Actions[1][i],Actions[1][j], Actions[1][k]}
                local ret = sim(game_,acts)
                if ret == "INVALID" or ret == "DEAD" then
                    ;
                elseif best == nil or tools.value_less(value(game_),best_v) then
                    best = game_
                    best_v = value(game_)
                    best_acts = acts
                end
            end
        end
    end
    return {best, best_acts}
end

local function solve4_v1(game)
    local best = nil
    local best_acts = nil
    local best_v = nil
    for i = 1,#Actions[1] do
        local game1 = tools.deepcopy(game)
        local ret = step(game1,Actions[1][i])
        if ret == "INVALID" or ret == "DEAD" then
            ;
        else
            for j = 1,#Actions[1] do
                local game2 = tools.deepcopy(game1)
                drop(game2)
                ret = step(game2,Actions[1][j])
                if ret == "INVALID" or ret == "DEAD" then
                    ;
                else
                    for k = 1,#Actions[1] do
                        local game3 = tools.deepcopy(game2)
                        ret = step(game3,Actions[1][k])
                        if ret == "INVALID" or ret == "DEAD" then
                            ;
                        else
                            for l = 1,#Actions[1] do
                                local game4 = tools.deepcopy(game3)
                                ret = step(game4,Actions[1][l])
                                if ret == "INVALID" or ret == "DEAD" then
                                    ;
                                elseif best == nil or tools.value_less(value(game4),best_v) then
                                    best = game4
                                    best_v = value(game4)
                                    best_acts = {Actions[1][i],Actions[1][j],Actions[1][k],Actions[1][l]}
                                end
                            end
                        end
                    end
                end
            end
        end
    end
    return {best, best_acts}
end


local function solve1(game)
    local best = nil
    local best_acts = nil
    local best_v = nil
    for i = 1,#Actions[1] do
        local game_ = tools.deepcopy(game)
        local acts = {Actions[1][i]}
        local ret = sim(game_,acts)
        tools.print_array(acts[1])
        if ret == "INVALID" or ret == "DEAD" then
            print(ret)
        else
            print_state(game_)
            if best == nil or tools.value_less(value(game_),best_v) then
                best = game_
                best_v = value(game_)
                best_acts = acts
            end
        end
    end
    return {best, best_acts}
end

local function solveK_v1(game,k)
    if k==4 then
        return solve4_v1(game)
    elseif k==3 then
        return solve3(game)
    elseif k==1 then
        return solve1(game)
    end
end

local function solveK(game, k)
    -- local socket = require("socket")
    local g_start = os.clock()
    local ret = solveK_v1(game, k)
    local g_end = os.clock()
    print(string.format("%.14f", g_end - g_start))
    print_state(ret[1])
    print()
    for i = 1,#ret[2] do
        tools.print_array(ret[2][i])
    end
    return ret
end

GameFunc.set_state = set_state
GameFunc.print_state = print_state
GameFunc.solveK = solveK

return GameFunc