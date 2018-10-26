import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;


public class HDFSFileExist {
    public static void main(String[] args){
        try {
            String fileName = "test";
            Configuration conf = new Configuration();
            conf.set("fs.defaultFS","hdfs://hadoop1:9000");
            conf.set("fs.hdfs.impl","org.apache.hadoop.hdfs.DistributedFileSystem");
            FileSystem fs = FileSystem.get(conf);
            if(fs.exists(new Path(fileName))){
                System.out.println("file exist!");
            }else{
                System.out.println("file not exist");
            }


        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
