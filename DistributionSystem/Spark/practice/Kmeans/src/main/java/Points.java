import scala.Serializable;

class Point implements Serializable {
    double x;
    double y;
    Point(){

    }

    Point(double x,double y){
        this.x=x;
        this.y=y;
    }

    public double EuclideanDis(Point other) {
        double distance = 0;

        distance = Math.pow((this.x - other.getX()),2) + Math.pow((this.y - other.getY()),2);

        return Math.sqrt(distance);
    }

    public double getX()
    {
        return x;
    }
    public double getY(){
        return y;
    }

    public void setX(double x){
        this.x = x;
    }

    public void setY(double y) {
        this.y = y;
    }

    public void addX(double x){
        this.x += x;
    }

    public void addY(double y){
        this.y += y;
    }

}