import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Reducer;

import java.io.IOException;

public class PageRankReducer extends Reducer<IntWritable, DoubleWritable, IntWritable, DoubleWritable> {



    public void reduce(IntWritable key, Iterable<DoubleWritable> values, Context context) throws IOException,
            InterruptedException {

        Configuration conf = context.getConfiguration();
        int pageNum = Integer.parseInt(conf.get("pageNum"));
        double dumping = Double.parseDouble(conf.get("dumping"));

        double rank = 0.0;
        for (DoubleWritable value : values)
            rank += value.get();

        rank = (1 - dumping) * (1.0/pageNum) + dumping * rank;

        context.write(key, new DoubleWritable(rank));

    }
}