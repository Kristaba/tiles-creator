! Generated by Tiles Creator, the graphics resources editor for Casio.
! This file contain the tile allocating function (aTile).

	.section ".text"
	.global _aTile
	.type _aTile, @function
	.align 2

	! aTile function, C prototype : GrayTile aTile(int tileId)
_aTile:
	mov.l	r14, @-r15
	sts	pr, r0
	mov.l	r0, @-r15
	mov.l	r13, @-r15
	mov	r2, r13		! Return struct adress value.
	mov.l	r12, @-r15
	mov.l	r11, @-r15
	mov.l	r10, @-r15
	mov.l	r9, @-r15
	mov.l	TileNumber, r0
	cmp/hi	r4, r0
	bf	Tile_ReturnError

	mov.l	Tile_JumpTableAddress, r0
	shll2	r4		! Offset in jtable * sizeof(int)
	add	r4, r0
	mov.l	@r0, r9
	mov.w	@r9+, r0
	mov.w	r0, @(0, r13)	! Copy Width into return struct
	extu.w	r0, r1
	mov.w	@r9+, r0
	mov.w	r0, @(2, r13)	! Copy Height into return struct
	
	! Get the array size (r12) by operating ((width-1)/8+1)*height	
	add	#-1, r1
	shlr2	r1
	shlr	r1
	add	#1, r1
	mulu.w	r1, r0
	sts	macl, r12	! Store the multiplication result.
	
#ifndef COPY_ON_RAM
	mov	r9, r0
	mov.l	r0, @(4, r13)
	add	r12, r0
	bra	Tile_End
	mov.l	r0, @(8, r13)

#else	
	! Two malloc of size r12*sizeof(char) :
	mov.l	MallocSyscallID, r0
	mov.l	SyscallAddress, r11

	mov	r12, r4
	jsr	@r11
	nop
	tst	r0, r0		! Test malloc return, if == 0, then malloc error...
	bt	Tile_ReturnError
	mov.l	r0, @(4, r13)
	mov	r0, r10

	mov.l	MallocSyscallID, r0
	mov	r12, r4
	jsr	@r11
	nop
	tst	r0, r0		! Test malloc return, if == 0, then malloc error...
	bt	Tile_ReturnError
	mov.l	r0, @(8, r13)
	mov	r0, r11

	! Copy each byte of each bitmap (light and dark) :
	mov	r12, r1		! Number of loops
	mov	r9, r2		! Start address of light bitmap
	mov	r2, r3
	add	r1, r3		! Start address of dark bitmap
	mov	#0, r0
Tile_CopyBitmaps:
	mov.b	@(r0, r2), r4
	mov.b	@(r0, r3), r5
	mov.b	r4, @(r0, r10)
	mov.b	r5, @(r0, r11)
	dt	r1
	bf/s	Tile_CopyBitmaps
	add	#1, r0

	bra	Tile_End
	nop
#endif

Tile_ReturnError:
	! In this case, init the return struct, then end of function.
	mov	#0, r0
	mov.l	r0, @(4, r13)
	mov.l	r0, @(8, r13)
	mov.w	r0, @r13
	mov.w	r0, @(2, r13)

Tile_End:
	mov.l	@r15+, r9
	mov.l	@r15+, r10
	mov.l	@r15+, r11
	mov.l	@r15+, r12
	mov.l	@r15+, r13
	mov.l	@r15+, r0
	lds	r0, pr	
	rts
	mov.l	@r15+, r14


	.align 4
Tile_JumpTableAddress:
	.long	Tile_JTable

