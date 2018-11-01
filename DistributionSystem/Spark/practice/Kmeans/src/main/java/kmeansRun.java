import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import scala.Tuple2;

import java.util.ArrayList;
import java.util.List;

public class kmeansRun {

    public static void main(String args[]) {
        Kmeans kmeans = new Kmeans();

        SparkConf conf = new SparkConf().setAppName("Kmeans");
        JavaSparkContext sc = new JavaSparkContext(conf);

        String kmeansFile = "hdfs://10.11.6.91:9000/user/hadoop7/input/k-means.dat";
        String outFile = "hdfs://10.11.6.91:9000/user/hadoop7/output/spark/kmeans";

        int counter = 1, maxIteration = 500;
        boolean result = false;

        JavaRDD<String> kmeansRDD = sc.textFile(kmeansFile).cache();

//        init centerList and Points RDD
        JavaPairRDD<Integer, Point> PointsRDD = kmeans.Prepare(kmeansRDD);

        while (!result && counter < maxIteration) {
            PointsRDD = kmeans.cluster(PointsRDD);

            result = kmeans.clusterCompare();
            System.out.println("*******KMEANS finished iteration:>> " + counter + " || means stable: " + result);
            counter++;
        }


        kmeans.saveToFile(outFile,sc);


    }
}
