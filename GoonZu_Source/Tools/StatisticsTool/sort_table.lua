function sort_table ( t )
    for i=1,#t do
        for j=i+1,#t do
            if t[i] > t[j] then
                v = t[i]
                t[i] = t[j]
                t[j] = v
            end
        end
    end
end

function sort_table_num ( t )
    for i=1,#t do
        for j=i+1,#t do
            if tonumber(t[i]) > tonumber(t[j]) then
                v = t[i]
                t[i] = t[j]
                t[j] = v
            end
        end
    end
end
