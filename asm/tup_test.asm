section .data

; TAKEN FROM https://github.com/davidad/asm_concurrency/blob/master/os_dependent_stuff.asm
; syscalls
%ifidn __OUTPUT_FORMAT__,elf64
; http://lxr.linux.no/linux+v3.13.5/arch/x86/syscalls/syscall_64.tbl
  %define SYSCALL_OPEN      2
  %define SYSCALL_WRITE     1
  %define SYSCALL_READ      0
  %define SYSCALL_MMAP      9
  %define SYSCALL_FTRUNCATE 77
  %define SYSCALL_PWRITE    18
  %define SYSCALL_FORK      57
  %define SYSCALL_WAITID    247
  %define SYSCALL_EXIT      60
%elifidn __OUTPUT_FORMAT__,macho64
; http://www.opensource.apple.com/source/xnu/xnu-1456.1.26/bsd/kern/syscalls.master
  %define SYSCALL_OPEN      0x2000005
  %define SYSCALL_WRITE     0x2000004
  %define SYSCALL_READ      0x2000003
  %define SYSCALL_MMAP      0x20000C5
  %define SYSCALL_FTRUNCATE 0x20000C9
  %define SYSCALL_PWRITE    0x200009A
  %define SYSCALL_FORK      0x2000002
  %define SYSCALL_WAITID    0x20000AD
  %define SYSCALL_EXIT      0x2000001
%endif

; fcntls
%ifidn __OUTPUT_FORMAT__,elf64
; http://lxr.linux.no/linux+v3.13.5/include/uapi/asm-generic/fcntl.h
  %define O_RDONLY      0o0000000
  %define O_WRONLY      0o0000001
  %define O_RDWR        0o0000002
  %define O_CREAT       0o0000100
  %define O_EXCL        0o0000200
  %define O_NOCTTY      0o0000400
  %define O_TRUNC       0o0001000
  %define O_APPEND      0o0002000
  %define O_NONBLOCK    0o0004000
  %define O_DSYNC       0o0010000
  %define FASYNC        0o0020000
  %define O_DIRECT      0o0040000
  %define O_LARGEFILE   0o0100000
  %define O_DIRECTORY   0o0200000
  %define O_NOFOLLOW    0o0400000
  %define O_NOATIME     0o1000000
  %define O_CLOEXEC     0o2000000
  %define O_SYNC        0o4000000
  %define O_PATH        0o10000000
%elifidn __OUTPUT_FORMAT__,macho64
; http://www.opensource.apple.com/source/xnu/xnu-1456.1.26/bsd/sys/fcntl.h
  %define O_RDONLY      0x0000
  %define O_WRONLY      0x0001
  %define O_RDWR        0x0002
  %define O_NONBLOCK    0x0004
  %define O_APPEND      0x0008
  %define O_SHLOCK      0x0010
  %define O_EXLOCK      0x0020
  %define O_ASYNC       0x0040
  %define O_SYNC        0x0080
  %define O_NOFOLOW     0x0100
  %define O_CREAT       0x0200
  %define O_TRUNC       0x0400
  %define O_EXCL        0x0800
  %define O_NOCTTY      0x20000
  %define O_DIRECTORY   0x100000
  %define O_SYMLINK     0x200000
  %define O_DSYNC       0x400000
%endif

; mmap() and mprotect() flags
%ifidn __OUTPUT_FORMAT__,elf64
; http://lxr.linux.no/linux+v3.13.5/include/uapi/asm-generic/mman-common.h
  %define MAP_SHARED   0x01
  %define MAP_PRIVATE  0x02
  %define MAP_FIXED    0x10
  %define MAP_ANON     0x20
  %define PROT_NONE     0b0000
  %define PROT_READ     0b0001
  %define PROT_WRITE    0b0010
  %define PROT_EXEC     0b0100
  %define PROT_SEM      0b1000
  %define PROT_GROWSDOWN 0x01000000
  %define PROT_GROWSUP   0x02000000
%elifidn __OUTPUT_FORMAT__,macho64
; http://www.opensource.apple.com/source/xnu/xnu-2050.18.24/bsd/sys/mman.h
  %define MAP_SHARED    0b000000000001
  %define MAP_PRIVATE   0b000000000010
  %define MAP_FIXED     0b000000010000
  %define MAP_RENAME    0b000000100000
  %define MAP_NORESERVE 0b000001000000
  %define MAP_INHERIT   0b000010000000
  %define MAP_NOEXTEND  0b000100000000
  %define MAP_SEMAPHORE 0b001000000000
  %define MAP_NOCACHE   0b010000000000
  %define MAP_JIT       0b100000000000
  %define MAP_FILE      0x0000
  %define MAP_ANON      0x1000
  %define PROT_NONE     0b000
  %define PROT_READ     0b001
  %define PROT_WRITE    0b010
  %define PROT_EXEC     0b100
%endif

; wait() flags are shared
; http://lxr.linux.no/linux+v3.13.5/include/uapi/linux/wait.h
%define WEXITED      0x04
%define P_ALL        0
%define P_PID        1
%define P_PGID       2
%define ECHILD       10

section .bss
  _2_buffer:    resb 100
  .len: equ $ - _2_buffer
  _3_buffer:    resb 21
  _3_registers: resq 1
  _5_buffer:    resb 30

default rel
section .text
_1: ; print
  mov rsi, [rsp+8]
  mov rdx, [rsi-8]
  mov rax, SYSCALL_WRITE
  mov rdi, 1
  syscall
  ret 8

_2:
  mov rax, SYSCALL_READ ; read
  mov rdi, 0 ; stream
  lea rsi, [_2_buffer]
  mov rdx, _2_buffer.len
  syscall
  push rax

  ; --- TO BE REPLACED WITH BETTER ALLOCATION CODE
  lea rsi, [rax+8]
  mov rax, SYSCALL_MMAP
  xor edi, edi
  mov rdx, PROT_READ | PROT_WRITE
  mov r10, MAP_PRIVATE | MAP_ANON
  mov r8, -1
  xor r9d, r9d
  syscall
  ; ---

  pop rcx
  mov [rax], rcx
  lea rdi, [rax+8]
  mov [rsp+8], rdi
  lea rsi, [_2_buffer]
  cld
  rep movsb
  ret

_3:
  mov rax, [rsp+8]    ; load integer
  lea rcx, [_3_buffer+20] ; load buffer adress
  mov rsi, 10         ; load divider

.loop:
  mov rdx, 0          ; clear most significant part of dividend
  div rsi             ; divide by 10
  add rdx, '0'        ; add '0' character
  mov [rcx], dl       ; write digit to string
  test rax, rax       ; if non-zero
  loopne .loop        ; decrease rcx and jump to _iota_loop
  inc rcx
  mov [_3_registers], rcx
  ;push rcx

  ; --- TO BE REPLACED WITH BETTER ALLOCATION CODE
  mov rax, SYSCALL_MMAP
  xor edi, edi
  lea rsi, [21+8+_3_buffer]
  sub rsi, rcx 
  mov rdx, PROT_READ | PROT_WRITE
  mov r10, MAP_PRIVATE | MAP_ANON
  mov r8, -1
  xor r9d, r9d
  syscall
  ; ---

  lea rcx, [rsi-8]  ; load length
  mov [rax], rcx    ; write length to string
  mov rsi, [_3_registers] ; get read position
  ;pop rsi
  lea rdi, [rax+8]  ; get write position
  mov [rsp+16], rdi ; set pointer as return value
  cld
  rep movsb         ; copy string 

  ret 8               ; return

_4: ; concat
  ; reserve string
  mov rax, [rsp+8]  ; load first string address
  mov rsi, [rax-8]  ; load first string size
  mov rax, [rsp+16] ; load second string address
  add rsi, [rax-8]  ; add second string size
  add rsi, 8        ; add string header size
  mov rax, SYSCALL_MMAP
  xor edi, edi
  mov rdx, PROT_READ | PROT_WRITE
  mov r10, MAP_PRIVATE | MAP_ANON
  mov r8, -1
  xor r9d, r9d
  syscall
  ; write length to string
  sub rsi, 8
  mov [rax], rsi
  ; copy first string
  mov rsi, [rsp+16] ; load first string adress
  mov rcx, [rsi-8]  ; load first string size
  mov rdx, rcx      ; save first string size
  lea rdi, [rax+8]  ; load write adress
  mov [rsp+24], rdi ; push return value
  cld 
  rep movsb     ; copy string
  ; copy second string
  mov rsi, [rsp+8]  ; load second string adress
  mov rcx, [rsi-8]  ; load second string size
  ; add rdi, rdx    ; load write adress
  rep movsb         ; copy string
  ; return
  ret 16

_5:
  ; rsp+8 : float
  mov r9, 0
  mov rdi, [rsp+8]
  mov rsi, 0b1000000000000000000000000000000000000000000000000000000000000000
  and rsi, rdi
  je .non_negative
  mov byte [r9+_5_buffer], '-'
  inc r9
  xor rdi, rsi
.non_negative:
  mov rsi, rdi
  shr rsi, 52  ; rsi = exponent
  mov rdx, 0b0000000000001111111111111111111111111111111111111111111111111111
  and rdi, rdx ; rdi = fraction
  test rsi, 0b11111111111
  je .sub_normal
  cmp rsi, 2047
  je .nan_and_infty
  
  ; regular float
  mov word [r9+_5_buffer], '1.'
  add r9, 2
  mov rcx, 52
  mov rdx, 500000000000000000
  mov r10, 0b1000000000000000000000000000000000000000000000000000
  mov rax, 0

.bin_to_dec_loop:
  mov r8, 0
  test rdi, r10
  cmovne r8, rdx
  add rax, r8
  shr r10, 1
  shr rdx, 1
  loop .bin_to_dec_loop

  mov rcx, 18
  mov rdi, 10
  add r9, 17
.dec_to_str_loop:
  mov rdx, 0               ; clear most significant part of dividend
  div rdi                  ; divide by 10
  add rdx, '0'             ; add '0' character
  mov [r9+_5_buffer], dl   ; write digit to string
  dec r9
  loop .dec_to_str_loop

  lea rcx, [r9+17]
.remove_zeroes:
  cmp byte [rcx+_5_buffer], '0'
  loope .remove_zeroes
  lea r9, [rcx+2]

  lea rax, [rsi-1023]
  cmp rax, 0
  je .to_string
  mov qword [r9+_5_buffer], '*2^!'
  add r9, 3
  cmp rax, 0
  mov rcx, 5
  jge .non_negative_exp

  ; negative exp
  mov byte [r9+_5_buffer], '-'
  inc r9
  neg rax

.non_negative_exp:
  add r9, 4

.exp_loop:
  mov rdx, 0               ; clear most significant part of dividend
  div rdi                  ; divide by 10
  add rdx, '0'             ; add '0' character
  mov [r9+_5_buffer], dl   ; write digit to string
  dec r9
  loop .exp_loop

  add r9, 5
  jmp .to_string

.sub_normal:
  mov word [r9+_5_buffer], '0.'
  add r9, 2
  mov byte [r9+_5_buffer], '0' ; temp
  add r9, 1
  jmp .to_string

.nan_and_infty:
  cmp rdi, 0
  je .infty

  ; nan
  mov qword [r9+_5_buffer], 'NaN '
  add r9, 3
  jmp .to_string

.infty:
  mov qword [r9+_5_buffer], 0x009e88e2 ; utf-8 infty
  add r9, 3
  
.to_string:
  ; reserve string
  lea rsi, [r9+8]
  mov rax, 9
  xor rdi, rdi
  mov rdx, 3
  mov r10, 34
  mov r8, -1
  xor r9, r9
  syscall

  lea rcx, [rsi-8]
  lea rdi, [rax+8] 
  mov [rax], rcx
  mov [rsp+16], rdi
  lea rsi, [_5_buffer]
  cld
  rep movsb

  ret 8

global _start
_start:
global start
start:
global main
main:
_6:
  push rbp
  mov rbp, rsp
  add rsp, 80
  mov qword [-8+rbp], 3
  mov qword [-16+rbp], 4
  mov r8, qword [-8+rbp]
  mov qword [-32+rbp], r8
  mov r8, qword [-16+rbp]
  mov qword [-40+rbp], r8
  sub rsp, 8
  mov r8, qword [-32+rbp]
  mov r9, qword [-40+rbp]
  mov r10, r8
  add r10, r9
  push r10
  call _3
  pop r8
  push r8
  call _1
_8:
  mov rax, 60
  mov rdi, 0
  syscall

