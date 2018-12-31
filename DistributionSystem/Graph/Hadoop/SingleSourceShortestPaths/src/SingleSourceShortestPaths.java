import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.util.GenericOptionsParser;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URISyntaxException;

import static java.lang.StrictMath.abs;


public class SingleSourceShortestPaths {

    private static final String CACHED_PATH = "output/cache";
    private static final String ACTUAL_PATH = "output/Graph/HadoopSingleSourceShortestPaths";
    public static final int maxIterations = 100;
    private static final double EPSILON = 0.0001;
    public static int sourcePoint = 1;

    public static void main(String[] args) throws IOException,
            InterruptedException, ClassNotFoundException, URISyntaxException {

        Configuration conf = new Configuration();
        String[] otherArgs = (new GenericOptionsParser(conf, args)).getRemainingArgs();
        if (otherArgs.length != 2) {
            System.err.println("Usage: PageRank <EdgesPath> <verticesNum>");
            System.exit(2);
        }

        int code = 0;

        Path EdgesPath = new Path(otherArgs[0]);
        int verticesNum = Integer.parseInt(otherArgs[1]);

        conf.set("verticesNum", verticesNum + "");

        Path cachePath = new Path(CACHED_PATH);
        Path actualPath = new Path(ACTUAL_PATH);

        // Delete output if exists
        FileSystem hdfs = FileSystem.get(conf);
        if (hdfs.exists(actualPath))
            hdfs.delete(actualPath, true); // recursive delete

        // prepare original distance
        for (int i = 1; i <= verticesNum; i++) {
            if (i == sourcePoint)
                writeFileByline(ACTUAL_PATH + "/part-r-00000", i + " " + 0.0);
            else
                writeFileByline(ACTUAL_PATH + "/part-r-00000", i + " " + Double.POSITIVE_INFINITY);
        }


        int counter = 0;
        boolean changed = true;

        while (counter < maxIterations && changed) {

            // Delete output if exists
            if (hdfs.exists(cachePath))
                hdfs.delete(cachePath, true);
            //moving the previous iteration file to the cache directory
            hdfs.rename(actualPath, cachePath);

            conf.set("mapreduce.output.textoutputformat.separator", " ");

            Job PageRank = Job.getInstance(conf, "SingleSourceShortestPaths " + (counter + ""));


            PageRank.setJarByClass(SingleSourceShortestPaths.class);
            FileInputFormat.addInputPath(PageRank, EdgesPath);
            FileOutputFormat.setOutputPath(PageRank, actualPath);

            PageRank.setMapperClass(SingleSourceShortestPathsMapper.class);
            PageRank.setMapOutputKeyClass(IntWritable.class);
            PageRank.setMapOutputValueClass(DoubleWritable.class);

            PageRank.setReducerClass(SingleSourceShortestPathsReducer.class);
            PageRank.setOutputKeyClass(IntWritable.class);
            PageRank.setOutputValueClass(DoubleWritable.class);

            // Execute job
            code = PageRank.waitForCompletion(true) ? 0 : 1;

            //checking if the mean is stable
            BufferedReader file1Reader = new BufferedReader(new InputStreamReader(hdfs.open(new Path(CACHED_PATH + "/part-r-00000"))));
            BufferedReader file2Reader = new BufferedReader(new InputStreamReader(hdfs.open(new Path(ACTUAL_PATH + "/part-r-00000"))));
            for (int i = 0; i < verticesNum; i++) {
                double distance1 = Double.parseDouble(file1Reader.readLine().split(" ")[1]);
                double distance2 = Double.parseDouble(file2Reader.readLine().split(" ")[1]);

                if (abs(distance1 - distance2) < EPSILON) {
                    changed = false;
                } else {
                    changed = true;
                    break;
                }
            }
            file1Reader.close();
            file2Reader.close();
            counter++;
            System.out.println("SingleSourceShortestPaths finished iteration:>> " + counter + " || distance change: " + changed);

        }

        System.exit(code);

    }


    public static void writeFileByline(String dst, String contents) throws IOException {
        Configuration conf = new Configuration();
        Path dstPath = new Path(dst);
        FileSystem fs = dstPath.getFileSystem(conf);
        FSDataOutputStream outputStream = null;

        if (!fs.exists(dstPath)) {
            outputStream = fs.create(dstPath);
        } else {
            outputStream = fs.append(dstPath);
        }
        contents = contents + "\n";
        outputStream.write(contents.getBytes("utf-8"));
        outputStream.close();
    }

}