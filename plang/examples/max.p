# # #
# max(x1, x2) returns the higher valued number
#
# - Mattias Eriksson, 5:e januari 2015
#

PROGRAM (X1, X2)
# x6 is the amount of numbers to pass before making a simulated if-check, it
# should be set to 100 for optimization purposes
X6 := 100

# save x1 and x2
X10 := SUCC(X1)
X10 := PRED(X10)

X11 := SUCC(X2)
X11 := PRED(X11)

# set the output to the initial x1
X3 := SUCC(X11)

# save the loop variable for resetting
X7 := SUCC(X6)

# decrease both x1 and x2 in steps of x6, then check whether any of x1 or x2
# is 0
WHILE X1 != 0 DO
    # store vars for final pass
    X21 := SUCC(X1)
    X22 := SUCC(X2)
    
    # decrease x1 and x2 by x6
    WHILE X6 != 0 DO
        X1 := PRED(X1)
        X2 := PRED(X2)
        X6 := PRED(X6)
    END

    X6 := PRED(X7)

    # simulate if-check: "if x2 <= 0, then break loop"
    X4 := SUCC(X2)
    X4 := PRED(X4)
    X5 := 0
    X5 := SUCC(X5)
    WHILE X4 != 0 DO
        X5 := PRED(X5)
        X4 := 0
    END
    WHILE X5 != 0 DO
        X1 := 0
        X5 := 0
    END
END

# final pass of max x6 checks, checking at each iteration whichever number hits
# 0 first, the other number is then selected
WHILE X21 != 0 DO
    X21 := PRED(X21)
    X22 := PRED(X22)

    X4 := SUCC(X22)
    X4 := PRED(X4)
    X5 := 0
    X5 := SUCC(X5)
    WHILE X4 != 0 DO
        X5 := PRED(X5)
        X4 := 0
    END
    WHILE X5 != 0 DO
        X21 := 0
        X3 := SUCC(X10)
        X5 := 0
    END
END

X3 := PRED(X3)

RESULT(X3)
