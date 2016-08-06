
void STRIP(char* dest, char* string, char option, char pad) __naked
{dest, string, option, pad;
__asm
    POP     AF
    POP     HL	; dest
    POP     DE	; string
    POP     BC	; option/pad
	push	bc
	push	de
	push	hl
	push	af
	ld  a,c
	ld  c,b
	ld  b,a

	ld	a,c
	cp	a,#' '
	jr	nc,100$
	ld	c,#' '	; pad = default
100$:	ld	a,b
	cp	a,#'B'
	jr	nc,101$
	ld	b,#'B'
101$:	ld	a,b
	cp	a,#'L'
	jr	z,103$
	cp	a,#'l'
	jr	z,103$
	cp	a,#'B'
	jr	z,103$
	cp	a,#'b'
	jr	z,103$
	cp	a,#'T'
	jr	z,105$
	cp	a,#'t'
	jr	z,105$

103$:
    ld	a,(de)	;skip leading chars
	or	a,a
	jp	z,1$
	cp	a,c	; compare with pad-char.
	jr	nz,104$
	inc	de
	jr	103$
104$:	ld 	a,b
	cp	a,#'B'
	jr	z,105$
	cp	a,#'b'
	jr	z,105$

2$: ld	a,(de)	;skip leading chars
	or	a,a
	jr	z,1$
	ld	(hl),a
	inc	hl
	inc	de
	jr	2$

105$:	push	hl	;dest
	push	de	;string
	push	bc

	ld	h,d
	ld	l,e
	ld	bc,#256
	xor	a,a
	cpir
	dec	hl
	dec hl

	pop	bc

3$:	ld	a,(hl)	;skip trailing chars
	cp	a,c	; compare with pad-char.
	jr	nz,4$
	dec	hl
	jr	3$
4$:	inc	hl
	or	a,a
	sbc	hl,de
	ld	b,h
	ld	c,l
	pop	hl	; change ptrs for ldir
	pop 	de
	ldir
	ex de,hl

1$:     ld	(hl),#0
	RET
__endasm;
}
void CHANGESTR(char* dest, char* needle, char* haystack, char* newneedle) __naked
{ dest,needle,haystack, newneedle;
__asm

        LD      (1$+1),SP
        POP     AF
        POP     HL
        LD      (2$+1),HL
        POP     DE
        LD      (3$+1),DE
        POP     HL
        POP     BC
        LD      (4$+1),BC

3$:     LD      DE,#0    ; var-in-opcode
5$:     LD      A,(HL)
        OR      A,A
        JR      Z,1$
        LD      A,(DE)
        OR      A,A
        JR      Z,6$

        CP      (HL)
        JP      NZ,2$
        INC     DE
        INC     HL
        JP      5$
2$:     LD      DE,#0    ; var-in-opcode
        LD      A,(HL)
        LD      (DE),A
        INC     HL
        INC     DE
        LD      (2$+1),DE
        JP      3$
6$:
        LD      DE,(2$+1)
7$:     LD      A,(BC)
        OR      A,A
        JR      Z,4$
        LD      (DE),A
        INC     BC
        INC     DE
        JP      7$
4$:
        LD      BC,#0    ; var-in-opcode
        LD      (2$+1),DE
;        INC     HL
        JP      3$
1$:
        LD      SP,#0   ; var-in-opcode
        LD      HL,(2$+1)
        LD      (HL),A
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

int COMPARE(char* s1, char* s2, char pad) __naked
{s1, s2, pad;
__asm
        LD      (3$+1),SP
        POP     AF
        POP     DE      ; s1
        POP     HL      ; s2
        POP     BC      ; C = padchar
        LD      B,#01   ; char counter
        LD      A,C
        OR      A,A
        JR      NZ,4$
        LD      C,#0x20

4$:     XOR     A,A
        CP      (HL)
        JR      Z,5$
        LD      A,(DE)
        OR      A,A
        JR      Z,6$
        CP      (HL)
        JR      NZ,2$
        INC     HL
        INC     DE
        INC     B
        JP      4$
5$:     LD      A,(DE)
        OR      A,A
        JP      Z,1$
        CP      C
        JP      NZ,2$
        INC     DE
        INC     B
        JP      5$
6$:     EX      DE,HL
        JP      5$
1$:     LD      HL,#0
        JP      3$
2$:     XOR     A,A
        CP      (HL)
        JR      NZ,9$
        LD      B,A
9$:     LD      H,A
        LD      L,B
3$:     LD      SP,#0
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void COMPRESS(char* d, char* s, char* list) __naked
{   d, s, list;
__asm
        ld      hl,#7
        add     hl,sp
        ld      b,(hl)
        dec     hl
        ld      c,(hl)
        dec     hl
        ex      de,hl
        ld      h,b
        ld      l,c
        ld      a,b
        or      a,c
        jr      z,10$
        xor     a,a
1$:
        cp      (hl)
        jr      z,2$
        inc     hl
        jr      1$
2$:     or      a,a
        sbc     hl,bc
10$:    ld      (3$+1),hl ; list lenght
        ld      (4$+1),bc ; list pointer
        ex      de,hl
5$:
        ld      d,(hl)
        dec     hl
        ld      e,(hl)
        dec     hl
        ld      a,(hl)
        dec     hl
        ld      l,(hl)
        ld      h,a
        ex      de,hl
6$:   ld      a,(hl)
        or      a,a
        jr     z,11$ ; source string end
        ld      a,(3$+1)
        or      a,a
        ld      a,(hl)
        jr      z,7$
         push    hl
3$:      ld      bc,#0
4$:      ld      hl,#0
         cpir
         pop     hl
        jr      z,8$
        jp      9$
7$:     cp      #0x20
        jr      z,8$
9$:     ld      (de),a
        inc     de
8$:     inc     hl
        jr      6$
11$:    ex      de,hl
        ld      (hl),#0
        ret
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void COPIES(char* d, char* s, unsigned char num) __naked
{d, s, num;
__asm

        POP     AF
        POP     DE
        POP     HL
        POP     BC
        PUSH    BC
        PUSH    HL
        PUSH    DE
        PUSH    AF

        PUSH    BC
        PUSH    HL
        XOR     A,A
        LD      B,A
        LD      C,A
        CPIR
        XOR     A,A
        SUB     C
        DEC     A
        POP     HL
        POP     BC
        LD      B,A     ; lenght of src sring

1$:     PUSH    BC
        PUSH    HL
        LD      C,B
        LD      B,#0
        LDIR
        POP     HL
        POP     BC
        DEC     C
        JP      NZ,1$

        XOR     A,A
        LD      (DE),A
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

int COUNTSTR(char* needle, char* haystack) __naked
{needle, haystack;
__asm
        POP     AF
        POP     DE      ; needle
        EX      (SP),HL ; haystack
        PUSH    DE
        PUSH    AF
        LD      C,#0    ; needle counter
1$:     PUSH    DE
2$:     XOR     A,A
        CP      (HL)
        JR      Z,3$
        LD      A,(DE)
        OR      A,A
        JP      Z,4$
        CP      (HL)
        JP      NZ,5$
        INC     HL
        INC     DE
        JP      2$
5$:     INC     HL
        POP     DE
        JP      1$
4$:     INC     C
        INC     HL
        POP     DE
        JP      1$
3$:     LD      H,#0
        LD      L,C
        POP     DE
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void DELSTR(char* d, char* s, int start, int lenght) __naked
{d, s, start, lenght;
__asm
        LD      (1$+1),SP
        POP     AF
        POP     DE      ; d
        POP     HL      ; s
        POP     BC      ; start
        LD      A,C
        OR      A,A
        JR      Z,6$
        DEC     C       ; 1to0 start correction
4$:     LD      A,(HL)
        LD      (DE),A
        OR      A,A
        JR      Z,2$
        INC     HL
        INC     DE
        DEC     C
        JP      NZ,4$
        POP     BC      ; lenght
        LD      A,C
        OR      A,A
        JP      Z,2$
        LD      B,#00
        ADD     HL,BC
5$:     LD      A,(HL)
        LD      (DE),A
        OR      A,A
        JR      Z,1$
        INC     HL
        INC     DE
        JP      5$

6$:     LD      HL,#9$  ; error start absent
        LD      BC,#11
        LDIR
        JP      2$
2$:     XOR     A,A
        LD      (DE),A
1$:     LD      SP,#0
        RET
9$:     .ASCII     "*=NOSTART=*"
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void LEFT(char* d, char* s, unsigned char lenght, char pad) __naked
{d, s, lenght, pad;
__asm
        POP     AF
        POP     HL      ; d
        POP     DE      ; s
        POP     BC      ; pad/lenght
        PUSH    BC
        PUSH    DE
        PUSH    HL
        PUSH    AF

        LD      A,B
        OR      A
        JR      NZ,1$
        LD      B,#0x20
1$:     LD      A,(DE)
        OR      A
        JR      Z,3$
        LD      (HL),A
        INC     HL
        INC     DE
        DEC     C
        JP      NZ,1$
2$:     LD      (HL),#0
        RET
3$:     LD      (HL),B
        INC     HL
        DEC     C
        JP      NZ,3$
        JP      2$
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void RIGHT(char* d, char* s, unsigned char lenght, char pad) __naked
{d, s, lenght, pad;
__asm
    LD      (1$+1),SP
    POP     AF

    POP     HL      ; d
    POP     DE      ; s
    pop     bc  ; c = len,  b = pad

    ld  a,b
    cp  a,#' '
    jr  nc,2$
    ld  a,#' '  ; if pad char < 0x20, then pad char = 0x20.
2$: ex  af,af
    ld  b,#0
    ADD  HL,BC
    INC C
    PUSH HL ; end of dest

    LD  B,C

    LD  H,D
    LD  L,E

    PUSH BC
    LD  B,#1
    CPIR
    POP BC

    OR  A,A

    PUSH HL
    SBC HL,DE
    LD  A,L
    POP DE
    DEC DE

    POP HL
    LD  C,A


7$: LD  A,(DE)
    LD  (HL),A
    DEC HL
    DEC DE
    DEC C
    JR  Z,3$
    DJNZ 7$
    JR  1$
3$: EX  AF,AF
5$: DJNZ    4$
1$: LD      SP,#0
    RET
4$: LD  (HL),A
    DEC HL
    JR  5$
__endasm;
}



/*--------------------------------- Cut here ---------------------------------*/

int LENGTH(char* string) __naked
{ string;
__asm
        POP     AF
        EX      (SP),HL
        PUSH    AF
        LD      D,H
        LD      E,L
        XOR     A
        LD      B,A
        LD      C,A
        CPIR
        SBC     HL,DE
        DEC     HL ; without ending ZERO.
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void REVERSE(char* d, char* s) __naked
{ d,s;
__asm
        LD      (1$+1),SP
        POP     AF
        POP     DE      ; d
        POP     HL      ; s
        LD      BC,#0
        XOR     A
        CPIR
        XOR     A
        SUB     C
        LD      C,A
        JR      NC,2$
        INC     B
2$:     XOR     A
        SUB     B
        LD      B,A
        DEC     HL
        DEC     HL
        DEC     BC
3$:     LD      A,(HL)
        LD      (DE),A
        INC     DE
        DEC     HL
        DEC     BC
        LD      A,B
        OR      C
        JP      NZ,3$
        LD      (DE),A
1$:     LD      SP,#0
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void SPACE(char* d, char* s, unsigned char len, char pad) __naked
{ d, s, len, pad;
__asm
        LD      (10$+1),SP
        POP     AF
        POP     DE      ; d
        POP     HL      ; s

        PUSH    HL
        LD      BC,#0
        XOR     A
        CPIR
        DEC     HL
        DEC     HL
        LD      A,#0x20
1$:     DEC     HL
        CP      (HL)
        JR      Z,1$
        INC     HL
        LD      A,(HL)
        LD      (9$+1),A
        LD      (HL),#0 ; zaboy
        POP     HL

        POP     BC      ; pad/len

        LD      A,C
        OR      A
        JR      NZ,2$
        INC     C
2$:     LD      A,B
        OR      A
        JR      NZ,3$
        LD      B,#0x20

3$:     LD      A,#0x20
4$:     CP      (HL)
        JR      NZ,5$
        INC     HL
        JP      4$
5$:     LD      A,(HL)
        OR      A
        JR      Z,9$
        CP      #0x20
        JR      Z,6$
        LD      (DE),A
        INC     DE
        INC     HL
        JP      5$
6$:     LD      A,C
        EX      DE,HL
7$:     LD      (HL),B
        INC     HL
        DEC     A
        JP      NZ,7$
        EX      DE,HL
        LD      A,#0x20
8$:     INC     HL      ; skip spaces
        CP      (HL)
        JP      Z,8$
        JP      4$

9$:     LD      A,#0    ; var-in-opcode
        LD      (HL),A
        EX      DE,HL
        LD      (HL),#0
10$:    LD      SP,#0
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void TRIM(char* d, char* s) __naked
{ d,s;
__asm
        POP     AF
        POP     DE
        EX      (SP),HL
        PUSH    DE
        PUSH    AF

        PUSH    HL
        XOR     A,A
        LD      B,A
        LD      C,A
        CPIR
        PUSH    HL
        LD      H,A
        LD      L,A
        SBC     HL,BC
        LD      B,H
        LD      C,L
        POP     HL
        LD      A,#0x20
        DEC     HL
1$:     DEC     HL
        DEC     BC
        CP      (HL)
        JR      NZ,2$
        JP      1$
2$:     POP     HL
        LDIR
        EX      DE,HL
        LD      (HL),#0
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void UPPER(char* d, char* s) __naked
{ s,d;
__asm
        POP     AF
        POP     DE
        EX      (SP),HL
        PUSH    DE
        PUSH    AF
 5$:    LD      A,(HL)
        OR      A,A
        JR      Z,1$
        LD      C,A
        CP      #0x7B
        JR      NC,4$
        LD      A,#0x60
        CP      C
        JR      NC,2$
        JP      3$
4$:     CP      #0xE0
        JR      C,2$
3$:     LD      A,C
        SUB     #0x20
        LD      C,A
2$:     LD      A,C
        LD      (DE),A
        INC     HL
        INC     DE
        JR      5$
1$:     LD      (DE),A
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void LOWER(char* d, char* s) __naked
{ s,d;
__asm
    POP     AF
    POP     DE
    EX      (SP),HL
    PUSH    DE
    PUSH    AF
 5$:
    LD      A,(HL)
	ld      c,a
	OR      A,A
    JR      Z,1$

	cp	a,#0xa8
	jr	nz,3$
	add	a,#0x10
	jr	100$

3$:	CP  a,#65
	jr	c,100$
	ld	a,#90
	cp	a,c
	jr	c,4$
	ld	a,c
	add	a,#0x20
	jr	100$

4$:	ld	a,c
	cp	#0xc0
	jr	c,100$
	ld	a,#0xdf
	cp	a,c
	jr	c,99$
	ld	a,c
	add	a,#0x20
	jr	100$
99$: ld  a,c
100$:

    ld	(de),a
	inc	de
	inc	hl
	jr	5$
1$:     LD      (DE),A
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void WORD(char* dst, char* string, int WordNum) __naked /* (DE,HL,B) */
{ dst, string, WordNum;
__asm
; copy WordNum word to dst.
        POP     AF
        POP     DE
        POP     HL
        POP     BC
        PUSH    BC
        PUSH    HL
        PUSH    DE
        PUSH    AF
        LD      B,C
        LD      C,#0 ; word counter
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
4$:     INC     C
        LD      A,B
        CP      C
        JR      Z,5$
3$:     INC     HL
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      NZ,3$
2$:     INC     HL
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
        JR      4$
1$:     LD      L,C
        RET
5$:     LD      A,(HL)
        OR      A,A
        JR      Z,6$
        CP      #0x20
        JR      Z,6$
        LD      (DE),A
        INC     HL
        INC     DE
        JR      5$
6$:     INC     DE
        XOR     A,A
        dec     de
        LD      (DE),A
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

int WORDPOS(char* phrase, char* string, int startpos) __naked  /* (DE, HL, B) */
{ phrase, string, startpos;
__asm
; return WordNumber or 0. lenght of target word
        POP     AF
        POP     DE
        POP     HL
        POP     BC
        PUSH    BC
        PUSH    HL
        PUSH    DE
        PUSH    AF

        LD      B,C
        LD      C,#0 ; word counter
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
4$:     INC     C
        LD      A,B
        CP      C
        JR      Z,5$
3$:     INC     HL
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      NZ,3$
2$:     INC     HL
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
        JR      4$
1$:     LD      HL,#0
        RET
5$:     PUSH    DE
6$:     LD      A,(DE)
        OR      A,A
        JR      Z,7$
        CP      #0x20
        JR      NZ,8$
        INC     DE
        JR      6$
8$:     LD      A,#0x20
9$:     CP      (HL)
        JR      NZ,10$
        INC     HL
        JR      9$
10$:    LD      A,(DE)
        CP      (HL)
        JR      NZ,13$
        INC     HL
        INC     DE
        JR      6$
13$:    POP     DE
        LD      A,#0x20
12$:
        CP      (HL)
        INC     HL
        JR      NZ,12$
11$:
        CP      (HL)
        INC     HL
        JR      Z,11$
        DEC     HL
        INC     C
        JR      5$
        RET
7$:     POP     DE
        LD      H,#0
        LD      L,C
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

int WORDLENGHT(char* string, int WordNum) __naked  /* (HL, B) */
{ string, WordNum;
__asm
; return lenght of target word
        POP     AF
        POP     HL
        POP     BC
        PUSH    BC
        PUSH    HL
        PUSH    AF
        LD      B,C
        LD      C,#0 ; word 5$er
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
4$:     INC     C
        LD      A,B
        CP      C
        JR      Z,5$
3$:     INC     HL
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      NZ,3$
2$:     INC     HL
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
        JR      4$
1$:     LD      L,C
        LD      H,#0
        RET
5$:     LD      C,#1 ; chars counter
6$:     INC     HL
        LD      A,(HL)
        CP      #0x21
        JR      C,1$
        INC     C
        JR      6$
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

int WORDINDEX(char* string, int WordNum) __naked  /*(HL, B) */
{ string, WordNum;
__asm
; return position of target word in chars
        POP     AF
        POP     HL
        POP     BC
        PUSH    BC
        PUSH    HL
        PUSH    AF
        LD      B,C ; WordNum
        LD      DE,#0 ; chars counter
        LD      C,#0 ; word counter
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
3$:     INC     C
        LD      A,B
        CP      C
        JR      Z,5$
4$:     INC     HL
        INC     DE
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      NZ,4$
2$:     INC     HL
        INC     DE
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
        JR      3$
1$:     LD      DE,#0
        RET
5$:     EX      DE,HL
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

int WORDS(char* string) __naked  /* ( HL ) */
{ string;
__asm
; return number of words in string
        pop     af
        pop     hl
        push    hl
        push    af
        LD      BC,#0 ; word counter
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
4$:     INC     C
3$:     INC     HL
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      NZ,3$
2$:     INC     HL
        LD      A,(HL)
        OR      A,A
        JR      Z,1$
        CP      #0x20
        JR      Z,2$
        JR      4$
1$:     LD      L,C
        LD      H,B
        RET
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void CENTER(char* dest, char* string, unsigned char len, char pad)
{ dest, string; len; pad;
__asm
    POP     AF  ; ret
    pop     de  ; dest
    POP     HL  ; string
    POP     BC  ; B = pad, C = len;
    push    bc
    push    hl
    push    de
    push    af

    ld  a,b
    cp  a,#' '
    jr  nc,2$
    ld  a,#' '  ; if pad char < 0x20, then pad char = 0x20.
2$: ex  af,af

    push bc
    ld  b,#1
    push de
    push hl
    ld  d,h
    ld  e,l
    xor a,a
    cpir
    dec hl
    or  a,a
    sbc hl,de
    ld  a,l
    pop hl
    pop de
    pop bc
    ld  b,c
    ld  c,a ; len of string
    cp  a,b

    jr  c,3$ ; string is shorter
    jr  z,4$ ; string is equal. Simple copy.
;   len is shorter then string
    sub a,b
    srl a
    add a,l
    ld  l,a
    jr  nc,4$
    inc h

4$: ld  c,b


    xor a,a
6$: ld  b,#0
    ldir
    ld  (de),a
    ret

3$: ld  a,b
    sub c
    ld  b,a
    srl a
    jr  z,5$
    push bc
    ld  b,a
    ex  af,af
7$: ld  (de),a
    inc de
    djnz 7$
    ex  af,af
    pop bc

    sub b
    neg
    ld  b,#0
    ldir
    ld  b,a
    ex  af,af
8$: ld  (de),a
    inc  de
    djnz    8$
    xor a,a
    ld  (de),a
    ret

5$: ex  af,af
    call 6$
    inc de
    xor a,a
    ld  (de),a
    ret
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

int FIND(char* string, char* phrase)
{ string; phrase;
__asm
    POP     AF  ; ret
    pop     de  ; string
    POP     HL  ; phrase
    push    hl
    push    de
    push    af

    ld  bc,#0
    push de
5$: push hl
2$: ld  a,(hl)
    or  a,a
    jr  z,100$  ; exit, phrase is found.
    cp  a,#' '
    jr  nz,1$
    inc hl
    jr  2$

1$: ld  a,(de)
    or  a,a
    jr  z,99$   ; exit, phrase not found.
    cp  a,#' '
    jr  nz,3$
    inc de
    jr  1$
3$: inc de
    cp  a,(hl)
    jr  z,4$
    pop hl      ; restore ptr to phrase.
    ld  b,d
    ld  c,e
    jr  5$
4$: inc hl
    jr  2$

99$:
    pop hl
    pop de
    ld  hl,#0
    ret
100$:
    inc bc
    pop de
    pop de
    ld  h,b
    ld  l,c
    or  a,a
    sbc hl,de
    ex  de,hl
    ld  bc,#0   ; word counter

9$: ld  a,d
    or  a,e
    jr  z,7$
    dec de

    ld  a,(hl)
    cp  a,#' '
    jr  nz,8$
    inc hl
    jr  9$

8$: inc bc  ; this is word!
11$:
    ld  a,(hl)
    cp  a,#' '
    jr  nz,10$
    inc hl
    jr  9$
10$:
    inc hl

    ld  a,d
    or  a,e
    jr  z,7$
    dec de
    inc hl
    jr  11$


7$: ; correct from 0
    ld  h,b
    ld  l,c
    ret
__endasm;
}

/*--------------------------------- Cut here ---------------------------------*/

void SUBSTR(char* dest, char* string, unsigned char start, unsigned char len, unsigned char padchar)
{ dest, string, start, len, padchar;
__asm
    push ix
    ld  ix,#0
    add ix,sp

    ld  a,10(ix)
    cp  a,#' '
    jr  nc,1$
    ld  10(ix),#' '
1$: ld  l,6(ix)
    ld  h,7(ix)
    ld  b,#1
    xor a,a
    push hl
    cpir
    dec hl
    ex  de,hl
    pop hl
    ld  c,8(ix)
    dec c   ; correct of start
    ld  b,a
    add hl,bc

    ex  de,hl
    push hl
    or  a,a
    sbc hl,de
    push af

    ld  a,9(ix)
    or  a,a
    jr  nz,9$
    ld  10(ix),l

9$: pop af
    pop hl
    ex  de,hl
    ld  b,9(ix) ; len
    ld  d,5(ix)
    ld  e,4(ix)
    jr  c,2$
3$: ld  a,(hl)
    or  a,a
    jr  z,2$
    ld  (de),a
    inc de
    inc hl
    djnz 3$
    jr  5$
2$: ld  a,10(ix)
4$: ld  (de),a
    inc de
    djnz 4$

5$: xor a,a
    ld  (de),a
    pop ix
    ret
__endasm;
}

