import java.util.ArrayList;
import java.util.Stack;

public class CouplePoint {
	Point p;
	Point s;
	
	public CouplePoint(Point p, Point s) {
		this.p = p;
		this.s = s;
	}
	
	public boolean equals(CouplePoint c) {
		return (this.p.equals(c.p)) && (this.s.equals(c.s));
	}
	
	public ArrayList<CouplePoint> successeurs(int maxX, int maxY, Terrain t) {
		ArrayList<CouplePoint> a = new ArrayList<CouplePoint>();
		if (s.y >0 && s.y < maxY-1 && estDispo(s.x, s.y-1, t) && estDispo(s.x, s.y+1, t)) {
			System.out.println("T1");
			if (existeChemin(p.x, p.y, s.x, s.y-1, maxX, maxY, t)) {
				System.out.println("1");
				a.add(new CouplePoint(new Point(s.x, s.y), new Point(s.x, s.y+1)));
			}
			if (existeChemin(p.x, p.y, s.x, s.y+1, maxX, maxY, t)) {
				System.out.println("2");
				a.add(new CouplePoint(new Point(s.x, s.y), new Point(s.x, s.y-1)));
			}
		}
		if (s.x > 0 && s.x < maxX-1 && estDispo(s.x-1, s.y, t) && estDispo(s.x+1, s.y, t)) {
			System.out.println("T2");
			if (existeChemin(p.x, p.y, s.x-1, s.y, maxX, maxY, t)) {
				System.out.println("3");
				a.add(new CouplePoint(new Point(s.x, s.y), new Point(s.x+1, s.y)));
			}
			if (existeChemin(p.x, p.y, s.x+1, s.y, maxX, maxY, t)) {
				System.out.println("4");
				a.add(new CouplePoint(new Point(s.x, s.y), new Point(s.x-1, s.y)));
			}
		}
		return a;
	}
	
	public boolean estDispo(int x, int y, Terrain t) {
		
		return t.consulter(x, y).estLibre() && x != s.x && y != s.y;
	}
	
	public boolean existeChemin(int di, int dj, int fi, int fj, int maxX, int maxY, Terrain t) {
    	Stack<Point> e = new Stack<Point>();
    	int [][] marq = new int[maxX][maxY];
    	Point sc = new Point(di, dj);
    	Point sf = new Point(fi, fj);
    	marq[sc.x][sc.y] = 1;
    	e.add(sc);
    	while ((!e.isEmpty()) && (!sc.equals(sf))) {
    		sc = e.pop();
    		ArrayList<Point> a = new ArrayList<Point>();
    		a = sc.successeurs(maxX, maxY);
    		for (Point succ : a) {
    			if (estDispo(succ.x, succ.y, t)) {
    				if (marq[succ.x][succ.y] != 1) {
    					marq[succ.x][succ.y] = 1;
        				e.add(succ);
    				}
    			}
    		}
    	}
    	return sc.equals(sf);
    }
    
	
	public String toString() {
		return "(" + p.toString() + "," + s.toString() + ")";
	}
}
