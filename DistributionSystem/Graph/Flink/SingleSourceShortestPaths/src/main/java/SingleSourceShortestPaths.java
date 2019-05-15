import org.apache.flink.api.common.functions.FlatJoinFunction;
import org.apache.flink.api.common.functions.JoinFunction;
import org.apache.flink.api.common.functions.MapFunction;
import org.apache.flink.api.common.typeinfo.Types;
import org.apache.flink.api.java.DataSet;
import org.apache.flink.api.java.ExecutionEnvironment;
import org.apache.flink.api.java.aggregation.Aggregations;
import org.apache.flink.api.java.functions.FunctionAnnotation;
import org.apache.flink.api.java.operators.DeltaIteration;
import org.apache.flink.api.java.tuple.Tuple1;
import org.apache.flink.api.java.tuple.Tuple2;
import org.apache.flink.api.java.tuple.Tuple3;
import org.apache.flink.api.java.utils.ParameterTool;
import org.apache.flink.util.Collector;

@SuppressWarnings("serial")
public class SingleSourceShortestPaths {
    public static int sourceVerticeID = 1;

    public static void main(String[] args) throws Exception {


        final ParameterTool params = ParameterTool.fromArgs(args);

        ExecutionEnvironment env = ExecutionEnvironment.getExecutionEnvironment();

        DataSet<Tuple3<Integer, Integer, Double>> edges = getEdgesDataSet(params, env);
        DataSet<Tuple2<Integer, Double>> vertices = getVerticesDataSet(params, env);

        DeltaIteration<Tuple2<Integer, Double>, Tuple2<Integer, Double>> iteration = vertices
                .iterateDelta(vertices, 100,0);


        DataSet<Tuple2<Integer, Double>> NewSolutionSet = iteration.getWorkset()
                .join(edges).where(0).equalTo(0)
                .with(new FindDistance())
                .groupBy(0).aggregate(Aggregations.MIN, 1)
                .join(iteration.getSolutionSet()).where(0).equalTo(0)
                .with(new DistanceFilter());

        // close the delta iteration (changes are empty)
        DataSet<Tuple2<Integer, Double>> result = iteration.closeWith(NewSolutionSet, NewSolutionSet);


        // emit result
        if (params.has("output")) {
            result.writeAsCsv(params.get("output"), "\n", " ");
            // execute program
            env.execute("Connected Components Example");
        } else {
            System.out.println("Printing result to stdout. Use --output to specify output path.");
            result.print();
        }

    }


    public static final class DistanceFilter implements FlatJoinFunction<Tuple2<Integer, Double>, Tuple2<Integer, Double>, Tuple2<Integer, Double>> {
        @Override
        public void join(Tuple2<Integer, Double> candidate, Tuple2<Integer, Double> old, Collector<Tuple2<Integer, Double>> out) throws Exception {
            if (candidate.f1 < old.f1)
                out.collect(candidate);
        }
    }


    /**
     * (from,to,dis) join (point,dis)
     * produces a (point, distance) pair.
     */
    @FunctionAnnotation.ForwardedFieldsSecond("f1->f0")
    public static final class FindDistance implements JoinFunction<Tuple2<Integer, Double>, Tuple3<Integer, Integer, Double>, Tuple2<Integer, Double>> {
        @Override
        public Tuple2<Integer, Double> join(Tuple2<Integer, Double> vertices, Tuple3<Integer, Integer, Double> edges) throws Exception {
            return Tuple2.of(edges.f1, vertices.f1 < Double.POSITIVE_INFINITY ? vertices.f1 + edges.f2 : Double.POSITIVE_INFINITY);
        }
    }


    /**
     * Get Edges data
     *
     * @param params
     * @param env
     * @return
     */
    private static DataSet<Tuple3<Integer, Integer, Double>> getEdgesDataSet(ParameterTool params, ExecutionEnvironment env) {
        if (params.has("edges")) {
            return env.readCsvFile(params.get("edges"))
                    .fieldDelimiter(" ")
                    .types(Integer.class, Integer.class, Double.class);
        } else {
            return SingleSourceShortestPathsData.getDefaultEdgeDataSet(env);
        }
    }

    /**
     * Get Vertices data
     *
     * @param params
     * @param env
     * @return
     */
    private static DataSet<Tuple2<Integer, Double>> getVerticesDataSet(ParameterTool params, ExecutionEnvironment env) {
        DataSet<Integer> vertices;
        if (params.has("vertices")) {
            vertices = env.readCsvFile(params.get("vertices")).types(Integer.class).map(
                    new MapFunction<Tuple1<Integer>, Integer>() {
                        public Integer map(Tuple1<Integer> value) {
                            return value.f0;
                        }
                    });
        } else
            vertices = env.fromElements(1, 2, 3, 4, 5);
        return vertices.map(new MapFunction<Integer, Tuple2<Integer, Double>>() {
            @Override
            public Tuple2<Integer, Double> map(Integer integer) throws Exception {
                if (integer == sourceVerticeID)
                    return Tuple2.of(integer, 0.0);
                else
                    return Tuple2.of(integer, Double.POSITIVE_INFINITY);
            }
        });
    }


}