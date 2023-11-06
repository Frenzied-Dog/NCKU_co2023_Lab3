.section .text.call_to_emu
.global call_to_emu

call_to_emu:
    ecall
    ret
