import org.apache.flink.api.common.functions.*;
import org.apache.flink.api.java.DataSet;
import org.apache.flink.api.java.ExecutionEnvironment;
import org.apache.flink.api.java.functions.FunctionAnnotation;
import org.apache.flink.api.java.operators.IterativeDataSet;
import org.apache.flink.api.java.tuple.Tuple2;
import org.apache.flink.api.java.tuple.Tuple3;
import org.apache.flink.api.java.utils.ParameterTool;
import org.apache.flink.configuration.Configuration;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Collection;

public class Kmeans {

    public static void main(String[] args) throws Exception {
        // Checking input parameters
        final ParameterTool params = ParameterTool.fromArgs(args);
        // set up execution environment
        ExecutionEnvironment env = ExecutionEnvironment.getExecutionEnvironment();
        env.getConfig().setGlobalJobParameters(params); // make parameters available in the web interface

        // get input data:
        DataSet<Point> points = getPoint(params, env);
        if (points == null)
            return;
        DataSet<Centroid> centroids = getCentroid(params, env);


        // set number of bulk iterations for KMeans algorithm
        IterativeDataSet<Centroid> loop = centroids.iterate(params.getInt("iterations", 100));

        DataSet<Centroid> newCentroids = points
                // compute closest centroid for each point
                .map(new SelectNearestCenter()).withBroadcastSet(loop, "centroids")
                // count and sum point coordinates for each centroid
                .map(new CountAppender())
                .groupBy(0).reduce(new CentroidAccumulator())
                // compute new centroids from point counts and coordinate sums
                .map(new CentroidAverager());


        // feed new centroids back into next iteration
        DataSet<Centroid> finalCentroids = loop.closeWith(newCentroids, newCentroids.filter(new thresholdFilter()).withBroadcastSet(loop,"centroids"));

//        DataSet<Tuple2<Integer, Point>> clusteredPoints = points
//                // assign points to final clusters
//                .map(new SelectNearestCenter()).withBroadcastSet(finalCentroids, "centroids");

        // emit result
        if (params.has("output")) {
            finalCentroids.writeAsCsv(params.get("output"), "\n", " ");
            env.execute();
        } else {
            System.out.println("Printing result to stdout. Use --output to specify output path.");
            finalCentroids.print();
        }
    }

    private static DataSet<Point> getPoint(ParameterTool params, ExecutionEnvironment env) {
        DataSet<Point> points;
        if (params.has("input")) {
            // read points from CSV file
            points = env.readCsvFile(params.get("input"))
                    .ignoreFirstLine()
                    .pojoType(Point.class, "x", "y");
        } else {
            System.out.println("Use --input to specify file input.");
            return null;
        }
        return points;
    }

    private static DataSet<Centroid> getCentroid(ParameterTool params, ExecutionEnvironment env) throws Exception {
        ArrayList<Centroid> centroidArrayList = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(params.get("input")));
        String text = br.readLine();
        int k = Integer.parseInt(text.split(",")[0]);

        while (k != 0) {
            text = br.readLine();
            double x = Double.parseDouble(text.split(",")[0]);
            double y = Double.parseDouble(text.split(",")[1]);
            centroidArrayList.add(new Centroid(k, x, y));
            k--;
        }

        DataSet<Centroid> centroids = env.fromCollection(centroidArrayList);
        return centroids;
    }

    /**
     * Determines the closest cluster center for a data point.
     */
    @FunctionAnnotation.ForwardedFields("*->1")
    public static final class SelectNearestCenter extends RichMapFunction<Point, Tuple2<Integer, Point>> {
        private Collection<Centroid> centroids;

        /**
         * Reads the centroid values from a broadcast variable into a collection.
         */
        @Override
        public void open(Configuration parameters) throws Exception {
            this.centroids = getRuntimeContext().getBroadcastVariable("centroids");
        }

        @Override
        public Tuple2<Integer, Point> map(Point p) throws Exception {

            double minDistance = Double.MAX_VALUE;
            int closestCentroidId = -1;

            // check all cluster centers
            for (Centroid centroid : centroids) {
                // compute distance
                double distance = p.euclideanDistance(centroid);

                // update nearest cluster if necessary
                if (distance < minDistance) {
                    minDistance = distance;
                    closestCentroidId = centroid.id;
                }
            }

            // emit a new record with the center id and the data point.
            return new Tuple2<>(closestCentroidId, p);
        }
    }

    /**
     * Appends a count variable to the tuple.
     */
    @FunctionAnnotation.ForwardedFields("f0;f1")
    public static final class CountAppender implements MapFunction<Tuple2<Integer, Point>, Tuple3<Integer, Point, Long>> {
        @Override
        public Tuple3<Integer, Point, Long> map(Tuple2<Integer, Point> t) {
            return new Tuple3<>(t.f0, t.f1, 1L);
        }
    }

    /**
     * Sums and counts point coordinates.
     */
    @FunctionAnnotation.ForwardedFields("0")
    public static final class CentroidAccumulator implements ReduceFunction<Tuple3<Integer, Point, Long>> {

        @Override
        public Tuple3<Integer, Point, Long> reduce(Tuple3<Integer, Point, Long> val1, Tuple3<Integer, Point, Long> val2) {
            return new Tuple3<>(val1.f0, val1.f1.add(val2.f1), val1.f2 + val2.f2);
        }
    }

    /**
     * Computes new centroid from coordinate sum and count of points.
     */
    @FunctionAnnotation.ForwardedFields("0->id")
    public static final class CentroidAverager implements MapFunction<Tuple3<Integer, Point, Long>, Centroid> {
        @Override
        public Centroid map(Tuple3<Integer, Point, Long> value) {
            // id, X/num Y/num
            return new Centroid(value.f0, value.f1.div(value.f2));
        }
    }


    /**
     * Filter that filters vertices where the centorid difference is below a threshold.
     */
    public static final class thresholdFilter extends RichFilterFunction<Centroid> {
        private Collection<Centroid> centroids;
        private double threshold = 1e-5;

        /**
         * Reads the centroid values from a broadcast variable into a collection.
         */
        @Override
        public void open(Configuration parameters) throws Exception {
            this.centroids = getRuntimeContext().getBroadcastVariable("centroids");
        }

        @Override
        public boolean filter(Centroid centroid) {
            for (Centroid oldcentroid : centroids) {
                if (centroid.id == oldcentroid.id) {
                    // compute distance
                    double distance = centroid.euclideanDistance(oldcentroid);
                    return (distance > this.threshold);
                }
            }
            return true;
        }
    }
}
