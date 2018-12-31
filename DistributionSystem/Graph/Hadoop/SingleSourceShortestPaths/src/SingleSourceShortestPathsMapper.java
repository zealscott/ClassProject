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
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.*;

public class SingleSourceShortestPathsMapper extends Mapper<Object, Text, IntWritable, DoubleWritable> {

    Map<Integer, Double> PointDistance = new HashMap<>();

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
            PointDistance.put(Integer.parseInt(keyValue[0]), Double.parseDouble(keyValue[1]));

        }
        br.close();
    }

    public void map(Object object, Text line, Context context) throws IOException, InterruptedException {

        String[] lineData = line.toString().split(" ");

        int fromPoint = Integer.parseInt(lineData[0]);
        int toPoint = Integer.parseInt(lineData[1]);
        double distance = Double.parseDouble(lineData[2]);

        if (distance < Double.POSITIVE_INFINITY) {
            context.write(new IntWritable(toPoint), new DoubleWritable(PointDistance.get(fromPoint) + distance));
            context.write(new IntWritable(fromPoint), new DoubleWritable(PointDistance.get(fromPoint)));
        } else
            context.write(new IntWritable(toPoint), new DoubleWritable(Double.POSITIVE_INFINITY));
    }

}
