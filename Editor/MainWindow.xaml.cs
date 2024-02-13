using System.Windows;

using Reveal3d.Views.Windows;

namespace Reveal3d;


public partial class MainWindow : Window {
    public MainWindow() {
        InitializeComponent();
        Loaded += OnMainWindowLoaded;
    }

    private void OnMainWindowLoaded(object sender, RoutedEventArgs e) {
        Loaded -= OnMainWindowLoaded;
        MainGrid.Margin = WindowState == WindowState.Maximized ? new Thickness(8) : new Thickness(0);
        Show();
        OpenProjectBrowserDialog();
    }
    
    private void OpenProjectBrowserDialog() {
        var projectBrowser = new ProjectsDialogView();
        projectBrowser.Show();
    }

    private void WindowStateChangedEvent(object? sender, EventArgs e) {
        MainGrid.Margin = WindowState == WindowState.Maximized ? new Thickness(8) : new Thickness(0);
    }
}
