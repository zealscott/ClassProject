import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.hdfs.protocol.DirectoryListing;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.api.java.function.PairFunction;
import scala.Tuple2;

public class NaturalJoin {


    public static void main(String args[]) {
        SparkConf conf = new SparkConf().setAppName("NaturalJoin");
        JavaSparkContext sc = new JavaSparkContext(conf);
        sc.setLogLevel("WARN"); //http://stackoverflow.com/questions/27781187/how-to-stop-messages-displaying-on-spark-console


        String addFile = "hdfs:///user/hadoop/input/address.txt";
        String personFile = "hdfs:///user/hadoop/input/person.txt";


        /***
         * return code,city
         */
        JavaPairRDD<Integer, String> addRDD = sc.textFile(addFile).mapToPair(
                line -> new Tuple2<>(
                        Integer.parseInt(line.split(" ")[0]), line.split(" ")[1]));

        /**
         * return return code,{ id + name }
         */
        JavaPairRDD<Integer, String> personRDD = sc.textFile(personFile).mapToPair(
                new PairFunction<String, Integer, String>() {
                    @Override
                    public Tuple2<Integer, String> call(String s) throws Exception {
                        String[] splitLines = StringUtils.split(s, " ");
                        if (splitLines.length < 3)
                            return null;
                        return new Tuple2<>(Integer.parseInt(splitLines[2]), splitLines[0] + splitLines[1]);
                    }
                }
        );

        /**
         * return code, [{id + name}, city]
         */
        JavaPairRDD<Integer, Tuple2<String,String>> resultRDD = personRDD.join(addRDD);


        resultRDD.saveAsTextFile("hdfs:///user/hadoop/output/spark/NaturalJoin");
    }
}
