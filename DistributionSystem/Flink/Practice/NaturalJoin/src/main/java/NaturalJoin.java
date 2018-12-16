import org.apache.flink.api.common.functions.FlatJoinFunction;
import org.apache.flink.api.common.typeinfo.Types;
import org.apache.flink.api.java.DataSet;
import org.apache.flink.api.java.ExecutionEnvironment;
import org.apache.flink.api.java.tuple.Tuple2;
import org.apache.flink.api.java.tuple.Tuple3;
import org.apache.flink.api.java.tuple.Tuple4;
import org.apache.flink.api.java.utils.ParameterTool;
import org.apache.flink.util.Collector;
import scala.Int;

import java.lang.reflect.Type;

public class NaturalJoin {

    public static void main(String args[]) throws Exception{
        ParameterTool params = ParameterTool.fromArgs(args);
        ExecutionEnvironment env = ExecutionEnvironment.getExecutionEnvironment();
        env.getConfig().setGlobalJobParameters(params);

        // code, city
        DataSet<Tuple2<Integer, String>> addDataSet;
        // id ,name, code
        DataSet<Tuple3<Integer, String, Integer>> personDataSet;


        if (params.has("addinput")) {
            addDataSet = env.readCsvFile(params.get("addinput"))
                    .fieldDelimiter(" ")
                    .ignoreInvalidLines()
                    .types(Integer.class, String.class);
        } else {
            System.out.println("Please confirm input keywords!");
            return;
        }

        if (params.has("personinput")) {
            personDataSet = env.readCsvFile(params.get("personinput"))
                    .fieldDelimiter(" ")
                    .ignoreInvalidLines()
                    .types(Integer.class, String.class, Integer.class);
        } else {
            System.out.println("Please confirm input keywords!");
            return;
        }


        DataSet<Tuple4<Integer, String, Integer, String>> result = personDataSet.join(addDataSet)
                .where(2)
                .equalTo(0)
                .with(
                        (x, y) -> Tuple4.of(x.f0, x.f1, x.f2, y.f1)
                ).returns(Types.TUPLE(Types.INT,Types.STRING,Types.INT,Types.STRING));

        personDataSet.print();

        //如果没有指定输出，则默认打印到控制台
        if (params.has("output")) {
            result.writeAsCsv(params.get("output"), "\n", ",");
            env.execute();
        } else {
            System.out.println("Printing result to stdout. Use --output to specify output path.");
            result.print();
        }

    }
}
