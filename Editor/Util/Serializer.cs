using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;

namespace Reveal3d.Util;

public static class Serializer {
   public static void WriteFile<T>(T instance, string path) {
      try {
         using var fs = new FileStream(path, FileMode.Create);
         var serializer = new DataContractSerializer(typeof(T));
         serializer.WriteObject(fs, instance);
      }
      catch (Exception e) {
         Debug.WriteLine(e.Message); 
      }
   }

   internal static T ReadFile<T>(string path) {
      try {
         using var fs = new FileStream(path, FileMode.Open);
         var serializer = new DataContractSerializer(typeof(T));
         T isntance = (T)serializer.ReadObject(fs);
         return isntance;
      }
      catch (Exception e) {
         Debug.WriteLine(e.Message);
         return default(T);
      }
   }
}