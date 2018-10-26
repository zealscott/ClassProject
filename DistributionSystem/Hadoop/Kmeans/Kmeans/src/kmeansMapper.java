import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;

public class kmeansMapper extends Mapper<LongWritable, Text, IntWritable, Text> {

    private List<Point> means;

    /**
     * reading the data from the distributed cache
     */
    public void setup(Context context) throws IOException, InterruptedException {
        means = new ArrayList<Point>();

//
//        URI[] cacheFiles  = context.getCacheFiles();
//        BufferedReader br = new BufferedReader(new FileReader(cacheFiles[0].toString()));

        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(conf);
        FSDataInputStream hdfsInStream = fs.open(new Path("output/cache/part-r-00000"));
        InputStreamReader isr = new InputStreamReader(hdfsInStream, "utf-8");
        BufferedReader br = new BufferedReader(isr);


        String lineString = null;
        while((lineString = br.readLine()) != null){
            String[] keyValue = StringUtils.split(lineString,",");

            Point tmpCluster = new Point(Double.parseDouble(keyValue[0]),Double.parseDouble(keyValue[1]));
            means.add(tmpCluster);

        }
        br.close();
    }

    public void map(LongWritable key, Text keyvalue, Context context) throws IOException, InterruptedException{
        // ignore first line
        if (key.get() == 0)
            return;

        String[] keyValue = StringUtils.split(keyvalue.toString(),",");

        String X = keyValue[0];
        String Y = keyValue[1];

        Point tmpPoint = new Point(Double.parseDouble(X),Double.parseDouble(Y));

        context.write(new IntWritable(findClosest(tmpPoint)), new Text(X + "," + Y));
    }

    /**
     * method that returns the closest mean from the point
     * @param value
     * @return
     */
    private int findClosest(Point value){
        int argmin = 0;
        double minimalDistance = Double.MAX_VALUE ;
        for(int i = 0; i<means.size(); i++){
            Point tmpCluster = means.get(i);
            double distance = value.EuclideanDis(tmpCluster);
            if(distance < minimalDistance){
                minimalDistance = distance;
                argmin = i;
            }
        }
        return argmin;
    }
}
