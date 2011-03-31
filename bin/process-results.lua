#!/usr/bin/lua

function parse_time(s) 
    -- parse time string formatted as m:ss:ff to seconds

    m, s, f = s:match('(%d):(%d%d).(%d%d)')
    return tonumber(m)*60 + tonumber(s) + tonumber(f)/100.0 
end


ifile = assert(io.open(arg[1]))

-- track execution time and memory use
local names = {}
local times = {}
local memories = {}

-- run through each file
for line in ifile:lines() do
    name, time, memory = line:match('(.+),(.+),(.+)')
    if name and time and memory then

        if times[name] then
            table.insert(times[name], parse_time(time))
        else
            table.insert(names, name)
            times[name] = {parse_time(time)}
        end

        if memories[name] then
            table.insert(memories[name], tonumber(memory))
        else
            memories[name] = {tonumber(memory)}
        end 

    end
end
ifile:close()

ofile = assert(io.open(arg[1] .. '-median', 'w'))

for _, name in ipairs(names) do

    time = times[name]
    table.sort(time)
    med_time = time[#time/2]

    memory = memories[name]
    table.sort(memory)

    med_memory = memory[#memory/2]

    ofile:write(name .. ',' .. med_time .. ',' .. med_memory .. '\n')
end

ofile:close()
