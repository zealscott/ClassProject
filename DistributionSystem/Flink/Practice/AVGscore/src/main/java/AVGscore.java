import org.apache.commons.compress.archivers.dump.DumpArchiveEntry;
import org.apache.flink.api.common.typeinfo.Types;
import org.apache.flink.api.java.DataSet;
import org.apache.flink.api.java.ExecutionEnvironment;
import org.apache.flink.api.java.tuple.Tuple2;
import org.apache.flink.api.java.utils.ParameterTool;

public class AVGscore {
    private static Integer groupSize = 5;

    public static void main(String[] args) throws Exception {
        ParameterTool params = ParameterTool.fromArgs(args);
        ExecutionEnvironment env = ExecutionEnvironment.getExecutionEnvironment();
        env.getConfig().setGlobalJobParameters(params);
        DataSet<Tuple2<Integer, Double>> fileDataSet;

        if (params.has("input")) {
            fileDataSet = env.readCsvFile(params.get("input"))
                    .types(Integer.class, Double.class);
        } else {
            System.out.println("Please confirm input keywords!");
            return;
        }

        /**
         * map string to (id, score) and convert to (group_id, (score,1))
         * GroupBy and reduce == reduceByKey
         * and then map to avg score
         */
        DataSet<Tuple2<Integer, Double>> stuAVGscore = fileDataSet
                .map(line -> Tuple2.of(
                        (line.f0-1)/ 5, Tuple2.of(line.f1, 1)))
                .returns(Types.TUPLE(Types.INT, Types.TUPLE(Types.DOUBLE, Types.INT)))
                .groupBy(0)
                .reduce(
                        (kv1, kv2) -> Tuple2.of(kv1.f0, Tuple2.of(kv1.f1.f0 + kv2.f1.f0, kv1.f1.f1 + kv2.f1.f1)))
                .returns(Types.TUPLE(Types.INT, Types.TUPLE(Types.DOUBLE, Types.INT)))
                .map(
                        line -> Tuple2.of(line.f0, line.f1.f0 / line.f1.f1)
                ).returns(Types.TUPLE(Types.INT, Types.DOUBLE));



        //如果没有指定输出，则默认打印到控制台
        if (params.has("output")) {
            stuAVGscore.writeAsCsv(params.get("output"), "\n", ",");
            env.execute();
        } else {
            System.out.println("Printing result to stdout. Use --output to specify output path.");
            stuAVGscore.print();
        }

    }
}
