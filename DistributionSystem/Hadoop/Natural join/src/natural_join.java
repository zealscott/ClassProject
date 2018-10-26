import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;


import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class natural_join {

    public natural_join() {

    }

    public static class joinMapper
            extends Mapper<Object, Text, Text, Text> {

        private static final String PERSON_FLAG = "person";
        private static final String ADDRESS_FLAG = "address";

        private FileSplit fileSplit;
        private Text outKey = new Text();
        private Text outValue = new Text();


        public void map(Object key, Text value, Context context)
                throws IOException, InterruptedException {

            fileSplit = (FileSplit) context.getInputSplit();
            String filePath = fileSplit.getPath().toString();

            String line = value.toString();
            String[] fields = StringUtils.split(line, " ");

            // 判断记录来自哪个文件
            if (filePath.contains(PERSON_FLAG)) {
                if (fields.length < 3)
                    return;
                //   fields[2] is code
                outKey.set(fields[2]);
                outValue.set(PERSON_FLAG + "," + line);
            } else if (filePath.contains(ADDRESS_FLAG)) {
                //   fields[0] is city code
                outKey.set(fields[0]);
                outValue.set(ADDRESS_FLAG + "," + fields[1]);
            }

            context.write(outKey, outValue);


        }
    }

    public static class joinReducer
            extends Reducer<Text, Text, Text, Text> {
        private static final String PERSON_FLAG = "person";
        private static final String ADDRESS_FLAG = "address";

        private String fileFlag = null;
        private String cityName = null;

        private Text outCity = new Text();
        private Text outPerson = new Text();

        public void reduce(Text key, Iterable<Text> values, Context context)
                throws IOException, InterruptedException {

            List<String> perosonInfo = new ArrayList<>();
            for (Text val : values) {
                String[] fields = StringUtils.split(val.toString(), ",");
                fileFlag = fields[0];
                // choose what file it is
                if (fileFlag.equals(ADDRESS_FLAG)) {
                    cityName = fields[1];
                    outCity.set(cityName);
                } else if (fileFlag.equals(PERSON_FLAG)) {
                    perosonInfo.add(fields[1]);
                }
            }

            // Cartesian product
            for (String person : perosonInfo) {
                outPerson.set(person);
                context.write(outPerson, outCity);
            }
        }

    }


    public static void main(String[] args) throws Exception {

        org.apache.hadoop.conf.Configuration conf = new Configuration();
        conf.set("mapreduce.output.textoutputformat.separator", " ");
        String[] otherArgs = (new GenericOptionsParser(conf, args)).getRemainingArgs();
        if (otherArgs.length <= 2) {
            System.err.println("Usage: natural <in> <in> <out>");
            System.exit(2);
        }

        Job job = Job.getInstance(conf, "natural join");
        job.setJarByClass(natural_join.class);

        job.setMapperClass(natural_join.joinMapper.class);
        job.setReducerClass(natural_join.joinReducer.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        for (int i = 0; i < otherArgs.length - 1; i++) {
            FileInputFormat.addInputPath(job, new Path(otherArgs[i]));
        }

        FileOutputFormat.setOutputPath(job, new Path(otherArgs[otherArgs.length - 1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}
