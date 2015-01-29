# # #
# Det här programmet dividerar X1 med X2 och returnerar resultatet som ett
# heltal.
#
# - Philip Arvidsson (philip@philiparvidsson.com), 4:e januari 2015
#

PROGRAM (X1, X2)
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
    X4 := PRED(X4)
RESULT (X4)
