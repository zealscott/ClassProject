import org.apache.storm.Config;
import org.apache.storm.LocalCluster;
import org.apache.storm.topology.TopologyBuilder;
import org.apache.storm.tuple.Fields;

public class WordCountTopology {

    public static void main(String[] args) throws Exception {

        SentenceSpout sentenceSpout = new SentenceSpout();
        SplitSentenceBolt splitSentenceBolt = new SplitSentenceBolt();
        WordCountBolt wordCountBolt = new WordCountBolt();

        TopologyBuilder builder = new TopologyBuilder();
        builder.setSpout("sentenceSpout-1", sentenceSpout);
        builder.setBolt("splitSentenceBolt-1", splitSentenceBolt).shuffleGrouping("sentenceSpout-1");
        builder.setBolt("wordCountBolt-1", wordCountBolt).fieldsGrouping("splitSentenceBolt-1", new Fields("word"));

        Config config = new Config();
        LocalCluster cluster = new LocalCluster();

        cluster.submitTopology("wordCountTopology-1", config, builder.createTopology());
        Thread.sleep(999999999);
        cluster.shutdown();
    }

}
