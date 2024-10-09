	.file	"fcall.c"
	.option nopic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	sum_in_place
	.type	sum_in_place, @function
sum_in_place:
	addi	sp,sp,-32
	sd	ra,24(sp)
	sd	s0,16(sp)
	addi	s0,sp,32
	sd	a0,-24(s0)
	mv	a5,a1
	sw	a5,-28(s0)
	ld	a5,-24(s0)
	lw	a5,0(a5)
	lw	a4,-28(s0)
	addw	a5,a4,a5
	sext.w	a4,a5
	ld	a5,-24(s0)
	sw	a4,0(a5)
	ld	a5,-24(s0)
	lw	a5,0(a5)
	mv	a0,a5
	ld	ra,24(sp)
	ld	s0,16(sp)
	addi	sp,sp,32
	jr	ra
	.size	sum_in_place, .-sum_in_place
	.section	.rodata
	.align	3
.LC0:
	.string	"true"
	.align	3
.LC1:
	.string	"false"
	.align	3
.LC2:
	.string	"%d == %d: %s\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sd	ra,40(sp)
	sd	s0,32(sp)
	addi	s0,sp,48
	mv	a5,a0
	sd	a1,-48(s0)
	sw	a5,-36(s0)
	addi	a5,s0,-24
	li	a1,2
	mv	a0,a5
	call	sum_in_place
	mv	a5,a0
	sw	a5,-20(s0)
	lw	a1,-24(s0)
	lw	a5,-24(s0)
	lw	a4,-20(s0)
	sext.w	a4,a4
	bne	a4,a5,.L4
	lui	a5,%hi(.LC0)
	addi	a5,a5,%lo(.LC0)
	j	.L5
.L4:
	lui	a5,%hi(.LC1)
	addi	a5,a5,%lo(.LC1)
.L5:
	lw	a4,-20(s0)
	mv	a3,a5
	mv	a2,a4
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	li	a0,200
	call	sleep
	lw	a5,-24(s0)
	lw	a4,-20(s0)
	sext.w	a4,a4
	sub	a5,a4,a5
	snez	a5,a5
	andi	a5,a5,0xff
	sext.w	a5,a5
	mv	a0,a5
	ld	ra,40(sp)
	ld	s0,32(sp)
	addi	sp,sp,48
	jr	ra
	.size	main, .-main
	.ident	"GCC: (Arch Linux Repositories) 14.1.0"
	.section	.note.GNU-stack,"",@progbits
