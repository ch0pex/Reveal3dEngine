using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms.VisualStyles;
using System.Windows.Input;

namespace Reveal3d.Views.UserControls;

public partial class TitleBarView : UserControl {
    public TitleBarView() {
        InitializeComponent();
    }

    private void CloseClick(object sender, RoutedEventArgs e) {
        //TODO: Check if project is saved and show save dialog
        Application.Current.Shutdown();
    }

    private void MaximizeClick(object sender, RoutedEventArgs e) {
        SwapMode();
    }

    private void MinimizeClick(object sender, RoutedEventArgs e) {
        var window_ = Window.GetWindow(this)!;
        window_.WindowState = WindowState.Minimized;
    }

    private void DragEvent(object sender, MouseButtonEventArgs e) {
        var window_ = Window.GetWindow(this)!;
        window_.DragMove();
    }

    private void DoubleClickEvent(object sender, MouseButtonEventArgs e) {
        SwapMode();
    }

    private void SwapMode() {
        var window_ = Window.GetWindow(this)!;
        window_.WindowState = (window_.WindowState == WindowState.Maximized) ? WindowState.Normal : WindowState.Maximized;
    }
}