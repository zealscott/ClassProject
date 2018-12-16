

public interface KmeansInterface {

    /**
     * @param point
     * @return cluster belonged
     * @Method get the closest cluster for the point
     */
    int findClosest(Point point);


    /**
     * @return False for not stable
     * @Method compare two cluster center with threshold
     */
    boolean clusterCompare();


    /**
     * @param outFile string
     * @Method save center to txt
     */
    void saveToFile(String outFile, JavaSparkContext sc);


    /**
     * @param kmeansRDD
     * @return init pointsRDD
     * @Method prepare Points RDD and select clusters randomly
     */
    public JavaPairRDD<Integer, Point> Prepare(JavaRDD<String> kmeansRDD);


    /**
     * @param pointsRDD to cluster
     * @return new classify PointsRDD
     * @method cluster and update new cluster center
     */
    public JavaPairRDD<Integer, Point> cluster(JavaPairRDD<Integer, Point> pointsRDD);

}
