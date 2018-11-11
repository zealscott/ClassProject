import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.Function2;
import scala.Serializable;
import scala.Tuple2;

import java.util.ArrayList;
import java.util.List;

public class Kmeans implements KmeansInterface, Serializable {

    // every point has a cluster number and point(x,y)
    private List<Tuple2<Integer, Point>> oldCenterList = new ArrayList<>();
    private List<Tuple2<Integer, Point>> newCenterList = new ArrayList<>();
    private double threshold = 0.000001;


    /**
     * @param point
     * @return cluster belonged
     * @Method get the closest cluster for the point
     */
    public int findClosest(Point point) {
        int argmin = -1;
        double minimalDistance = Double.MAX_VALUE;
        for (Tuple2<Integer, Point> i : oldCenterList) {
            double distance = point.EuclideanDis(i._2);
            if (distance < minimalDistance) {
                minimalDistance = distance;
                argmin = i._1;
            }
        }
        return argmin;
    }


    /**
     * @param outFile string
     * @Method save center to txt
     */
    public void saveToFile(String outFile, JavaSparkContext sc) {
        List<String> outCenterList = new ArrayList<>();
//        format center points
        for (Tuple2<Integer, Point> tmp : newCenterList) {
            outCenterList.add(String.valueOf(tmp._2.getX()) + " " + String.valueOf(tmp._2.getY()));
        }

        JavaRDD<String> center = sc.parallelize(outCenterList);
        center.saveAsTextFile(outFile);
    }


    /**
     * @return False for not stable
     * @Method compare two cluster center with threshold
     */
    public boolean clusterCompare() {
        for (Tuple2<Integer, Point> oldCenter : oldCenterList) {
            int clusterNum = oldCenter._1;
            for (Tuple2<Integer, Point> newCenter : newCenterList) {
                if (newCenter._1 == clusterNum) {
                    double dis = oldCenter._2.EuclideanDis(newCenter._2);
                    if (dis > threshold)
                        return false;
                    break;
                }
            }
        }
        return true;
    }


    /**
     * @param kmeansRDD
     * @return init pointsRDD
     * @Method prepare Points RDD and select clusters randomly
     */
    public JavaPairRDD<Integer, Point> Prepare(JavaRDD<String> kmeansRDD) {
        // get the number of cluster
        String fisrtLine = kmeansRDD.first();
        int clusterCount = Integer.parseInt(fisrtLine.split(",")[0]);

//        filter first line and convert to <Point,clusternum>, init set all cluster number 1
        JavaPairRDD<Integer, Point> pointsRDD = kmeansRDD.filter(line -> !line.equals(fisrtLine)).mapToPair(
                line -> {
                    String[] splitLine = line.split(",");
                    double X = Double.parseDouble(splitLine[0]);
                    double Y = Double.parseDouble(splitLine[1]);
                    return new Tuple2<>(0, new Point(X, Y));
                }
        );

//       init center list
        oldCenterList.addAll(pointsRDD.take(clusterCount));

        for (int i = 0; i < clusterCount; i++) {
            Tuple2<Integer, Point> tmp = oldCenterList.get(i);
            oldCenterList.set(i, new Tuple2<>(i, tmp._2));
        }

        newCenterList.addAll(oldCenterList);

        return pointsRDD;

    }


    /**
     * @param pointsRDD to cluster
     * @return new classify PointsRDD
     * @method cluster and update new cluster center
     */
    public JavaPairRDD<Integer, Point> cluster(JavaPairRDD<Integer, Point> pointsRDD) {

        JavaPairRDD<Integer, Point> newPointsRDD = pointsRDD.mapToPair(
                kv -> new Tuple2<>(findClosest(kv._2), kv._2)
        );

        JavaPairRDD<Integer, Point> newClusterRDD = newPointsRDD
                .mapValues(
                        value -> new Tuple2<>(value, 1))
                .reduceByKey(
                        new Function2<Tuple2<Point, Integer>, Tuple2<Point, Integer>, Tuple2<Point, Integer>>() {
                            @Override
                            public Tuple2<Point, Integer> call(Tuple2<Point, Integer> value1, Tuple2<Point, Integer> value2) throws Exception {
                                Point tmp = new Point(value1._1.getX() + value2._1.getX(), value1._1.getY() + value2._1.getY());
                                int count = value1._2 + value2._2;
                                return new Tuple2<>(tmp, count);
                            }
                        }
                ).mapValues(
                        v -> new Point(v._1.getX() / v._2, v._1.getY() / v._2)
                );

        oldCenterList.clear();
        oldCenterList.addAll(newCenterList);
        // convert to list to store
        newCenterList.clear();
        newCenterList.addAll(newClusterRDD.collect());

        return newPointsRDD;
    }

}
