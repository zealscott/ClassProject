import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaSparkContext;
import scala.Tuple2;

import static java.lang.StrictMath.min;

public class ConnectedComponents {

    public static int MaxIteration = 100;

    public static void main(String[] args) {
        SparkConf conf = new SparkConf().setAppName("ConnectedComponents");
        JavaSparkContext sc = new JavaSparkContext(conf);
        sc.setLogLevel("WARN");

        String edgesFile = "hdfs:///user/hadoop/input/ConnectedComponents/edges.txt";
        String verticesFile = "hdfs:///user/hadoop/input/ConnectedComponents/vertices.txt";
        String outFile = "hdfs:///user/hadoop/output/Graph/SparkConnectedComponents";


        /**
         * edgesRDD: [x,y]
         * componentsRDD: [x,x] init
         */

        JavaPairRDD<Integer, Integer> edgesRDD = sc.textFile(edgesFile)
                .mapToPair(
                        line -> new Tuple2<>(
                                Integer.parseInt(line.split(" ")[0]),
                                Integer.parseInt(line.split(" ")[1])
                        )
                );

        JavaPairRDD<Integer, Integer> componentsRDD = sc.textFile(verticesFile)
                .mapToPair(
                        line -> new Tuple2<>(Integer.parseInt(line), Integer.parseInt(line))
                );

        int count = 0;

        while (count < MaxIteration) {
            JavaPairRDD<Integer, Integer> newcomponentsRDD = componentsRDD.join(edgesRDD)
                    .mapToPair(
                            x -> new Tuple2<>(x._2._2, x._2._1)
                    )
                    .reduceByKey(
                            (v1, v2) -> min(v1, v2)
                    );

            JavaPairRDD<Integer, Tuple2<Integer, Integer>> filterRDD = newcomponentsRDD.join(componentsRDD)
                    .filter(
                            each -> each._2._1 < each._2._2
                    );

            if (filterRDD.isEmpty())
                break;

            // update to componentsRDD
            componentsRDD = componentsRDD.leftOuterJoin(newcomponentsRDD).
                    mapValues(
                            v -> min(v._1, v._2.orElse(v._1))
                    );

            count++;
        }

        componentsRDD.saveAsTextFile(outFile);


    }
}
