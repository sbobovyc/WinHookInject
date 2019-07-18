#https://reverseengineering.stackexchange.com/questions/16448/pydbg-list-imported-api
#https://www.codeproject.com/articles/16541/create-your-proxy-dlls-automatically
#https://github.com/MrColdbird/SW42-SteamGamepadFix/blob/master/dinput8/dinput8.cpp

import pefile
import string
import os
import re

dll_path = r"C:\Windows\System32\dinput8.dll"

dll_main = """#include <windows.h>
#pragma pack(1)
HINSTANCE hLThis = 0;
HINSTANCE hL = 0;
FARPROC p[${num_ordinals}] = {0};
// -----------
BOOL WINAPI DllMain(HINSTANCE hInst,DWORD reason,LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        hLThis = hInst;
        hL = LoadLibrary(LR"(${dll_path})");
        if (!hL) return false;

        // collect function addresses
        ${function_addresses}
    }
    if (reason == DLL_PROCESS_DETACH)
    {
        FreeLibrary(hL);
    }
    return 1;
}
// function hooks
${function_hooks}
"""

get_proc_address = """
        p[${ordinal}] = GetProcAddress(hL, "${function_name}");"""

function_hook = """
// ${function_name}
extern "C" __declspec(naked) void __stdcall __E__${function_id}__()
{
    __asm
    {
        jmp p[${function_id}*4];
    }
}
"""

export = """LIBRARY
EXPORTS
${export_definitions}
"""

export_definition = """${function_name}=__E__${function_id}__ @${ordinal}\n"""

dll_no_ext = os.path.splitext(os.path.basename(dll_path))[0]

pe = pefile.PE(dll_path)
export_directory = pe.OPTIONAL_HEADER.DATA_DIRECTORY[0]
exp_dir = pe.parse_export_directory(export_directory.VirtualAddress, export_directory.Size)
num_oridinals = len(exp_dir.symbols)

get_procs = []
function_hooks = []
export_definitions = []
for symbol in exp_dir.symbols:
    #print(symbol.name, symbol.ordinal)
    params = {}    
    params['ordinal'] = symbol.ordinal
    params['function_id'] = symbol.ordinal - 1
    params['function_name'] = symbol.name.decode()
    get_proc = string.Template(get_proc_address).substitute(params)
    get_procs.append(get_proc)

    func_hook = string.Template(function_hook).substitute(params)
    function_hooks.append(func_hook)

    export_definitions.append(string.Template(export_definition).substitute(params))
    
params = {}
params['dll_path'] = dll_path
params['num_ordinals'] = num_oridinals
params['function_addresses'] = ''.join(get_procs)
params['function_hooks'] = ''.join(function_hooks)
with open(dll_no_ext + ".cpp", 'w') as f:
    f.write(string.Template(dll_main).substitute(params))
params['export_definitions'] = ''.join(export_definitions)

with open(dll_no_ext + ".def", 'w') as f:
    f.write(string.Template(export).substitute(params))


