import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

import java.io.IOException;

public class kmeansReducer extends Reducer<IntWritable, Text, DoubleWritable, DoubleWritable> {

    public void reduce(IntWritable key, Iterable<Text> values, Context context) throws IOException,
            InterruptedException{

        double sumX = 0.0;
        double sumY = 0.0;
        int count = 0;
        for(Text value : values){
            String[] keyValue = StringUtils.split(value.toString(),",");
            sumX +=  Double.parseDouble(keyValue[0]);
            sumY +=  Double.parseDouble(keyValue[1]);
            count ++;
        }


        context.write(new DoubleWritable(sumX/count), new DoubleWritable(sumY/count));

    }
}