
--
-- Parameters
--
wordlen = 10                    -- word length
nwords = tonumber(arg[1]) or 10000        -- number of words
nsearches = tonumber(arg[2]) or 100000    -- number of searches
zipf_s = tonumber(arg[3]) or 1            -- Zipf distribution "s" parameter

--
-- Return a random string of the specified length
--
function random_string(len) 
    local s = ''
    for i=1,len do
        s = s .. string.char(96 + math.random(1, 26))
    end

    return s
end

--
-- Return frequency of rank k based upon Zipf distribution 
-- k: rank in set
-- s: parameter
-- N: total set size
-- 
function zipf(k, s, N) 
    local h = 0.0
    for n=1,N do
        h = h + 1.0/math.pow(n, s)
    end

    return 1.0/math.pow(k, s)/h
end

--
-- Shuffle the array
--
function shuffle(a) 
    for i=1,#a do
        local r = math.random(i, #a)
        local t = a[i]
        a[i] = a[r]
        a[r] = t
    end 
end

--
-- Generate dictionary of random words
-- nwords: number of words in dictionary
--
function generate_dictionary(nwords) 
    local words = {}

    for i=1,nwords do
        table.insert(words, {word=random_string(wordlen), p=zipf(i, zipf_s, nwords)})
    end

    return words
end

--
-- Generate sample from dictionary accordingly to determined probability
-- nwords: number of words in dictionary
--
function generate_sample(words, nwords) 
    local sample = {}

    while #sample <= nwords do
        local r = math.random()

        local cp = 0.0  --cumulative probability
        for _,word in ipairs(words) do
            cp = cp + word.p
            if r < cp then
                table.insert(sample, word.word)
                break
            end 
        end
    end

    return sample 
end

--
-- Count frequencies of each word in the given sample
--
function count_sample(sample)

    local freq = {}

    for _, word in ipairs(sample) do

        if freq[word] then
            freq[word] = freq[word] + 1
        else
            freq[word] = 1
        end 
    end

    return freq
end

-- set random seed
math.randomseed(os.time())

words = generate_dictionary(nwords)
searches = generate_sample(words, nsearches)

freq = count_sample(searches)

-- calculate ensemble entropy 
entropy = 0.0
for _, word in ipairs(words) do
--    print(word.word, word.p, math.floor(word.p*nsearches), freq[word.word])
    entropy = entropy + word.p * math.log(1.0/word.p)/math.log(2)
end

print(entropy)

file = assert(io.open('s' .. nwords .. '-' .. nsearches .. '-z' .. zipf_s .. '.txt', 'w')) 

file:write(';wordlen: ' .. wordlen .. '\n')
file:write(';nwords: ' .. nwords .. '\n')
file:write(';nsearches: ' .. nsearches .. '\n')
file:write(';zipf s: ' .. zipf_s .. '\n')
file:write(';entropy: ' .. entropy .. '\n')

shuffle(words)
for _,word in ipairs(words) do
    file:write('i ' .. word.word .. ' ' .. math.floor(word.p*nsearches) .. '\n')
end

for _,w in ipairs(searches) do
    file:write('s ' .. w .. '\n') 
end

file:close()

