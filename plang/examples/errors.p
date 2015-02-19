# # #
# Den här filen går inte att kompilera, utan finns endast för att testa
# kompileringsfel.
#
# Följande fel bör genereras:
#   Fel 1     : 'expected , or ), but got X2'
#   Fel 2     : 'this must be zero'
#   Fel 3     : 'invalid identifier'
#   Fel 4     : 'expected ) but got <eof>'
#   Varning 1 : 'assigned value is non-zero'
#   Varning 2 : 'loop is empty and will never finish'
#   Varning 2 : 'the conditional loop var X3 is never modified'
#
# - Philip Arvidsson (philip@philiparvidsson.com), 4:e januari 2015
#

PROGRAM (X1 X2)        # Fel 1: Komma saknas mellan X1 och X2.
    X3 := 1            # Varning 1: Tilldelning med annat värde än noll.
    WHILE X3 != 9 DO   # Fel 3: Det är bara tillåtet att testa mot noll i
                       #        villkor för while-loopar.
        X3 := PRED(X3)
    END
    WHILE X1 != 0 DO
    END                # Varning 2: Tom loop.
    WHILE X3 != 0 DO   # Varning 3: Loop-variabeln modifieras inte.
        X999999 := 0   # Fel 4: Ogiltig variabel.
    END
RESULT (X1             # Fel 5: Höger parentes saknas.
