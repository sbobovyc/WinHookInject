import sys
import time
import binascii
import struct
from winappdbg import System, Process, HexDump

def memory_search( process, bytes, ):
    # Search for the string in the process memory.
    for address in process.search_bytes( bytes ):
        # Print the memory address where it was found.
        print HexDump.address( address )    

def print_threads_and_modules( pid ):

    # Instance a Process object.
    process = Process( pid )
    print "Process %d" % process.get_pid()

    # Now we can enumerate the threads in the process...
    print "Threads:"
    for thread in process.iter_threads():
        print "\t%d" % thread.get_tid()

    # ...and the modules in the process.
    print "Modules:"
    bits = process.get_bits()
    for module in process.iter_modules():
        print "\t%s\t%s" % (
            HexDump.address( module.get_base(), bits ),
            module.get_filename()
        )

system = System()
procs = system.find_processes_by_filename("printloop.exe")

p = None
if len(procs) != 0:
    p = procs[0][0]    
else:
    sys.exit(0)

p.suspend()
#print_threads_and_modules(p.get_pid())
module_address = p.get_module_by_name("printloop.exe").get_base()
addrs = p.search_bytes("\x90"*10, minAddr=module_address, maxAddr=module_address+0x100000)
cave_addr = 0
for addr in addrs:
    cave_addr = addr
    print "Cave address:",hex(cave_addr)
    break

hook_addr = module_address + 0x16B9
print "Hook address:",hex(hook_addr)
cave_code = "\x55\x89\xe5\x83\xe4\xf0\x83\xec\x10" + "\xe9" + struct.pack("<I", hook_addr - cave_addr - 9 - 5 ) # 9 is length of code cave except the jump
print binascii.hexlify(cave_code)
p.write(cave_addr, cave_code)
hook_code = "\xe9" + struct.pack("<i", cave_addr - hook_addr)
print "offset", hex(cave_addr - 9 - hook_addr)
p.write((module_address + 0x16B9), hook_code)
p.resume()
