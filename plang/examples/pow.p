# # #
# x1^x2
#
# - Mattias Eriksson, 7:e januari 2015
#

program (x1, x2)
    x3 := succ(x1)
    x2 := pred(x2)
    while x2 != 0 do
        x5 := pred(x3)
        x5 := pred(x5)
        x4 := succ(x1)
        x4 := pred(x4)
        while x5 != 0 do
            x6 := succ(x4)
            x6 := pred(x6)
            while x6 != 0 do
                x1 := succ(x1)
                x6 := pred(x6)
            end
            x5 := pred(x5)
        end
        x2 := pred(x2)
    end
result(x1)
