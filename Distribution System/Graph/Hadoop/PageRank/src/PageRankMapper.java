import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.*;

public class PageRankMapper extends Mapper<Text, Text, IntWritable, DoubleWritable> {

    Map<Integer, Double> rank = new HashMap<>();
    Map<Integer, Integer> pages = new HashMap<>();

    /**
     * reading the rank from the distributed cache
     */
    public void setup(Context context) throws IOException, InterruptedException {
        String lineString = null;
        // read rank file
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(conf);
        FSDataInputStream hdfsInStream = fs.open(new Path("output/cache/part-r-00000"));
        InputStreamReader isr = new InputStreamReader(hdfsInStream, "utf-8");
        BufferedReader br = new BufferedReader(isr);

        while ((lineString = br.readLine()) != null) {
            String[] keyValue = StringUtils.split(lineString, " ");
            rank.put(Integer.parseInt(keyValue[0]), Double.parseDouble(keyValue[1]));

        }
        br.close();

        // read pages file
        String PagesFiles = context.getLocalCacheFiles()[0].getName();
        br = new BufferedReader(new FileReader(PagesFiles));
        while ((lineString = br.readLine()) != null) {
            String[] keyValue = StringUtils.split(lineString, " ");
            pages.put(Integer.parseInt(keyValue[0]), Integer.parseInt(keyValue[1]));
        }
        br.close();

    }

    public void map(Text from, Text to, Context context) throws IOException, InterruptedException {
        int fromPoint = Integer.parseInt(from.toString());
        int toPoint = Integer.parseInt(to.toString());
        double newRank = rank.get(fromPoint) * (1.0 / pages.get(fromPoint));

        context.write(new IntWritable(toPoint), new DoubleWritable(newRank));
    }

}
