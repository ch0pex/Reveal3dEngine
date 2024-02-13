using System.Windows.Controls;
using System.Windows.Input;

namespace Reveal3d.Views.UserControls;

public partial class FloatingWindowView : UserControl {
    private bool _showConsole = false; 
    
    
    public FloatingWindowView() {
        InitializeComponent();
    }

    private void Console_click(object sender, MouseButtonEventArgs e) {
        if (!_showConsole) {
            Canvas.SetBottom(Console, -80);
            _showConsole = true;
        }
        else {
            Canvas.SetBottom(Console, -380);
            _showConsole = false;
        }
       
       
    }
}