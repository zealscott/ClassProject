import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Reducer;

import java.io.IOException;

public class ConnectedComponentsReduer extends Reducer<IntWritable, IntWritable, IntWritable, IntWritable> {


    public void reduce(IntWritable key, Iterable<IntWritable> values, Context context) throws IOException,
            InterruptedException {

        Configuration conf = context.getConfiguration();
        int component = Integer.parseInt(conf.get("verticesNum"));

        for (IntWritable value : values) {
            if (value.get() < component)
                component = value.get();
        }

        context.write(key, new IntWritable(component));
    }
}