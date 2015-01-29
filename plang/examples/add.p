# # #
# Program f�r addition av X1 och X2.
#
# - Philip Arvidsson (philip@philiparvidsson.com), 4:e januari 2015
#

PROGRAM (X1, X2)
    WHILE X2 != 0 DO   # S� l�nge X2 inte �r noll...
        X1 := SUCC(X1) # ... s� �kar vi X1 med ett ...
        X2 := PRED(X2) # ... och minskar X2 med ett.
    END
RESULT (X1)            # Resultatet finns, som bekant, i X1.
