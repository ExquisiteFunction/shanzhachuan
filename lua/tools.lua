local function sum(a)
    local t = 0;
    for i = 1,#a do
        t = t+a[i]
    end
    return t
end

local function max(a)
    local t = 0;
    for i = 1,#a do
        if a[i] > t then
            t = a[i]
        end
    end
    return t
end

local function print_array(a)
    local s = ""
    for j = 1,#a do
        s = s..a[j].." "
    end
    print(s)
end

local function get_actions()
    Actions[1] = {}
    for i = 1,6 do
        for j = 1,6 do
            if i~=j then
                Actions[1][#Actions[1]+1] = {i,j}
            end
        end
    end
end

local function deepcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopy(orig_key)] = deepcopy(orig_value)
        end
        setmetatable(copy, deepcopy(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end

local function value_less(va, vb)
    for i = 1, #va do
        if va[i]>vb[i] then
            return false
        end
    end
    return true
end

tools = {}
tools.sum = sum
tools.max = max
tools.print_array = print_array
tools.get_actions = get_actions
tools.deepcopy = deepcopy
tools.value_less = value_less
return tools