import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

import java.io.IOException;
import java.util.Random;

public class Score {

    public Score() {

    }

    public static class StuIDMapper
            extends Mapper<Object, Text, LongWritable, IntWritable> {

        private final static IntWritable one = new IntWritable(1);
        LongWritable ID = new LongWritable();

        public void map(Object key, Text value, Context context)
                throws IOException, InterruptedException {

            ID.set(Long.valueOf(value.toString()));

            context.write(this.ID, one);

        }
    }

    public static class ScoreReducer
            extends Reducer<LongWritable, IntWritable, LongWritable, IntWritable> {
        private IntWritable score = new IntWritable();
        private Random rand = new Random();

        public void reduce(LongWritable ID, Iterable<IntWritable> values, Context context)
                throws IOException, InterruptedException {

            score.set(rand.nextInt(100) + 1);

            context.write(ID, this.score);
        }

    }


    public static void main(String[] args) throws Exception {

        org.apache.hadoop.conf.Configuration conf = new Configuration();
        String[] otherArgs = (new GenericOptionsParser(conf, args)).getRemainingArgs();
        if (otherArgs.length < 2) {
            System.err.println("Usage: wordcount <in>[<in>...] <out>");
            System.exit(2);
        }

        Job job = Job.getInstance(conf, "student score");
        job.setJarByClass(Score.class);
        job.setMapperClass(Score.StuIDMapper.class);
        job.setReducerClass(Score.ScoreReducer.class);
        job.setOutputKeyClass(LongWritable.class);
        job.setOutputValueClass(IntWritable.class);

        for (int i = 0; i < otherArgs.length - 1; i++) {
            FileInputFormat.addInputPath(job, new Path(otherArgs[i]));
        }

        FileOutputFormat.setOutputPath(job, new Path(otherArgs[otherArgs.length - 1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}
