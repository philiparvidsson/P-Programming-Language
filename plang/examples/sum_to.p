# # #
# Det här programmet summerar alla tal upp till och med input.
#
# - Philip Arvidsson (philip@philiparvidsson.com), 4:e januari 2015
#

PROGRAM (X1)
    X3 := 0
    WHILE X1 != 0 DO
        X1 := PRED(X1)
        X2 := SUCC(X1)
        WHILE X2 != 0 DO
            X2 := PRED(X2)
            X3 := SUCC(X3)
        END
    END
RESULT (X3)
