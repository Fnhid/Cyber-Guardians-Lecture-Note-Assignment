import struct
#asdf
def goout(addr): # test_suc
    for x in XrefsTo(addr, flags=0):
      addr = x.frm


def find_key_cmp(addr):
    flag_mov = True
    flag_lea = True
    flag_sub = True
    flag_1 = False
    flag_2 = False
    flag_3 = False
    while True:
        if (print_insn_mnem(addr) == "mov" and flag_mov == True):
            check = addr
            flag_mov = False
        if( print_insn_mnem(addr) == "jz"): #test_suc
            tmp = get_operand_value(addr, 0)
            if(tmp == check):
                addr = idc.prev_head(addr)
                return get_operand_value(addr, 1)
        if(print_insn_mnem(addr) == "jnz"): #test_suc
                addr = idc.prev_head(addr)
                return get_operand_value(addr, 1)
        if(print_insn_mnem(addr) == "add"):
            while True:
                if (print_insn_mnem(addr) == "lea" and flag_lea == True): 
                     
                    unk = get_operand_value(addr, 1)
                    flag_lea = False
                    flag_1 = True
                if (print_insn_mnem(addr) == "cmp"):
                    comp = get_operand_value(addr, 1)
                    flag_2 = True
                    
                if (print_insn_mnem(addr) == "sub" and flag_sub == True):
                    sub = get_operand_value(addr, 1)
                    flag_sub = False
                    flag_3 = True
                if(flag_1 == True and flag_2 == True and flag_3 == True):
                    for i in range(0xff):
                        if (i - sub <= comp):
                            tmp = struct.unpack("<I", get_bytes(unk + (i - sub) * 4, 4))[0] + unk&0xffffffff
                            if(tmp == check):
                                return i
                addr = idc.prev_head(addr)
        addr = idc.prev_head(addr)

pay = ''
global addr
addr = goout(0x59BE5)
while True:
    pay += chr(find_key_cmp(addr))
    addr = get_func_attr(addr, 0)
    addr = goout(addr)
    
    if(get_func_name(addr) == "main"):
        break
    
pay = pay[::-1]
print(pay)
