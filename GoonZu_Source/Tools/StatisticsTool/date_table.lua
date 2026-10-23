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
