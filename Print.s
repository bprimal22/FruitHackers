; Print.s
; Student names: BP Rimal and Kara Olson
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB



;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
; Lab 7 requirement is for at least one local variable on the stack with symbolic binding
; NO DECIMAL

V  EQU 0
LCD_OutDec
	PUSH{R2, LR}
	SUB SP, #8
	STR R0, [SP, #V]
	
; BASE CASE
	CMP R0, #10
	BLT PRINT
	
	MOV R1, #10
	UDIV R0, R0, R1 		;R0 <- V/10 
	BL LCD_OutDec			; LCD_OUTDEC(V/10)
	LDR R0, [SP, #V]
	MOV R1, #10
	BL MOD
	
PRINT
	ADD R0, R0, #0x30
	BL ST7735_OutChar
	ADD SP, #8
	POP{R2, PC}
	
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.01, range 0.00 to 9.99
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.00 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.89 "
;       R0=123,  then output "1.23 "
;       R0=999,  then output "9.99 "
;       R0>999,  then output "*.** "
; Invariables: This function must not permanently modify registers R4 to R11
; Lab 7 requirement is for at least one local variable on the stack with symbolic binding
N EQU 0
LCD_OutFix
; DECIMALS

	PUSH{R5, LR}
	SUB SP, #8
	STR R0, [SP, #N]
	
	MOV R5, #999
	CMP R0, R5
	BLS CONTINUE
	MOV R0, #0x2A
	BL ST7735_OutChar	; CALLING ST7735_OUTCHAR
	MOV R0, #0x2E
	BL ST7735_OutChar	; CALLING ST7735_OUTCHAR
	MOV R0, #0x2A
	BL ST7735_OutChar	; CALLING ST7735_OUTCHAR
	MOV R0, #0x2A
	BL ST7735_OutChar	; CALLING ST7735_OUTCHAR
	B DONE

CONTINUE
	MOV R1, #100
	UDIV R1, R0, R1		;R1<- Q(N/100) OR MSD
	ADD R0, R1, #0x30	; GIVE R0 (OX30 + N/100)
	BL ST7735_OutChar	; CALLING ST7735_OUTCHAR
	LDR R0, [SP, #N]		; PULL THE ORIGINAL N FROM THE STACK
	MOV R1, #100
	BL MOD				; CALLS THE FUNCITON MOD
	STR R0, [SP, #N]		; R0 HAS MY NEW N; SOTRE IT IN STACK
	MOV R0, #0x2E		; ASCII FOR '.'
	BL ST7735_OutChar	; CALLING ST7735_OUTCHAR
	
	MOV R1, #10
	LDR R0, [SP, #N]
	UDIV R1, R0, R1		;R1<- Q(N/10) OR SMSD
	ADD R0, R1, #0x30	; GIVE R0 (OX30 + N/10)
	BL ST7735_OutChar	; CALLING ST7735_OUTCHAR
	LDR R0, [SP, #N]		; PULL THE ORIGINAL N FROM THE STACK
	MOV R1, #10
	BL MOD				; CALLS THE FUNCITON MOD
	STR R0, [SP, #N]		; R0 HAS MY NEW N; SOTRE IT IN STACK
	ADD R0, #0x30
	BL ST7735_OutChar	; CALLING ST7735_OUTCHAR
	
	
DONE
	ADD SP, #8
    POP{R5, PC}

;INPUT: R0: VALUE TO MOD
;		R1: DIVISOR
;OUTPUT	R0: REMAINDER OF R0/R1
MOD

	UDIV R2, R0, R1
	MUL R2, R2, R1
	SUB R0, R0, R2
	BX LR
	
;     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN          ; make sure the end of this section is aligned
     END            ; end of file
