import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaSparkContext;
import scala.Tuple2;

import javax.validation.constraints.Max;

import static java.lang.StrictMath.min;

public class SingleSourceShortestPaths {
    public static int sourceVerticeID = 1;
    public static int MaxIteration = 100;

    public static void main(String[] args) throws Exception {
        SparkConf conf = new SparkConf().setAppName("ConnectedComponents");
        JavaSparkContext sc = new JavaSparkContext(conf);
        sc.setLogLevel("WARN");

        String edgesFile = "hdfs:///user/hadoop/input/SingleSourceShortestPaths/edges.txt";
        String verticesFile = "hdfs:///user/hadoop/input/SingleSourceShortestPaths/vertices.txt";
        String outFile = "hdfs:///user/hadoop/output/Graph/SparkSingleSourceShortestPaths";

        /**
         * edgesRDD: [from, to, dis ]
         * verticesRDD: [vertice, dis]
         */


        JavaPairRDD<Integer, Tuple2<Integer, Double>> edgesRDD = sc.textFile(edgesFile)
                .mapToPair(
                        line -> {
                            int from = Integer.parseInt(line.split(" ")[0]);
                            int to = Integer.parseInt(line.split(" ")[1]);
                            double dis = Double.parseDouble(line.split(" ")[2]);
                            return new Tuple2<>(from, new Tuple2<>(to, dis));
                        }
                );

        JavaPairRDD<Integer, Double> verticesRDD = sc.textFile(verticesFile)
                .mapToPair(
                        line -> {
                            int vertice = Integer.parseInt(line);
                            if (vertice == sourceVerticeID)
                                return new Tuple2<>(vertice, 0.0);
                            return new Tuple2<>(vertice, Double.POSITIVE_INFINITY);
                        }
                );

        int count = 0;
        while (count < MaxIteration) {
            // get new dis
            JavaPairRDD<Integer, Double> newVerticesRDD = verticesRDD
                    .join(edgesRDD)
                    .mapToPair(
                            line -> {
                                if (line._2._1 != Double.POSITIVE_INFINITY)
                                    return new Tuple2<>(line._2._2._1, line._2._1 + line._2._2._2);
                                return new Tuple2<>(line._2._2._1, Double.POSITIVE_INFINITY);
                            }
                    ).reduceByKey(
                            (v1, v2) -> min(v1, v2));

            JavaPairRDD<Integer, Tuple2<Double, Double>> filterRDD = newVerticesRDD.join(verticesRDD)
                    .filter(
                            each -> each._2._1 < each._2._2);

            if (filterRDD.isEmpty())
                break;

            // update to verticesRDD
            verticesRDD = verticesRDD.leftOuterJoin(newVerticesRDD).
                    mapValues(
                            v -> min(v._1, v._2.orElse(v._1)));
        }
        verticesRDD.saveAsTextFile(outFile);

    }
}