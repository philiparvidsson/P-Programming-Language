# returns floor(sqrt(input)), the integer square root
program (x1)
    # store x1
    x101 := succ(x1)
    x101 := pred(x1)

    x2 := 0
    x2 := succ(x2)

    x4 := succ(x1)
    x4 := pred(x1)

    # keep refining the guess until it stops changing (babylonian method, new
    # guess = (old guess + input/old guess) / 2)
    x5 := 0
    x5 := succ(x5)

    # but first check if input = 0 or 1 and handle it
    x150 := succ(x1)
    x151 := succ(x2)

    while x151 != 0 DO
        x151 := pred(x151)
        x150 := pred(x150)
    end

    x152 := succ(x151)
    while x150 != 0 DO
        x152 := 0
        x150 := 0
    end

    while x152 != 0 DO
        x152 := 0
        x5 := 0 # cancel the refining loop
        x4 := 0
        while x1 != 0 DO
            x1 := 0
            x4 := succ(x4)
        end
    end

    while x5 != 0 DO
        # store old guess
        x6 := succ(x4)
        x6 := pred(x6)

        # calculate input / old guess
        x7 := succ(x101) # no pred needed, division algo needs to check x+1
        x8 := succ(x4)

        x10 := 0
        while x7 != 0 DO
            x9 := pred(x8)
            while x9 != 0 DO
                x7 := pred(x7)
                x9 := pred(x9)
            end
            x10 := succ(x10)
        end
        x10 := pred(x10)

        # add this value to the old guess (also add one, for the coming division)
        x11 := succ(x4)

        while x10 != 0 DO
            x11 := succ(x11)
            x10 := pred(x10)
        end

        # divide it with 2
        x4 := 0
        while x11 != 0 DO
            x3 := succ(x2)
            while x3 != 0 DO
                x11 := pred(x11)
                x3 := pred(x3)
            end
                x4 := succ(x4)
        end
        x4 := pred(x4)

        # x4 is the new guess, it will always be smaller than the old guess x6, except when floor(sqrt(input)) is reached
        x12 := succ(x4)
        x12 := pred(x12)
        x13 := succ(x6)

        while x12 != 0 DO
            x6 := pred(x6)
            x12 := pred(x12)
        end

        x25 := 0
        x25 := succ(x25)
        while x6 != 0 DO
            x25 := 0
            x6 := 0
        end

        while x25 != 0 DO
            x25 := 0
            x5 := 0
        end
    end

result (x4)
