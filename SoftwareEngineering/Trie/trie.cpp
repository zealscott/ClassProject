class TrieNode // 字典树节点
{
    private int num;// 有多少单词通过这个节点,即由根至该节点组成的字符串模式出现的次数
    private TrieNode[] son;// 所有的儿子节点
    private boolean isEnd;// 是不是最后一个节点
    private char val;// 节点的值

    TrieNode()
    {
        num = 1;
        son = new TrieNode[SIZE];
        isEnd = false;
    }
}


Trie() // 初始化字典树
    {
        root = new TrieNode();
    }

// 建立字典树
    public void insert(String str) // 在字典树中插入一个单词
    {
        if (str == null || str.length() == 0)
        {
            return;
        }
        TrieNode node = root;
        char[] letters = str.toCharArray();//将目标单词转换为字符数组
        for (int i = 0, len = str.length(); i < len; i++)
        {
            int pos = letters[i] - 'a';
            if (node.son[pos] == null)  //如果当前节点的儿子节点中没有该字符，则构建一个TrieNode并复值该字符
            {
                node.son[pos] = new TrieNode();
                node.son[pos].val = letters[i];
            } 
            else   //如果已经存在，则将由根至该儿子节点组成的字符串模式出现的次数+1
            {
                node.son[pos].num++;
            }
            node = node.son[pos];
        }
        node.isEnd = true;
    }