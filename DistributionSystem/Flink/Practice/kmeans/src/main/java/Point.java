import java.io.Serializable;

public class Point implements Serializable {
    public double x,y;

    public Point() {

    }

    public Point(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public Point add(Point other) {
        x += other.x;
        y += other.y;
        return this;
    }

    public Point div(long val) {
        x /= val;
        y /= val;
        return this;
    }

    public double euclideanDistance(Point other) {
        return Math.sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    public String toString() {
        return x + " " + y;
    }

}