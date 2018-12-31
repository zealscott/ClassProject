import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Reducer;

import java.io.IOException;

public class SingleSourceShortestPathsReducer extends Reducer<IntWritable, DoubleWritable, IntWritable, DoubleWritable> {


    public void reduce(IntWritable key, Iterable<DoubleWritable> values, Context context) throws IOException,
            InterruptedException {

        double dis = Double.POSITIVE_INFINITY;

        for (DoubleWritable value : values) {
            if (value.get() < dis)
                dis = value.get();
        }

        context.write(key, new DoubleWritable(dis));
    }
}