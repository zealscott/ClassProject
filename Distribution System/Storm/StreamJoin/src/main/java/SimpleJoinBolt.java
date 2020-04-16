import org.apache.storm.Config;
import org.apache.storm.generated.GlobalStreamId;
import org.apache.storm.task.OutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichBolt;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Tuple;
import org.apache.storm.utils.TimeCacheMap;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;



public class SimpleJoinBolt extends BaseRichBolt {
    private OutputCollector _collector;
    private Fields _outFields;
    private Fields _idFields;
    int _numSources;
    Map<String, GlobalStreamId> _fieldLocations;

    //在内存中保留近期活跃的对象
    //由于bolt在接收两个数据源的流数据时，同一id两个数据流很可能不会再一个时间点同时发出，因此需要对数据流先进行缓存，直到所有
    //id相同的数据源都被后被聚合处理，做完聚合处理后再将对应的tuple信息从缓存中删除。在
    TimeCacheMap<List<Object>,Map<GlobalStreamId, Tuple>> _pending;

    //传进的Fields是聚合后将被输出的字段
    public SimpleJoinBolt(Fields outFields){
        this._outFields=outFields;
    }
    public void execute(Tuple tuple) {
        // TODO Auto-generated method stub
        //从tuple中获取_idFields字段，如果不存在于等待被处理的队列_pending中，则加入一行
        List<Object> id=tuple.select(_idFields);
        GlobalStreamId streamId=new GlobalStreamId(tuple.getSourceComponent(),tuple.getSourceStreamId());
        //打印当前处理元组的来源Spout
        System.out.println("元组来源："+tuple.getSourceComponent());
        //打印当前元组
        System.out.println("接收的元组："+tuple.getFields().get(0)+" = "+tuple.getValues().get(0)+" , "+tuple.getFields().get(1)+" = "+tuple.getValues().get(1));
        //如果当前pending中还不存在join key为此id的元组，则将该条记录加入
        if(!_pending.containsKey(id)){
            _pending.put(id, new HashMap<GlobalStreamId,Tuple>());
        }
        //从_pending队列中获取当前GlobalStreamId对应的HashMap对象
        Map<GlobalStreamId,Tuple> parts=_pending.get(id);
        //如果streamId已经包含其中，则抛出异常，接收到同一个spout中的两条一样id的tuple，否则将该streamId加入parts中
        if(parts.containsKey(streamId)){
            throw new RuntimeException("Received same side of single join twice");
        }
        parts.put(streamId, tuple);

        //如果parts中已经包含了聚合数据源的个数，则从_pending队列中移除这条记录
        if(parts.size()==_numSources){
            _pending.remove(id);
            List<Object> joinResult=new ArrayList<Object>();
            for(String outField:_outFields){
                GlobalStreamId loc=_fieldLocations.get(outField);
                joinResult.add(parts.get(loc).getValueByField(outField));
            }
            //输出聚合结果
            System.out.print("两条关系流中id值为"+id.get(0)+"的元组均已收到，聚合结果为：");
            for(Object obj:joinResult){
                System.out.print(obj+" ");
            }
            System.out.println();

            //多锚定
            _collector.emit(new ArrayList<Tuple>(parts.values()),joinResult);
            for (Tuple part : parts.values()) {
                _collector.ack(part);
            }
        }else{
            System.out.println("只从一个关系流中收取到id值为"+id+"的元组，不可进行join操作");
        }
    }

    public void prepare(Map conf, TopologyContext context, OutputCollector collector) {
        // TODO Auto-generated method stub
        _fieldLocations = new HashMap<String, GlobalStreamId>();
        this._collector=collector;
        //创建TimeCacheMap对象，设置超时回调接口，用于tuple处理失败时fail消息
        int timeout=((Number)conf.get(Config.TOPOLOGY_MESSAGE_TIMEOUT_SECS)).intValue();
        _pending=new TimeCacheMap<List<Object>,Map<GlobalStreamId,Tuple>>(timeout,new ExpireCallback());
        //记录数据源的数据个数
        _numSources=context.getThisSources().size();

        Set<String> idFields=null;
        //遍历TopologyContext中不同的数据源:genderSpout和ageSpout
        System.out.println(context.getThisSources().keySet());
        for(GlobalStreamId source:context.getThisSources().keySet()){
            //得到公共的Fields字段id,保存到_idFields中
            Fields fields=context.getComponentOutputFields(source.get_componentId(),source.get_streamId());
            //fields:[id,gender],[id,age]
            Set<String> setFields=new HashSet<String>(fields.toList());
            if(idFields==null){
                idFields=setFields;
            }else{
                //求交集
                idFields.retainAll(setFields);
                System.out.println(idFields);
            }
            //同时将_outFields中字段所在数据源记录下来，保存到一张HashMap _fieldLocations中，以便聚合后获取对应的字段值
            for(String outfield:_outFields){
                for(String sourcefield:fields){
                    if(outfield.equals(sourcefield)){
                        _fieldLocations.put(outfield, source);
                    }
                }
            }
            //打印结果:gender=GlobalStreamId(componentId=gender-spout,streamId=default)
            //age=GlobalStreamId(componentId=age-spout,streamId=default)
            System.out.println(_fieldLocations);

        }
        _idFields=new Fields(new ArrayList<String>(idFields));
        if(_fieldLocations.size()!=_outFields.size()){
            throw new RuntimeException("Cannot find all outfields among sources");
        }
    }

    public void declareOutputFields(OutputFieldsDeclarer declarer) {
        // TODO Auto-generated method stub
        declarer.declare(_outFields);
    }

    private class ExpireCallback implements TimeCacheMap.ExpiredCallback<List<Object>, Map<GlobalStreamId,Tuple>>{

        public void expire(List<Object> key, Map<GlobalStreamId, Tuple> tuples) {
            // TODO Auto-generated method stub
            for(Tuple tuple:tuples.values()){
                _collector.fail(tuple);
            }
        }

    }

}
