# # #
# gcd - greatest common divisor
#
# - Mattias Eriksson, 5:e januari 2015
#

PROGRAM (X1, X2)

    # make sure x1>x2
    X20 := SUCC(X1)
    X20 := PRED(X20)

    X21 := SUCC(X2)
    X21 := PRED(X21)

    WHILE X20 != 0 DO
        X20 := PRED(X20)
        X21 := PRED(X21)
    END

    X22 := 0
    WHILE X21 != 0 DO
        X22 := SUCC(X22)
        X21 := 0
    END

    WHILE X22 != 0 DO
        X3 := SUCC(X1)
        X3 := PRED(X3)
        X1 := SUCC(X2)
        X1 := PRED(X1)
        X2 := SUCC(X3)
        X2 := PRED(X2)

        X22 := PRED(X22)
    END

    # then calc gcd
    X100 := 0
    X100 := SUCC(X100)
    WHILE X100 != 0 DO

        X101 := SUCC(X1)
        X101 := PRED(X101)
    
        X1 := SUCC(X1)
        X4 := 0
        WHILE X1 != 0 DO
            X3 := SUCC(X2)
            X3 := PRED(X3)
            WHILE X3 != 0 DO
                X1 := PRED(X1)
                X3 := PRED(X3)
            END
            X4 := SUCC(X4)
        END

        # x4,x104 = floor(x1/x2)
        X104 := PRED(X4)
        X4 := PRED(X4)
    
        # remainder (x1/x2) = x1 - x4 * x2
        
        X11 := SUCC(X101)
        X11 := PRED(X11)

        X6 := SUCC(X2)
        X6 := PRED(X6)
    
        X1 := SUCC(X2)
        X1 := PRED(X1)
    
        WHILE X4 != 0 DO
            X4 := PRED(X4)

            WHILE X2 != 0 DO
                X11 := PRED(X11)
                X2 := PRED(X2)
            END

            X2 := SUCC(X6)
            X2 := PRED(X2)
        END

        # remainder in X11, check if it's zero, otherwise loop
        X7 := 0
        X7 := SUCC(X7)
        
        X12 := SUCC(X11)
        X12 := PRED(X12)
        
        WHILE X12 != 0 DO
            X7 := PRED(X7)
            X12 := PRED(X12)
        END
    
        WHILE X7 != 0 DO
            X100 := PRED(X100)
            X7 := PRED(X7)
        END

        X2 := SUCC(X11)
        X2 := PRED(X2)
    
        X1 := SUCC(X6)
        X1 := PRED(X1)
    END
RESULT(X1)
