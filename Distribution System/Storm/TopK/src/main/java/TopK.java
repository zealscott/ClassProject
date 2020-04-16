import org.apache.storm.topology.BasicOutputCollector;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseBasicBolt;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Tuple;
import org.apache.storm.tuple.Values;

import java.util.ArrayList;
import java.util.HashMap;


public class TopK extends BaseBasicBolt {
    private HashMap<String, Integer> counts;
    private ArrayList<pair> TopList;
    public int K;
    public int minCount;

    TopK(int k) {
        this.K = k;
        this.counts = new HashMap<>();
        this.TopList = new ArrayList<>();
        this.minCount = 0;
    }


    public void insertWord(pair word) {
        int max = TopList.size() - 1;

//        if the same word, the new.count > old.count
        for (int i = max; i >= 0; i--) {
            pair tmp = TopList.get(i);
            // find the same word,replace older one
            if (word.compareWord(tmp) == 0) {
                TopList.set(i, word);
                return;
            }
            if (word.compareCount(tmp) <= 0) {
                TopList.add(i + 1, word);
                return;
            }
        }
        TopList.add(0, word);
    }


    public void execute(Tuple tuple, BasicOutputCollector collector) {
        String word = tuple.getStringByField("word");
        Integer count = counts.get(word);
        if (count == null) {
            count = 0;
        }
        count++;
        this.counts.put(word, count);
        if (count > minCount || TopList.size() < K) {
            insertWord(new pair(word, count));
            if (TopList.size() > K) {
                TopList.remove(TopList.size() - 1);
                minCount = TopList.get(TopList.size() - 1).count;
            }
            for (pair tmpWord : TopList)
                System.out.println(tmpWord.content + " " + tmpWord.count.toString());
            collector.emit(new Values(word, count));
        }


    }

    public void declareOutputFields(OutputFieldsDeclarer outputfieldsdeclarer) {
        outputfieldsdeclarer.declare(new Fields("word", "count"));
    }
}
