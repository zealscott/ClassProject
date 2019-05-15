import org.apache.storm.Config;
import org.apache.storm.LocalCluster;
import org.apache.storm.topology.TopologyBuilder;
import org.apache.storm.tuple.Fields;

public class WordTopKTopology {

    public static void main(String[] args) throws Exception {

        SentenceSpout sentenceSpout = new SentenceSpout();
        SplitSentenceBolt splitSentenceBolt = new SplitSentenceBolt();
        TopK wordTopKBolt = new TopK(5);

        TopologyBuilder builder = new TopologyBuilder();
        builder.setSpout("sentenceSpout-1", sentenceSpout);
        builder.setBolt("splitSentenceBolt-1", splitSentenceBolt).shuffleGrouping("sentenceSpout-1");
        builder.setBolt("wordTopKBolt-1", wordTopKBolt).shuffleGrouping("splitSentenceBolt-1");

        Config config = new Config();
        LocalCluster cluster = new LocalCluster();

        cluster.submitTopology("wordTopKTopology-1", config, builder.createTopology());
        Thread.sleep(999999999);
        cluster.shutdown();
    }

}
