
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.util.GenericOptionsParser;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URISyntaxException;

import static java.lang.StrictMath.abs;


public class PageRank {

    private static final String CACHED_PATH = "output/cache";
    private static final String ACTUAL_PATH = "output/Graph/HadoopPageRank";
    public static final int maxIterations = 500;
    public static final double threshold = 0.0001;
    public static final double dumping = 0.85;
    public static int pageNum = 0;

    public static void main(String[] args) throws IOException,
            InterruptedException, ClassNotFoundException, URISyntaxException {

        Configuration conf = new Configuration();
        String[] otherArgs = (new GenericOptionsParser(conf, args)).getRemainingArgs();
        if (otherArgs.length != 3) {
            System.err.println("Usage: PageRank <PagePath> <LinksPath> <PageNum>");
            System.exit(2);
        }

        int code = 0;

        Path PagePath = new Path(otherArgs[0]);
        Path LinksPath = new Path(otherArgs[1]);
        pageNum = Integer.parseInt(otherArgs[2]);

        conf.set("pageNum", pageNum + "");
        conf.set("dumping", dumping + "");


        Path cachePath = new Path(CACHED_PATH);
        Path actualPath = new Path(ACTUAL_PATH);

        // Delete output if exists
        FileSystem hdfs = FileSystem.get(conf);
        if (hdfs.exists(actualPath))
            hdfs.delete(actualPath, true); // recursive delete

        // prepare original rank
        for (int i = 1; i <= pageNum; i++)
            writeFileByline(ACTUAL_PATH + "/part-r-00000", i + " " + 1.0 / pageNum);


        int counter = 0;
        boolean changed = true;

        while (counter < maxIterations && changed) {

            // Delete output if exists
            if (hdfs.exists(cachePath))
                hdfs.delete(cachePath, true);
            //moving the previous iteration file to the cache directory
            hdfs.rename(actualPath, cachePath);

            conf.set("mapreduce.output.textoutputformat.separator", " ");
            conf.set("mapreduce.input.keyvaluelinerecordreader.key.value.separator", " ");


            Job PageRank = Job.getInstance(conf, "PageRank " + (counter + ""));

            // add cache
            PageRank.addCacheFile(PagePath.toUri());

            PageRank.setJarByClass(PageRankMapper.class);
            FileInputFormat.addInputPath(PageRank, LinksPath);
            // set out put path : output/means
            FileOutputFormat.setOutputPath(PageRank, actualPath);

            PageRank.setMapperClass(PageRankMapper.class);
            PageRank.setInputFormatClass(KeyValueTextInputFormat.class);
            PageRank.setMapOutputKeyClass(IntWritable.class);
            PageRank.setMapOutputValueClass(DoubleWritable.class);

            PageRank.setReducerClass(PageRankReducer.class);
            PageRank.setOutputKeyClass(IntWritable.class);
            PageRank.setOutputValueClass(DoubleWritable.class);

            // Execute job
            code = PageRank.waitForCompletion(true) ? 0 : 1;

            //checking if the mean is stable
            BufferedReader file1Reader = new BufferedReader(new InputStreamReader(hdfs.open(new Path(CACHED_PATH + "/part-r-00000"))));
            BufferedReader file2Reader = new BufferedReader(new InputStreamReader(hdfs.open(new Path(ACTUAL_PATH + "/part-r-00000"))));
            for (int i = 0; i < pageNum; i++) {
                double rank1 = Double.parseDouble(file1Reader.readLine().split(" ")[1]);
                double rank2 = Double.parseDouble(file2Reader.readLine().split(" ")[1]);

                if (abs(rank1 - rank2) <= threshold) {
                    changed = false;
                } else {
                    changed = true;
                    break;
                }
            }
            file1Reader.close();
            file2Reader.close();
            counter++;
            System.out.println("PageRank finished iteration:>> " + counter + " || rank change: " + changed);

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