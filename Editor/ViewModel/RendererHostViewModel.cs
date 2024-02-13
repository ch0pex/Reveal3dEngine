using System.Runtime.InteropServices;
using System.Windows.Interop;

namespace Reveal3d.ViewModel; 

public class RendererHostViewModel : HwndHost{
    protected override HandleRef BuildWindowCore(HandleRef hwndParent) {
        throw new NotImplementedException();
    }

    protected override void DestroyWindowCore(HandleRef hwnd) {
        throw new NotImplementedException();
    }
}