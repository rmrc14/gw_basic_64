10 PRINT "GW-BASIC SIMPLE CALCULATOR"
20 PRINT "==========================="
30 PRINT
40 INPUT "Enter first number: ", A
50 INPUT "Enter second number: ", B
60 PRINT "Select operation:"
70 PRINT "1. Addition (+)"
80 PRINT "2. Subtraction (-)"
90 PRINT "3. Multiplication (*)"
100 PRINT "4. Division (/)"
110 INPUT "Enter choice (1-4): ", C

120 IF C = 1 THEN RESULT = A + B : PRINT "Result: "; A; " + "; B; " = "; RESULT : GoTo 310
130 IF C = 2 THEN RESULT = A - B : PRINT "Result: "; A; " - "; B; " = "; RESULT : GoTo 310
140 IF C = 3 THEN RESULT = A * B : PRINT "Result: "; A; " * "; B; " = "; RESULT : GoTo 310
150 IF C = 4 THEN IF B = 0 THEN PRINT "Error: Division by zero!" ELSE RESULT = A / B : PRINT "Result: "; A; " / "; B; " = "; RESULT : GoTo 310
160 PRINT "Invalid choice."

310 PRINT
320 INPUT "Do you want to calculate again (Y/N)? ", R$
330 IF R$ = "Y" Or R$ = "y" THEN GOTO 10
340 PRINT "Goodbye!"
