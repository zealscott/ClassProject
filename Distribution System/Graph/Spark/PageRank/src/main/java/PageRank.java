import org.apache.spark.SparkConf;
import org.apache.spark.api.java.*;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.api.java.function.PairFunction;
import scala.Tuple2;

import static java.lang.Math.abs;


public class PageRank {
    private static int MaxIteration = 100;
    private static final double DAMPENING_FACTOR = 0.85;
    private static final double EPSILON = 0.0001;

    public static void main(String[] args) {
        SparkConf conf = new SparkConf().setAppName("PageRank");
        JavaSparkContext sc = new JavaSparkContext(conf);
        sc.setLogLevel("WARN");

        String linksFile = "hdfs:///user/hadoop/input/PageRank/links.txt";
        String pagesFile = "hdfs:///user/hadoop/input/PageRank/pages.txt";
        String rankFile = "hdfs:///user/hadoop/output/Graph/SparkPageRank";

        /**
         *  neighborRDD: (from, s)
         *  linksRDD: tuple (from, [to,1/m])
         *  pageRDD: vertex
         *  pageRankRDD: (point, 1/n)
         */


        JavaPairRDD<Integer, Integer> neighborRDD = sc.textFile(linksFile)
                .mapToPair(
                        line -> new Tuple2<>(
                                Integer.parseInt(line.split(" ")[0]), 1))
                .reduceByKey((x, y) -> x + y);

        JavaPairRDD<Integer, Tuple2<Integer, Integer>> linksRDD = sc.textFile(linksFile)
                .mapToPair(
                        line -> new Tuple2<>(
                                Integer.parseInt(line.split(" ")[0]),
                                Integer.parseInt(line.split(" ")[1])
                        ))
                .join(neighborRDD);

        JavaRDD<Integer> pagesRDD = sc.textFile(pagesFile).map(line -> Integer.parseInt(line));
        long pageCount = pagesRDD.count();
        JavaPairRDD<Integer, Double> pageRankRDD = pagesRDD.mapToPair(
                vertex -> new Tuple2<>(vertex, 1.0 / pageCount)
        );

        int count = 0;
        while (count < MaxIteration) {
            JavaPairRDD<Integer, Double> NewPageRankRDD = linksRDD.join(pageRankRDD)
                    .mapToPair(
                            new PairFunction<Tuple2<Integer, Tuple2<Tuple2<Integer, Integer>, Double>>, Integer, Double>() {
                                @Override
                                public Tuple2<Integer, Double> call(Tuple2<Integer, Tuple2<Tuple2<Integer, Integer>, Double>> ans) throws Exception {
//                               // [ toNode, fraction * rank]
                                    return new Tuple2<>(ans._2._1._1, ans._2._2/ans._2._1._2);
                                }
                            })
                    .reduceByKey((v1, v2) -> v1 + v2)
                    .mapValues(
                            new Function<Double, Double>() {
                                double dampening = DAMPENING_FACTOR;
                                double randomJump = (1 - DAMPENING_FACTOR) / pageCount;

                                @Override
                                public Double call(Double value) throws Exception {
                                    value = value * dampening + randomJump;
                                    return value;
                                }
                            }
                    );
            count++;
            JavaPairRDD<Integer, Tuple2<Double, Double>> compare = pageRankRDD.join(NewPageRankRDD).filter(each -> abs(each._2._1 - each._2._2) > EPSILON);
            if (compare.isEmpty() || count > MaxIteration)
                break;
            pageRankRDD = NewPageRankRDD;

        }

        pageRankRDD.saveAsTextFile(rankFile);

    }


}

//参考 : https://blog.csdn.net/gx304419380/article/details/79455833