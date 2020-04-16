import java.util.HashMap;
import java.util.Map;

import org.apache.storm.task.OutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichBolt;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Tuple;
import org.apache.storm.tuple.Values;

public class WordCountBolt extends BaseRichBolt {

    private OutputCollector outputCollector;
    private HashMap<String, Integer> counts = null;

    public void prepare(Map map, TopologyContext topologycontext, OutputCollector outputcollector) {
        this.outputCollector = outputcollector;
        this.counts = new HashMap<String, Integer>();
    }

    public void execute(Tuple tuple) {
        String word = tuple.getStringByField("word");
        Integer count = counts.get(word);
        if (count == null) {
            count = 0;
        }
        count++;
        this.counts.put(word, count);
        this.outputCollector.emit(new Values(word, count));
        this.outputCollector.ack(tuple);
        System.out.println(word + ": " + count);
    }

    public void declareOutputFields(OutputFieldsDeclarer outputfieldsdeclarer) {
        outputfieldsdeclarer.declare(new Fields("word", "count"));
    }

}
