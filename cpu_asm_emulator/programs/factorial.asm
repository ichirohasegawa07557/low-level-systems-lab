; 5! = 120
MOV R0, 5
MOV R1, 1
MOV R2, 1
fact_loop:
MUL R1, R1, R0
SUB R0, R0, R2
CMP R0, 1
JG fact_loop
PRINT R1
HALT
