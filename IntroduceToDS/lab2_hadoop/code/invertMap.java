package hadoop_intervt;

import java.io.IOException;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;



public class invertMap extends Mapper<LongWritable, Text, Text, Text>{
	@Override
	protected void map(LongWritable key, Text value, Context context)throws IOException, InterruptedException {
		String [] lines = value.toString().split(" ");
		Text datetime;
		String temp="";
		if (lines[0].length()>4)
		{
			temp=lines[0].substring(0, lines[0].length()-4);
			datetime=new Text(temp);
			for (int i = 1; i < lines.length; i++) {
				boolean result = lines[i].matches("[\\u4e00-\\u9fa5]+");
				if (lines[i].length() > 1 && result == true) {
					context.write(new Text(lines[i]),datetime);
				}
			}
		}
	}
}
