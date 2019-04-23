/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package grapher.ui;

import javafx.event.EventHandler;
import javafx.geometry.Point2D;
import javafx.scene.input.MouseEvent;

/**
 *
 * @author benaissm
 */
public class Interaction implements EventHandler<MouseEvent>{

    enum State {IDLE, WAITING, DRAGGING};
    State state = State.IDLE;
    GrapherCanvas canvas;
    Point2D p;
    
    Interaction(GrapherCanvas gc) {
        canvas = gc;
    }


    @Override
    public void handle(MouseEvent e) {
        switch(state) {
            case IDLE:
                switch(e.getEventType().getName()){
                    case "MOUSE_PRESSED":
                        p = new Point2D(e.getX(), e.getY());
                        state = State.WAITING;
                        break;
                    default:
                        break;
                }
                break;
            case WAITING:
                switch(e.getEventType().getName()){
                    case "MOUSE_RELEASED":
                        canvas.zoom(this.p, 5);
                        state = State.IDLE;
                        break;
                    case "MOUSE_DRAGGED":
                        Point2D position = new Point2D(e.getX(), e.getY());
                        if(p.distance(position) > 5){
                            canvas.translate(e.getX() - p.getX(), e.getY() - p.getY());
                            p = position;
                            state = State.DRAGGING;
                        }
                        break;
                }
                break;
            case DRAGGING : 
                
                break;
        }
    }
    
    
}
