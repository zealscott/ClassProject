import org.apache.spark.SparkConf;
import org.apache.spark.api.java.*;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.api.java.function.Function2;
import org.apache.spark.api.java.function.PairFunction;
import scala.Tuple2;


import java.util.Iterator;
import java.util.Random;

public class AVGScore {
    private static Integer groupSize = 5;

    public static void main(String[] args) {
        SparkConf conf = new SparkConf().setAppName("AVGScore");
        JavaSparkContext sc = new JavaSparkContext(conf);
        sc.setLogLevel("WARN"); //http://stackoverflow.com/questions/27781187/how-to-stop-messages-displaying-on-spark-console

        String logFile = "hdfs:///user/hadoop/input/score.csv";

        JavaRDD<String> fileRDD = sc.textFile(logFile);


        /**
         * map string to (id, score) and convert to (group_id, (score,1))
         * reduceByKey => (group_id,(sumScore, count))
         * and then mapValues to avg score
         */

        JavaPairRDD<Integer, Double> stuScore = fileRDD.mapToPair(
                line -> new Tuple2<>(
                        (Integer.parseInt(line.split(" ")[0]) + 1 )/5, new Tuple2<>(Double.parseDouble(line.split(" ")[1]),1)))
                .reduceByKey(
                        (x,y) -> new Tuple2<>(x._1 + y._1,x._2+y._2))
                .mapValues(x -> x._1/x._2);

        stuScore.saveAsTextFile("hdfs:///user/hadoop/output/spark/AVGscore");
    }
}

//参考 : https://blog.csdn.net/gx304419380/article/details/79455833