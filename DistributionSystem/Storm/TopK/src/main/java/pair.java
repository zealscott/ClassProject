
public class pair {
    public final String content;
    public final Integer count;

    public pair(String content, Integer count) {
        this.content = content;
        this.count = count;
    }

    public int compareCount(pair other) {
        return this.count - other.count;
    }

    public int compareWord(pair other) {
        return this.content.compareTo(other.content);
    }
}