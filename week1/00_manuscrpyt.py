def decrypt_str(src):
    des =""
    for i in range(len(src)):
        tmp=src[i]
        if ( ord(tmp) < 105 or ord(tmp) > 112 ):
            if ( ord(tmp) < 114 or ord(tmp) > 121 ):
                if ( ord(tmp) < 73 or ord(tmp) > 80 ):
                    if ( ord(tmp) >= 82 and ord(tmp) <= 89 ):
                        des += chr(ord(tmp)-9)
                    else:
                        des += chr(ord(tmp))
                else:
                    des += chr(ord(tmp)+9)
            else:
                des += chr(ord(tmp)-9)
        else:    
            des += chr(ord(tmp)+9)
    return des
 
def find_function_arg(addr):
    while True:
        addr = idc.prev_head(addr)
        if (print_insn_mnem(addr) == "push"):
            return get_operand_value(addr, 0)
    return ""
 
def get_string(addr):
    out = ""
    while True:
        if get_wide_byte(addr) != 0:
            out += chr(get_wide_byte(addr))
        else:
            break
        addr += 1
    return out
 
print("[*] Attempting to decrypt strings in malware")
for x in XrefsTo(0x10003B00, flags=0):
    ref = find_function_arg(x.frm)
    string = get_string(ref)
    deobf_string = decrypt_str(string)
    print ('[STRING]:%s\n[Deobfuscated]:%s' % (string,deobf_string))
    set_cmt(x.frm, deobf_string,0)
    set_cmt(ref, deobf_string,0) 
    
    print("[ADDRESS] :"+hex(x.frm))
    ct = idaapi.decompile(x.frm)
    tl = idaapi.treeloc_t()
    tl.ea = x.frm
    tl.itp = idaapi.ITP_SEMI
    ct.set_user_cmt(tl,deobf_string)
    ct.save_user_cmts()