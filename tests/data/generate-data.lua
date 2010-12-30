
--
-- Parameters
--
wordlen = 10          -- word length
nwords = 10000        -- number of words
nsearches = 100000    -- number of searches
zipf_s = 0.9          -- Zipf distribution "s" parameter

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

-- set random seed
math.randomseed(os.time())

words = generate_dictionary(nwords)
searches = generate_sample(words, nsearches)

file = assert(io.open('s10000.txt', 'w')) 

shuffle(words)
for _,word in ipairs(words) do
    file:write('i ' .. word.word .. ' ' .. math.floor(word.p*nwords) .. '\n')
end

for _,w in ipairs(searches) do
    file:write('s ' .. w .. '\n') 
end

file:close()

