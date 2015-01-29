# # #
# Det h�r programmet multiplicerar X1 med X2. Testk�r det genom att ange v�rden
# f�r X1 respektive X2, och verifiera sedan att resultatet motsvarar produkten
# av de angivna v�rdena.
#
# - Philip Arvidsson (philip@philiparvidsson.com), 4:e januari 2015
#

PROGRAM (X1, X2)
    X3 := SUCC(X1)
    X1 := 0              # Vi nollar X1 s� att vi f�r noll som resultat �ven om
                         # X1 inte �r noll, men X2 �r noll.
    WHILE X2 != 0 DO
        X4 := PRED(X3)
        WHILE X4 != 0 DO # H�r anv�nder vi oss av additions-algoritmen!
            X1 := SUCC(X1)
            X4 := PRED(X4)
        END
        X2 := PRED(X2)
    END
RESULT (X1)
