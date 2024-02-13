using System.Diagnostics;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Input;

namespace Reveal3d.Views.Windows;

public partial class ProjectsDialogView : Window {
    private OpenFileDialog? _fileDialog;
    public ProjectsDialogView() {
        _fileDialog = null;
        InitializeComponent();
    }

    private void OnDeactivated(object? sender, EventArgs e) {
        if(_fileDialog == null) 
            Close();
    }


    private void OnProjectClicked(object sender, MouseButtonEventArgs e) {
        //var dataContext = (ProjectHeader)(sender as ListBoxItem)!.DataContext;
        //OpenProject(ProjectPath)
        Close();
    }

    private void OpenProjectFile(object sender, MouseButtonEventArgs e) {
        _fileDialog = new OpenFileDialog();
        _fileDialog.Title = "Browse project file";
        _fileDialog.Filter = "Hype file (*.hype)|(*.hype)";
        _fileDialog.FilterIndex = 1;
        _fileDialog.ShowDialog();
        if (_fileDialog.FileName != "") {
            Debug.WriteLine(_fileDialog.FileName);
            //OpenProject(fileDialog.Filename);
            Close();
        } 
        _fileDialog = null;
    }
}