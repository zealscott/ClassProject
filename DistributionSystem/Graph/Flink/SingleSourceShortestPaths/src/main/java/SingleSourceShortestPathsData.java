import org.apache.flink.api.java.DataSet;
import org.apache.flink.api.java.ExecutionEnvironment;
import org.apache.flink.api.java.tuple.Tuple3;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class SingleSourceShortestPathsData {

    public static final Object[][] DEFAULT_EDGES = new Object[][]{
            new Object[]{1, 2, 12.0},
            new Object[]{1, 3, 13.0},
            new Object[]{2, 3, 23.0},
            new Object[]{3, 4, 34.0},
            new Object[]{3, 5, 35.0},
            new Object[]{4, 5, 45.0},
            new Object[]{5, 1, 51.0}
    };

    public static final String RESULTED_SINGLE_SOURCE_SHORTEST_PATHS = "1,0.0\n" + "2,12.0\n" + "3,13.0\n" +
            "4,47.0\n" + "5,48.0";

    public static DataSet<Tuple3<Integer, Integer, Double>> getDefaultEdgeDataSet(ExecutionEnvironment env) {

        List<Tuple3<Integer, Integer, Double>> edgeList = new ArrayList<>();
        for (Object[] edge : DEFAULT_EDGES) {
            edgeList.add(Tuple3.of((Integer) edge[0], (Integer) edge[1], (Double) edge[2]));
        }
        return env.fromCollection(edgeList);
    }

}