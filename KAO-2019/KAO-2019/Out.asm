.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib kernel32.lib
	includelib ../Debug/StaticLib.lib
	ExitProcess PROTO :DWORD

EXTRN Random: proc
	EXTRN Strlen: proc
	EXTRN Line: proc
	EXTRN OutInt: proc
	EXTRN OutStr: proc

.stack 4096


.const
	divisionByZero db 'ERROR: DIVIDE BY ZERO', 0
	belowZeroNum db 'ERROR: NUM IS BELOVER THAN ZERO', 0
	L1 SDWORD 0
	L2 SDWORD 2
	L3 SDWORD 1
	L4 SDWORD 0
	L5 SDWORD 1
	L6 SDWORD 5
	L7 BYTE "my first kursach", 0
	L8 BYTE "help", 0
	L9 SDWORD 3
	L10 BYTE "even", 0
	L11 BYTE "odd", 0
	L12 SDWORD 36

.data
	mathresult SDWORD 0
	fcheckboo SDWORD 0
	fcheckres SDWORD 0
	gof SDWORD 0
	goran SDWORD 0
	golen DWORD ?
	goboo SDWORD 0
	goeo SDWORD 0
	gox SDWORD 0

.code

math PROC mathone : DWORD, mathtwo : DWORD
	push mathone
	push mathtwo
	pop eax
	pop ebx
	add eax, ebx
	push eax
	push L1
	pop ebx
	mov edx, 0 
	pop eax
	cmp ebx, 0
	je finish
	idiv ebx
	push eax
	pop mathresult
	push mathresult
	jmp local0
	pop eax
	ret
	call exitProcess
	finish:
	push offset divisionByZero
	call outstr
	call exitProcess
local0:
	pop eax
	ret
math ENDP

fcheck PROC fchecknumb : DWORD
	push fchecknumb
	push L2
	pop ebx
	mov edx, 0 
	pop eax
	idiv ebx
	push edx
	mov eax, edx
	pop fcheckres
	mov eax, fcheckres
	cmp eax, L1
	jz m0
	jnz m1
	je m1
m0:
	push L3
	pop fcheckboo
	jmp e0
m1:
	push L4
	pop fcheckboo
e0:
	push fcheckboo
	jmp local1
	pop eax
	ret
	call exitProcess
	finish:
	push offset divisionByZero
	call outstr
	call exitProcess
local1:
	pop eax
	ret
fcheck ENDP

main PROC
	push math
	push L2
	push L5
	pop edx
	pop edx
	push L5
	push L2
	call math
	push eax
	pop gof
	push gof
	call OutInt
	call Line
	push L6
	pop edx
	push L6
	call Random
	push eax
	pop goran
	push goran
	call OutInt
	call Line
	push offset L7
	pop golen
	push L3
	pop goboo
	mov eax, goboo
	cmp eax, 1
	jz m2
	jnz m3
	je m3
m2:
	push golen
	call OutStr
	call Line
	jmp e1
m3:
	push offset L8
	call OutStr
	call Line
e1:
	push fcheck
	push L9
	pop edx
	push L9
	call fcheck
	push eax
	pop goeo
	mov eax, goeo
	cmp eax, 1
	jz m4
	jnz m5
	je m5
m4:
	push offset L10
	call OutStr
	call Line
	jmp e2
m5:
	push offset L11
	call OutStr
	call Line
e2:
	push L12
	pop gox
	finish:
	push offset divisionByZero
	call ExitProcess
main ENDP
end main