# # #
# Programmet nedan räknar ut fakultet för input. Precis som multiplikations-
# programmet bygger vidare på additionsalgoritmen, så bygger detta program
# vidare på multiplikationsalgoritmen.
#
# TIPS: Använd inte höga inputvärden! Programmet ger enorma tal redan vid 10-
#       fakultet, och resultaten tar då väldigt lång tid att utvärdera.
#
# - Philip Arvidsson (philip@philiparvidsson.com), 4:e januari 2015
#

PROGRAM (X5)
    X1 := 0
    X1 := SUCC(X1)
    WHILE X5 != 0 DO
        X3 := PRED(X1)
        X1 := 0
        X2 := SUCC(X5)
        X2 := PRED(X2)
        WHILE X2 != 0 DO
            X4 := SUCC(X3)
            WHILE X4 != 0 DO
                X1 := SUCC(X1)
                X4 := PRED(X4)
            END
            X2 := PRED(X2)
        END
        X5 := PRED(X5)
    END
RESULT (X1)
