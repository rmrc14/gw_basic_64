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

120 IF C = 1 THEN
130     RESULT = A + B
140     PRINT "Result: "; A; " + "; B; " = "; RESULT
150 ELSE IF C = 2 THEN
160     RESULT = A - B
170     PRINT "Result: "; A; " - "; B; " = "; RESULT
180 ELSE IF C = 3 THEN
190     RESULT = A * B
200     PRINT "Result: "; A; " * "; B; " = "; RESULT
210 ELSE IF C = 4 THEN
220     IF B = 0 THEN
230         PRINT "Error: Division by zero!"
240     ELSE
250         RESULT = A / B
260         PRINT "Result: "; A; " / "; B; " = "; RESULT
270     END IF
280 ELSE
290     PRINT "Invalid choice."
300 END IF

310 PRINT
320 INPUT "Do you want to calculate again (Y/N)? ", R$
330 IF UCASE$(R$) = "Y" THEN GOTO 10
340 PRINT "Goodbye!"
