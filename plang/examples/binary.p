# # #
# while number > 0, bit = number mod 2, number = number div 2, repeat
#
# - Mattias Eriksson, 7:e januari 2015
#

program (x1)
    # current bit position
    x2 := 0
    x2 := succ(x2)

    # x3 = 9, for easy access to 10
    x3 := 0
    x3 := succ(x2)
    x3 := succ(x3)
    x3 := succ(x3)
    x3 := succ(x3)
    x3 := succ(x3)
    x3 := succ(x3)
    x3 := succ(x3)
    x3 := succ(x3)

    # x4 = 1, for easy access to 2
    x4 := 0
    x4 := succ(x4)

    # set flag to skip first 10 multiply
    x11 := 0

    # result
    x123 := 0
    
    while x1 != 0 do # outer loop

    # div with 2
    x1 := succ(x1)
    x5 := 0
    while x1 != 0 do
        x6 := succ(x4)
        x7 := pred(x1) # in last loop, x1 is either 1 or 2, 1 means rem 0, 2
                       # means rem 1
        while x6 != 0 do
            x1 := pred(x1)
            x6 := pred(x6)
        end
        x5 := succ(x5)
    end
    x1 := pred(x5)

    x17 := 0
    while x7 != 0 do
        x7 := 0
        x17 := succ(x17)
    end

    # multiply current bit position with 10
    while x11 != 0 do
        x11 := 0

        # mul 10
        x13 := succ(x2) # current bit+1
        x12 := succ(x3) # 10
        x2 := 0
        while x12 != 0 do
            x14 := pred(x13) # current bit holder
            while x14 != 0 do
                x2 := succ(x2)
                x14 := pred(x14)
            end
            x12 := pred(x12)
        end
    end
    x11 := succ(x11)

    # if remainder was 1, add current bit to result
    while x17 != 0 do
        x17 := 0
        x20 := succ(x2)
        x20 := pred(x20)
        while x20 != 0 do
            x123 := succ(x123)
            x20 := pred(x20)
        end
    end

    end # outer loop
result (x123)
