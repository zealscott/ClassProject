
import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.util.GenericOptionsParser;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.URISyntaxException;

public class kmeansMain {

    private static final String CACHED_PATH = "output/cache";
    private static final String ACTUAL_PATH = "output/means";

    // directory store acutal result
    private static final String CACHED_MEANS = "output/cache/part-r-00000";
    private static final String ACTUAL_MEANS = "output/means/part-r-00000";


    public static void writeFileByline(String dst, String contents)  throws IOException{
        Configuration conf = new Configuration();
        Path dstPath = new Path(dst);
        FileSystem fs = dstPath.getFileSystem(conf);
        FSDataOutputStream outputStream = null;

        if (!fs.exists(dstPath)) {
            outputStream = fs.create(dstPath);
        }else{
            outputStream = fs.append(dstPath);
        }
        contents = contents + "\n";
        outputStream.write(contents.getBytes("utf-8"));
        outputStream.close();
    }

    public static int readFileByLines(String fileName,String meansPath) throws IOException {
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(fileName), conf);
        FSDataInputStream hdfsInStream = fs.open(new Path(fileName));
        InputStreamReader isr = new InputStreamReader(hdfsInStream, "utf-8");
        BufferedReader br = new BufferedReader(isr);
        // get first line k
        String line =  br.readLine();
        int k = Integer.parseInt(StringUtils.split(line, ",")[0]);
        int count = 0;

        while ((line = br.readLine()) != null && count < k) {
            writeFileByline(meansPath, line);
            count++;
        }
        return k;
    }


    public static void main(String[] args) throws IOException,
            InterruptedException, ClassNotFoundException, URISyntaxException {

        Configuration conf = new Configuration();
        String[] otherArgs = (new GenericOptionsParser(conf, args)).getRemainingArgs();
        if (otherArgs.length < 2) {
            System.err.println("Usage: kmeans <in> <out>");
            System.exit(2);
        }

        int code = 0;

        Path inputPath = new Path(otherArgs[0]);
        Path outputDir = new Path(otherArgs[1] + "");

        Path cacheMeansPath = new Path(CACHED_MEANS);
        Path actualMeansPath = new Path(ACTUAL_MEANS);

        Path cachePath = new Path(CACHED_PATH);
        Path actualPath = new Path(ACTUAL_PATH);

        int k = readFileByLines(otherArgs[0],ACTUAL_MEANS);
        int maxIterations = 500;
        double threshold = 0.000001;


        // Delete output if exists
        FileSystem hdfs = FileSystem.get(conf);
        if (hdfs.exists(outputDir))
            hdfs.delete(outputDir, true); // recursive delete

        boolean changed = false;
        int counter = 0;

        while(!changed && counter < maxIterations){

            // Delete output if exists
            if (hdfs.exists(cachePath))
                hdfs.delete(cachePath, true);
            //moving the previous iteration file to the cache directory
            hdfs.rename(actualPath, cachePath);

            conf.set("threshold", threshold+"");
            //passing K to the map reduce as a parameter
            conf.set("k", k+"");
            conf.set("mapreduce.output.textoutputformat.separator", ",");


            Job kmeans = Job.getInstance(conf, "Kmeans "+ (counter + ""));

            // add cache
            kmeans.addCacheFile(cacheMeansPath.toUri());

            kmeans.setJarByClass(kmeansMapper.class);
            FileInputFormat.addInputPath(kmeans, inputPath);
            // set out put path : output/means
            FileOutputFormat.setOutputPath(kmeans, actualPath);

            kmeans.setMapperClass(kmeansMapper.class);
            kmeans.setMapOutputKeyClass(IntWritable.class);
            kmeans.setMapOutputValueClass(Text.class);

            kmeans.setReducerClass(kmeansReducer.class);
            kmeans.setOutputKeyClass(DoubleWritable.class);
            kmeans.setOutputValueClass(DoubleWritable.class);

            // Execute job
            code = kmeans.waitForCompletion(true) ? 0 : 1;

            //checking if the mean is stable
            BufferedReader file1Reader = new BufferedReader(new InputStreamReader(hdfs.open(cacheMeansPath)));
            BufferedReader file2Reader = new BufferedReader(new InputStreamReader(hdfs.open(actualMeansPath)));
            for(int i = 0; i<k; i++){
                String[] keyValue1 = file1Reader.readLine().split(",");
                String[] keyValue2 = file2Reader.readLine().split(",");

                Point p1 = new Point(Double.parseDouble(keyValue1[0]),Double.parseDouble(keyValue1[1]));
                Point p2 = new Point(Double.parseDouble(keyValue2[0]),Double.parseDouble(keyValue2[1]));

                if(p1.EuclideanDis(p2) <= threshold){
                    changed = true;
                }else{
                    changed = false;
                    break;
                }
            }
            file1Reader.close();
            file2Reader.close();
            counter++;
            System.out.println("KMEANS finished iteration:>> "+counter + " || means stable: "+ changed);

        }


        hdfs.rename(actualPath, outputDir);

        System.exit(code);


    }

}