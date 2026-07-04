; Print the 10th Fibonacci number, with F0=0, F1=1 => 55
MOV R0, 0      ; a
MOV R1, 1      ; b
MOV R2, 10     ; remaining iterations
MOV R3, 1      ; decrement
loop:
ADD R4, R0, R1 ; next
MOV R0, R1
MOV R1, R4
SUB R2, R2, R3
CMP R2, 1
JG loop
PRINT R1
HALT
