-- globals.lua
-- show all global variables

--local seen={}
--
--function dump(t,i)
--  seen[t]=true
--  local s={}
--  local n=0
--  for k in pairs(t) do
--    n=n+1 s[n]=k
--  end
--  table.sort(s)
--  for k,v in ipairs(s) do
--    print(i,v)
--    v=t[v]
--    if type(v)=="table" and not seen[v] then
--      dump(v,i.."\t")
--    end
--  end
--end
--
--dump(_G,"")

local dump = io.open("state_dump.txt","w")

local aprased = {}

function tab_find_exists(tab,data)
    for k,v in pairs(tab) do
        if v == data then
            return true
        end
    end
    return false
end

function dump_table(fi,tab,depth)
    table.insert(aprased,tab)
    for k,v in pairs(tab) do
        if type(v) == "table" and not tab_find_exists(aprased,v) then
            for i=1,depth,1 do
                fi:write("  ")
            end
            fi:write(tostring(k),":\n")
            dump_table( fi, v, depth+1 )
        else
            for i=1,depth,1 do
                fi:write("  ")
            end
            fi:write( tostring(k)," = ",tostring(v),"\n" );
        end
    end
    fi:write("\n")
    fi:flush()
end

dump_table( dump ,_G ,0 )
dump:flush()
dump:close()