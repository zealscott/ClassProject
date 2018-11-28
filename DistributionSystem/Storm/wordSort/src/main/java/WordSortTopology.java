import org.apache.storm.Config;
import org.apache.storm.LocalCluster;
import org.apache.storm.topology.TopologyBuilder;
import org.apache.storm.tuple.Fields;

public class WordSortTopology {

    public static void main(String[] args) throws Exception {

        SentenceSpout sentenceSpout = new SentenceSpout();
        SplitSentenceBolt splitSentenceBolt = new SplitSentenceBolt();
        WordSortBolt wordSortBolt = new WordSortBolt();

        TopologyBuilder builder = new TopologyBuilder();
        builder.setSpout("sentenceSpout-1", sentenceSpout);
        builder.setBolt("splitSentenceBolt-1", splitSentenceBolt).shuffleGrouping("sentenceSpout-1");
        builder.setBolt("wordSortBolt-1", wordSortBolt).fieldsGrouping("splitSentenceBolt-1", new Fields("word"));

        Config config = new Config();
        LocalCluster cluster = new LocalCluster();

        cluster.submitTopology("wordSortTopology-1", config, builder.createTopology());
        Thread.sleep(999999999);
        cluster.shutdown();
    }

}
