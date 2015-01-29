# # #
# Det här programmet multiplicerar X1 med X2. Testkör det genom att ange värden
# för X1 respektive X2, och verifiera sedan att resultatet motsvarar produkten
# av de angivna värdena.
#
# - Philip Arvidsson (philip@philiparvidsson.com), 4:e januari 2015
#

PROGRAM (X1, X2)
    X3 := SUCC(X1)
    X1 := 0              # Vi nollar X1 så att vi får noll som resultat även om
                         # X1 inte är noll, men X2 är noll.
    WHILE X2 != 0 DO
        X4 := PRED(X3)
        WHILE X4 != 0 DO # Här använder vi oss av additions-algoritmen!
            X1 := SUCC(X1)
            X4 := PRED(X4)
        END
        X2 := PRED(X2)
    END
RESULT (X1)
