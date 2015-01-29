# # #
# Programmet nedan r�knar ut fakultet f�r input. Precis som multiplikations-
# programmet bygger vidare p� additionsalgoritmen, s� bygger detta program
# vidare p� multiplikationsalgoritmen.
#
# TIPS: Anv�nd inte h�ga inputv�rden! Programmet ger enorma tal redan vid 10-
#       fakultet, och resultaten tar d� v�ldigt l�ng tid att utv�rdera.
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
