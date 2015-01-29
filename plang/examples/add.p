# # #
# Program för addition av X1 och X2.
#
# - Philip Arvidsson (philip@philiparvidsson.com), 4:e januari 2015
#

PROGRAM (X1, X2)
    WHILE X2 != 0 DO   # Så länge X2 inte är noll...
        X1 := SUCC(X1) # ... så ökar vi X1 med ett ...
        X2 := PRED(X2) # ... och minskar X2 med ett.
    END
RESULT (X1)            # Resultatet finns, som bekant, i X1.
