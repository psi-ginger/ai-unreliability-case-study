function sort_table ( t )
    for i=1,#t do
        for j=i+1,#t do
            a = tonumber(t[i])
            b = tonumber(t[j])
            if a ~= nil and b ~= nil then
                comp = a > b
            else
                comp = t[i] > t[j]
            end
            if comp then
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

function get_dates ( tablename )
    ret = {}
    check = {}
    for rk, rv in pairs( NStatistics ) do
        for tk, tv in pairs( rv) do
            if tk == tablename then
                if check[rk] == nil then 
                    ret[#ret + 1] = rk
                    check[rk] = true
                end
                break;
            end
        end
    end

    sort_table( ret )
    
    return ret
end

function get_times ( tablename )
    ret = {}
    check = {}
    for rk, rv in pairs( NStatistics ) do
        for tk, tv in pairs( rv ) do
            if tk == tablename then
                for hk, hv in pairs( tv ) do
                    if check[hk] == nil then 
                        ret[#ret + 1] = hk
                        check[hk] = true
                    end
                end
                break;
            end
        end
    end

    sort_table( ret )
    
    return ret
end

function get_rows( tablename )
    dates = get_dates( tablename )
    times = get_times( tablename )

    ret = {}
    check = {}

    for i, n in ipairs(dates) do
        table = NStatistics[n]
        if table ~= nil and table[tablename] ~= nil then
            root = table[tablename]
            for hi, hn in ipairs(times) do
                row = root[hn]
                if row ~= nil then
                    for rk, rv in pairs(row) do
                        if check[rk] == nil then 
                            ret[#ret + 1] = rk
                            check[rk] = true
                        end
                    end
                end
            end
        end
    end

    sort_table( ret )

    return ret
end

function get_cols( table )
    column = {}
    if type(table) ~= "string" then return column end

    for rk, rv in pairs(NStatistics) do
        for tk, tv in pairs(rv) do
            if tk == table then
                root = tv
                for k, v in pairs(root) do
                    for rk, rv in pairs(v) do
                        for ck, cv in pairs( rv ) do
                            column[ck] = true
                        end
                    end
                end
            end
        end
    end

    sortedcolumn = {}
    for n in pairs(column) do sortedcolumn[#sortedcolumn + 1] = n end
    sort_table(sortedcolumn)

    return sortedcolumn
end

function get_tables ( date )
    ret = {}
    check = {}
    for rk, rv in pairs( NStatistics ) do
        if date ~= nil then
            if rk ~= date then rv = nil end
        end
        if rv ~= nil then
            for tk, tv in pairs( rv ) do
                if check[tk] == nil then 
                    ret[#ret + 1] = tk
                    check[tk] = true
                end
            end
        end
    end

    sort_table( ret )
    
    return ret
end

function date_table( date, table, incolumn )
    print("TableName - " .. table)
    print("Date - " .. date)
    print()

    root = NStatistics[date][table]
    ntable = {}
    column = {}
    row = {}
    for k, v in pairs(root) do
        for rk, rv in pairs(v) do
            if ntable[rk] == nil then ntable[rk] = {} end
            row[rk] = true
            for ck, cv in pairs( rv ) do
                column[ck] = true
                if type(cv) == "number" then
                    if ntable[rk][ck] == nil then ntable[rk][ck] = 0 end
                    ntable[rk][ck] = ntable[rk][ck] + cv
                else
                    ntable[rk][ck] = cv
                end
            end
        end
    end

    sortedcolumn = {}
    for n in pairs(column) do sortedcolumn[#sortedcolumn + 1] = n end
    sort_table(sortedcolumn)

    if type(incolumn) == 'table' then sortedcolumn = incolumn end

    sortedrow = {}
    for n in pairs(row) do sortedrow[#sortedrow + 1] = n end
    sort_table_num(sortedrow)

    colnames = "rowdatas"
    for i, n in ipairs(sortedcolumn) do
        colnames = colnames .. "\t" .. n
    end
    
    print(colnames)

    row = ""
    for i, rv in ipairs(sortedrow) do
        row = rv
        v = ntable[rv]
        for i, nv in ipairs(sortedcolumn) do
            if v[nv] ~= nil then
                row = row .. "\t" .. v[nv]
            else
                row = row .. "\t" .. 0
            end
        end
        print(row)
    end
end

function time_table( date, time, table, incolumn )
    print("TableName - " .. table)
    print("Date - " .. date)
    print("Time - " .. time)
    print()

    root = NStatistics[date][table][time]
    ntable = {}
    column = {}
    row = {}
    for rk, rv in pairs(root) do
        if ntable[rk] == nil then ntable[rk] = {} end
        row[rk] = true
        for ck, cv in pairs( rv ) do
            column[ck] = true
            if type(cv) == "number" then
                if ntable[rk][ck] == nil then ntable[rk][ck] = 0 end
                ntable[rk][ck] = ntable[rk][ck] + cv
            else
                ntable[rk][ck] = cv
            end
        end
    end

    sortedcolumn = {}
    for n in pairs(column) do sortedcolumn[#sortedcolumn + 1] = n end
    sort_table(sortedcolumn)

    if type(incolumn) == 'table' then sortedcolumn = incolumn end

    sortedrow = {}
    for n in pairs(row) do sortedrow[#sortedrow + 1] = n end
    sort_table_num(sortedrow)

    colnames = "rowdatas"
    for i, n in ipairs(sortedcolumn) do
        colnames = colnames .. "\t" .. n
    end
    
    print(colnames)

    row = ""
    for i, rv in ipairs(sortedrow) do
        row = rv
        v = ntable[rv]
        for i, nv in ipairs(sortedcolumn) do
            if v[nv] ~= nil then
                row = row .. "\t" .. v[nv]
            else
                row = row .. "\t" .. 0
            end
        end
        print(row)
    end
end

