import org.apache.flink.api.common.functions.MapFunction;
import org.apache.flink.api.java.DataSet;
import org.apache.flink.api.java.ExecutionEnvironment;
import org.apache.flink.api.java.tuple.Tuple2;
import org.apache.flink.api.java.utils.ParameterTool;
import java.util.Random;

public class StuScore {
    private static Random rand = new Random();

    public StuScore(){}

    public static void main(String[] args) throws Exception {
        ParameterTool params = ParameterTool.fromArgs(args);
        ExecutionEnvironment env = ExecutionEnvironment.getExecutionEnvironment();
        env.getConfig().setGlobalJobParameters(params);
        DataSet<String> text;

        if(params.has("input")){
            text = env.readTextFile(params.get("input"));
        }else{
            System.out.println("Please confirm input keywords!");
            return;
        }


        DataSet<Tuple2<String,Integer>> stuscore =  text.map(new MapFunction<String, Tuple2<String, Integer>>() {
            @Override
            public Tuple2<String, Integer> map(String s) throws Exception {
                return new Tuple2<>(s,rand.nextInt(100) +1);
            }
        });

        //如果没有指定输出，则默认打印到控制台
        if(params.has("output")){
            stuscore.writeAsCsv(params.get("output"),"\n", ",");
            env.execute();
        }else{
            System.out.println("Printing result to stdout. Use --output to specify output path.");
            stuscore.print();
        }

    }
}
