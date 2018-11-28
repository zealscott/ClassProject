import org.apache.storm.topology.BasicOutputCollector;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseBasicBolt;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Tuple;
import org.apache.storm.tuple.Values;

import java.util.ArrayList;
import java.util.List;

public class WordSortBolt extends BaseBasicBolt {
    List<String> wordList = new ArrayList<String>();

    /**
     * @param array
     * @param key
     * @return
     */
    public int arrayIndexOf(String key) {
        int min, max, mid;
        min = 0;
        max = wordList.size() - 1;

        while (min <= max) {
            mid = (min + max) >> 1;
            String tmp = wordList.get(mid);
            if (key.compareTo(tmp) > 0) {
                min = mid + 1;
            } else if (key.compareTo(tmp) < 0) {
                max = mid - 1;
            } else {
                return mid;
            }
        }
        return min;
    }


    public void execute(Tuple tuple, BasicOutputCollector collector) {
        String word = tuple.getString(0);
        if (wordList == null) {
            wordList.add(word);
            System.out.println(word);
        } else {
            int addIndex = arrayIndexOf(word);
            wordList.add(addIndex,word);
            for (String tmp:wordList
                 ) {
                System.out.println(tmp);
            }
        }
        collector.emit(new Values(word));
    }

    public void declareOutputFields(OutputFieldsDeclarer outputfieldsdeclarer) {
        outputfieldsdeclarer.declare(new Fields("word"));
    }
}
