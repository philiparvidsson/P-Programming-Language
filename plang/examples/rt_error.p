# # #
# Den h�r filen g�r att kompilera, men b�r krascha vid k�rning p� grund av
# den o�ndliga loopen.
#
# - Philip Arvidsson (philip@philiparvidsson.com), 4:e januari 2015
#

PROGRAM (X1)
    X2 := 0
    X2 := SUCC(X2)
    X3 := 0
    WHILE X2 != 0 DO
        X3 := SUCC(X3)
    END
RESULT (X1)
