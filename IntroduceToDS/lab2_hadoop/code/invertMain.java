package hadoop_intervt;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class invertMain {
	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException, URISyntaxException {
		String inputPath = "hdfs://192.168.163.128:9000/input";
		String outputPath = "hdfs://192.168.163.128:9000/output"; //auto create, but not override
		
		Configuration conf = new Configuration();
		
		FileSystem fileSystem = FileSystem.get(new URI(inputPath),conf);
		
		if (fileSystem.exists(new Path(outputPath))) {
			fileSystem.delete(new Path(outputPath),true);
		}
		Job job = new Job(conf,"invert");
		
		
		FileInputFormat.setInputPaths(job, inputPath);
		FileOutputFormat.setOutputPath(job, new Path(outputPath));
		
		job.setJarByClass(invertMain.class);
		job.setMapperClass(invertMap.class);
		job.setReducerClass(invertReduce.class);
		
		
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		job.waitForCompletion(true);
		
		
	}
}
