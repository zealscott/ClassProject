import org.apache.storm.spout.SpoutOutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichSpout;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Values;
import org.apache.storm.utils.Utils;

import java.util.Map;
import java.util.UUID;

public class SentenceSpout extends BaseRichSpout {

    private SpoutOutputCollector spoutOutputCollector;
    private String[] sentences = {"the cow jumped over the moon", "an apple a day keeps the doctor away",
            "four score and seven years ago", "snow white and the seven dwarfs", "i am at two with nature"};

    public void open(Map map, TopologyContext topologycontext, SpoutOutputCollector spoutoutputcollector) {
        this.spoutOutputCollector = spoutoutputcollector;
    }

    public void nextTuple() {
        for (String sentence : sentences) {
            Values values = new Values(sentence);
            UUID msgId = UUID.randomUUID();
            this.spoutOutputCollector.emit(values, msgId);
        }
        Utils.sleep(1000);
    }

    public void declareOutputFields(OutputFieldsDeclarer outputfieldsdeclarer) {
        outputfieldsdeclarer.declare(new Fields("sentence"));
    }

}
