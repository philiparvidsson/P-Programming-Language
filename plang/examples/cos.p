# # #
# Some theory:
# The Taylor series expansion is used, expanding cos(x) near x=pi -> (-1)^k * |x-pi|^2k / 2k!
# This is used because the expansion is fairly good for ±pi from the point of expansion,
# even when using only 5 terms in the expansion. The most common expansion, the Maclaurin
# series, is by definition made at x=0, making it viable for x=±pi, though we can't supply
# plang with negative integers.
#
# Furthermore, we work at 4 decimals to the right, that is at 10^4 * everything, to simulate
# decimals in plang. This means x and pi are mult. by 10^4, which is then counteracted with
# some 10^4 divisions at each term in the series.
#
# Recall also that cos(x) = -1..1, this is transposed to cos(x)+1 = 0..2, again plang can't
# handle negative integers. Basically, this translates to removing the first term in the
# Taylor series where k=0.
#
# - Mattias Eriksson, 11:e januari 2015
#

# returns (cos(x1)+1)*10000
# x1 = 0..360 (degrees)
program (x1)
    # make sure input is between 0 and 360, inclusive
    while x1 != 0 do
        x2 := succ(x1)
        x3 := 360
        while x3 != 0 do
            x3 := pred(x3)
            x1 := pred(x1)
        end
    end
    x1 := pred(x2)

    # convert input to radians, mul pi, div 180
    # multiply with 10000*pi
    x3 := 31416
    x1 := pred(x1)
    while x1 != 0 do
        x4 := 31416
        while x4 != 0 do
            x3 := succ(x3)
            x4 := pred(x4)
        end
        x1 := pred(x1)
    end

    # div 180
    x4 := 0
    x1 := succ(x3)
    while x1 != 0 do
        x3 := 180
        while x3 != 0 do
            x3 := pred(x3)
            x1 := pred(x1)
        end
        x4 := succ(x4)
    end

    # x1 in radians * 10000
    x1 := pred(x4)

    # calc (pi-x) and (x-pi), choose the non-negative one (quite non-optimal code)
    x2 := 31416
    x3 := succ(x1)
    x3 := pred(x3)
    while x2 != 0 do
        x2 := pred(x2)
        x3 := pred(x3)
    end
    
    x5 := 0
    x5 := succ(x5)
    while x3 != 0 do
        x5 := 0
        x4 := succ(x3)
        x3 := 0
    end

    while x5 != 0 do
        x5 := 0
        x2 := 31416
        x3 := succ(x1)
        x3 := pred(x3)
        while x3 != 0 do
            x2 := pred(x2)
            x3 := pred(x3)
        end
        x4 := succ(x2)
    end

    # x1 = |x-pi|
    x1 := pred(x4)

    # if x1>=128 (*31416/180), the result will be one off due to rounding errors,
    # so we set a flag to subtract 1 in those cases
    #
    # note, 131 and 132 still manage to create a one off error, though all other
    #       inputs are correct
    x10 := 22340
    x11 := succ(x1)
    x11 := pred(x11)
    while x10 != 0 do
        x10 := pred(x10)
        x11 := pred(x11)
    end

    while x11 != 0 do
        x101 := 0
        x101 := succ(x101)
        x11 := 0
    end

    # calculate each term and either add or subtract it from the previous result

    # the current taylor series term
    x2 := 0
    x2 := succ(x2) # start at 1 to make the loop cleaner

    # add or subtract flags, x3=1 -> add, x4=1 -> sub
    x3 := 0
    x3 := succ(x3)
    x4 := 0

    # 2^k term
    x5 := 0

    # the sum of the series
    x100 := 0

    # 10^4 divisor flag
    x8 := 0

    # calculate more taylor terms as long as the current one is not zero
    while x2 != 0 do
        # calculate each term in two passes
        x6 := 0
        x6 := succ(x6)
        x6 := succ(x6)

        while x6 != 0 do
            x6 := pred(x6)
            
            # mul |x-pi|
            x7 := succ(x1)
            x7 := pred(x1)

            x9 := succ(x2)

            while x7 != 0 do
                x7 := pred(x7)
                x11 := pred(x9)
                while x11 != 0 do
                    x11 := pred(x11)
                    x2 := succ(x2)
                end
            end

            # div 10000
            while x8 != 0 do
                x8 := 0
                x9 := succ(x2)
                x11 := 0
                while x9 != 0 do
                    x12 := 10000
                    while x12 != 0 do
                        x12 := pred(x12)
                        x9 := pred(x9)
                    end
                    x11 := succ(x11)
                end
                x2 := pred(x11)
            end
            x8 := 0
            x8 := succ(x8)

            # div with the next factorial factor
            x11 := succ(x2)
            x12 := 0
            while x11 != 0 do
                x13 := succ(x5)
                while x13 != 0 do
                    x13 := pred(x13)
                    x11 := pred(x11)
                end
                x12 := succ(x12)
            end
            x2 := pred(x12)
            x5 := succ(x5)
        end

        # add taylor term x2 to sum x100
        x4 := 0
        x4 := succ(x4)
        while x3 != 0 do
            x3 := 0
            x4 := 0
            x6 := succ(x2)
            x6 := pred(x6)
            while x6 != 0 do
                x6 := pred(x6)
                x100 := succ(x100)
            end
        end
        
        # sub taylor term x2 from sum x100
        while x4 != 0 do
            x3 := 0
            x3 := succ(x3)
            x4 := 0
            x6 := succ(x2)
            x6 := pred(x6)
            while x6 != 0 do
                x6 := pred(x6)
                x100 := pred(x100)
            end
        end
    end

    while x101 != 0 do
        x101 := 0
        x100 := pred(x100)
    end

result (x100)
