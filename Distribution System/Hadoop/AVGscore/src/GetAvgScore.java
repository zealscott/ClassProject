import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueLineRecordReader;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

import java.io.IOException;
import java.util.Random;

public class GetAvgScore {

    public GetAvgScore() {

    }

    public static class StuScoreMapper
            extends Mapper<Text, Text, IntWritable, IntWritable> {

        IntWritable Group = new IntWritable();
        IntWritable Score = new IntWritable();

        public void map(Text key, Text value, Context context)
                throws IOException, InterruptedException {

            // every 5 students as a group,[1,2,3,4,5] => group1
            Group.set((Integer.parseInt(key.toString()) - 1) / 5 + 1);
            Score.set(Integer.parseInt(value.toString()));

            context.write(this.Group, this.Score);

        }
    }

    public static class AvgScoreReducer
            extends Reducer<IntWritable, IntWritable, IntWritable, FloatWritable> {
        private FloatWritable avgscore = new FloatWritable();

        public void reduce(IntWritable Group, Iterable<IntWritable> Score, Context context)
                throws IOException, InterruptedException {

            int sum = 0;
            int count = 0;
            for (IntWritable val : Score) {
                sum += val.get();
                count++;
            }
            avgscore.set((float) sum / count);
            context.write(Group, avgscore);
        }

    }


    public static void main(String[] args) throws Exception {

        org.apache.hadoop.conf.Configuration conf = new Configuration();
//        set seperator
        conf.set(KeyValueLineRecordReader.KEY_VALUE_SEPERATOR, ",");
        String[] otherArgs = (new GenericOptionsParser(conf, args)).getRemainingArgs();
        if (otherArgs.length < 2) {
            System.err.println("Usage: wordcount <in>[<in>...] <out>");
            System.exit(2);
        }

        Job job = Job.getInstance(conf, "student avg score");
        job.setInputFormatClass(KeyValueTextInputFormat.class);
        job.setJarByClass(GetAvgScore.class);

        job.setMapperClass(GetAvgScore.StuScoreMapper.class);
        job.setReducerClass(GetAvgScore.AvgScoreReducer.class);

        job.setMapOutputKeyClass(IntWritable.class);
        job.setMapOutputValueClass(IntWritable.class);
        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(FloatWritable.class);

        for (int i = 0; i < otherArgs.length - 1; i++) {
            FileInputFormat.addInputPath(job, new Path(otherArgs[i]));
        }

        FileOutputFormat.setOutputPath(job, new Path(otherArgs[otherArgs.length - 1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}
