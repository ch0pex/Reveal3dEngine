using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using Reveal3d.Util;
using Reveal3d.Models;

namespace Reveal3d.ViewModel;


[DataContract]
public class ProjectsDialogCache {
    public ProjectsDialogCache() {
        Projects = new List<ProjectHeader>();
        Templates = new List<ProjectHeader>();
    }
    [DataMember] public List<ProjectHeader> Projects { get; set; } 
    [DataMember] public List<ProjectHeader> Templates { get; set; }
}

public class ProjectsDialogViewModel {

    private static readonly string RecentProjectsDataPath = $@"{Constants.Paths.AppData}\ProjectsCache.xml";
    private readonly ObservableCollection<ProjectHeader> _recentProjects = new();
    public ReadOnlyObservableCollection<ProjectHeader> RecentProjects { get; }
    private readonly ObservableCollection<ProjectHeader> _templates = new();
    public ReadOnlyObservableCollection<ProjectHeader> Templates { get; }

    public ProjectsDialogViewModel() {
        RecentProjects = new ReadOnlyObservableCollection<ProjectHeader>(_recentProjects);
        Templates = new ReadOnlyObservableCollection<ProjectHeader>(_templates);
        try {
            if (!Directory.Exists(Constants.Paths.AppData)) { 
                Directory.CreateDirectory(Constants.Paths.AppData);
                EmptyRecentProjects();
                return;
            }

            var cache = new ProjectsDialogCache();
            cache.Projects = new();
            cache.Projects.Add(new ProjectHeader("Project1", "Path1"));
            cache.Projects.Add(new ProjectHeader("Project2", "Path2"));
            cache.Projects.Add(new ProjectHeader("Project3", "Path3"));
            cache.Projects.Add(new ProjectHeader("Project4", "Path4"));
            
            cache.Templates = new();
            cache.Templates.Add(new ProjectHeader("Template1", "Path1"));
            cache.Templates.Add(new ProjectHeader("Template2", "Path2"));
            cache.Templates.Add(new ProjectHeader("Template3", "Path3"));
            cache.Templates.Add(new ProjectHeader("Template4", "Path4"));
            Serializer.WriteFile(cache, RecentProjectsDataPath);
            LoadCacheProjects();
        } 
        catch (Exception e) {
            Debug.WriteLine(e.Message);       
        }
        
    }

    private static void EmptyRecentProjects() { 
        //TODO: Put no recent projects msg
    }

    private void LoadCacheProjects() {
        try {
            var cache = Serializer.ReadFile<ProjectsDialogCache>(RecentProjectsDataPath);
            if (!cache.Projects.Any()) {
                EmptyRecentProjects();
                return;
            }

            foreach (var projectInfo in cache.Projects) {
                _recentProjects.Add(projectInfo);
            }
            
            foreach (var projectInfo in cache.Templates) {
                _templates.Add(projectInfo);
            }
        }
        catch (Exception e) {
           Debug.WriteLine(e.Message); 
        }
    }
}