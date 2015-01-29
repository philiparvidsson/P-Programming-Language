# # #
# Detta program räknar ut det X1:e talet i Fibonacci-sekvensen.
#
# - Mattias Eriksson, 4:e januari 2015
#

PROGRAM (X1)

    X1 := PRED(X1)

    X2 := 0
    X3 := 0
    X3 := SUCC(X3)

    WHILE X1 != 0 DO
        X4 := SUCC(X3)
        X4 := PRED(X4)

        X3 := SUCC(X2)
        X3 := PRED(X3)

        X2 := SUCC(X4)
        X2 := PRED(X2)
        
        WHILE X4 != 0 DO
            X4 := PRED(X4)
            X3 := SUCC(X3)
        END

        X1 := PRED(X1)
    END
    
RESULT (X3)
