using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;

namespace Reveal3d.DllWrapper;

public class EngineAPI {
    private const string _engineDll = "Wrapper.dll";

    [DllImport(_engineDll)]
    public static extern int CreateViewport(IntPtr host, int width, int height);
    
    [DllImport(_engineDll)]
    public static extern IntPtr GetWindowHandle();
    
    [DllImport(_engineDll)]
    public static extern int RemoveViewport();
}