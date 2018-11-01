import org.apache.spark.SparkConf;
import org.apache.spark.api.java.*;
import org.apache.spark.api.java.function.Function;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;


public class StuScore {

    private static Random rand = new Random();

    public static void main(String[] args) {
        SparkConf conf = new SparkConf().setAppName("StuScore");
        JavaSparkContext sc = new JavaSparkContext(conf);
        String logFile = "hdfs://10.11.6.91:9000/user/hadoop7/input/stuID.csv";
        JavaRDD<String> stuID = sc.textFile(logFile);

        JavaRDD<String> stuScore = stuID.map(
                new Function<String, String>() {
                    @Override
                    public String call(String s) throws Exception {
                        String tmp  =  s + " " +  String.valueOf( rand.nextInt(100) +1);
                        return tmp;
                    }
                }
        );

        stuScore.saveAsTextFile("hdfs://10.11.6.91:9000/user/hadoop7/output/spark/StuRandomScore");
    }
}