function get_col_names( table )
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