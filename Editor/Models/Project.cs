using System.Runtime.Serialization;

namespace Reveal3d.Models;

[DataContract]
public class ProjectHeader { //Project header as model? 

    public ProjectHeader(string name, string path) {
        Name = name;
        Path = path;
    }

    [DataMember] public string Name { get; set; }
    [DataMember] public string Path { get; set; }
}

[DataContract]
public class Project {
    public Project(string name, string path) {
        Header = new ProjectHeader(name, path);
    }
    
    [DataMember] 
    public ProjectHeader Header {get; set;}

    
}