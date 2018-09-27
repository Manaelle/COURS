/*
Sokoban - implémentation manuelle et automatique du célèbre jeu
Copyright (C) 2009 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
import java.awt.event.*;

import Dessin.Fenetre;

class EcouteurDeClavier implements KeyListener {
    Moteur m;
    
	public EcouteurDeClavier(Moteur m) {
		super();
		this.m = m;
	}

	public void keyPressed(KeyEvent e) {
        switch (e.getKeyCode()) {
        case KeyEvent.VK_UP:
            System.out.println("Up");
            break;
        case KeyEvent.VK_RIGHT:
            System.out.println("Right");
            break;
        case KeyEvent.VK_DOWN:
            System.out.println("Down");
            break;
        case KeyEvent.VK_LEFT:
            System.out.println("Left");
            break;
        case KeyEvent.VK_H:
            System.out.println("H");
            m.solve();
            break;
        default:
            System.out.println(e.getKeyCode());
        }
    }

    public void keyReleased(KeyEvent e) {}
    public void keyTyped(KeyEvent e) {}
}
