# void read_32bit_value(unsigned short address)
.global read_32bit_value
read_32bit_value:
    mov %di, %dx
    in %dx, %eax
    ret

# void read_24bit_value(unsigned short address)
.global read_24bit_value
read_24bit_value:
    mov %di, %dx
    in %dx, %eax
    and $0x00FFFFFF, %eax
    ret
