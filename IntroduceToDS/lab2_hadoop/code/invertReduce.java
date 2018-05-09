package hadoop_intervt;

import java.io.IOException;


import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class invertReduce extends Reducer<Text, Text, Text, Text>{
	@Override
	protected void reduce(Text key, Iterable<Text> values,Context context) throws IOException, InterruptedException {
		String times="";
		for (Text item : values) {
			String i;
			i=item.toString();
			times=times+i+"|";
		}
		times=times.substring(0, times.length()-1);
		context.write(key, new Text(times));
	}
}
