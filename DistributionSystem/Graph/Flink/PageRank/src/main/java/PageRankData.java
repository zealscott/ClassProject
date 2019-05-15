import org.apache.flink.api.java.DataSet;
import org.apache.flink.api.java.ExecutionEnvironment;
import org.apache.flink.api.java.tuple.Tuple2;

import java.util.ArrayList;
import java.util.List;

/**
 * Provides the default data sets used for the PageRank example program.
 * The default data sets are used, if no parameters are given to the program.
 *
 */
public class PageRankData {

    public static final Object[][] EDGES = {
            {1L, 2L},
            {1L, 15L},
            {2L, 3L},
            {2L, 4L},
            {2L, 5L},
            {2L, 6L},
            {2L, 7L},
            {3L, 13L},
            {4L, 2L},
            {5L, 11L},
            {5L, 12L},
            {6L, 1L},
            {6L, 7L},
            {6L, 8L},
            {7L, 1L},
            {7L, 8L},
            {8L, 1L},
            {8L, 9L},
            {8L, 10L},
            {9L, 14L},
            {9L, 1L},
            {10L, 1L},
            {10L, 13L},
            {11L, 12L},
            {11L, 1L},
            {12L, 1L},
            {13L, 14L},
            {14L, 12L},
            {15L, 1L},
    };

    private static int numPages = 15;

    public static DataSet<Tuple2<Long, Long>> getDefaultEdgeDataSet(ExecutionEnvironment env) {

        List<Tuple2<Long, Long>> edges = new ArrayList<Tuple2<Long, Long>>();
        for (Object[] e : EDGES) {
            edges.add(new Tuple2<Long, Long>((Long) e[0], (Long) e[1]));
        }
        return env.fromCollection(edges);
    }

    public static DataSet<Long> getDefaultPagesDataSet(ExecutionEnvironment env) {
        return env.generateSequence(1, 15);
    }

    public static int getNumberOfPages() {
        return numPages;
    }

}