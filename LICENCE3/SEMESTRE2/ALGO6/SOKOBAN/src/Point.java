import java.util.ArrayList;

public class Point {
	public int x;
	public int y;
	
	public Point(int x, int y) {
		super();
		this.x = x;
		this.y = y;
	}
	
	public boolean equals(Point p) {
		return (this.x == p.x) && (this.y == p.y);
	}
	
	public ArrayList<Point> successeurs(int maxX, int maxY) {
		ArrayList<Point> a = new ArrayList<Point>();
		if (y > 0) {
			a.add(new Point(this.x, this.y-1));
		}
		if (y < maxY-1) {
			a.add(new Point(this.x, this.y+1));
		}
		if (x > 0) {
			a.add(new Point(this.x-1, this.y));
		}
		if (x < maxX-1) {
			a.add(new Point(this.x+1, this.y));
		}
		return a;
	}
	
	public String toString() {
		return "(" + x + "," + y + ")";
	}
	
}
