using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DllInjection;

namespace InjectorApp
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                System.Console.WriteLine("Usage: {0} program.exe library.dll", System.AppDomain.CurrentDomain.FriendlyName);
                return;
            }
            
            string sProcName = args[0];
            string sDllPath = args[1];
            System.Console.WriteLine("{0} {1}", sProcName, sDllPath);
            DllInjector inj = DllInjector.GetInstance;
            DllInjectionResult result = inj.Inject(sProcName, sDllPath);
            System.Console.WriteLine(result);
        }
    }
}
