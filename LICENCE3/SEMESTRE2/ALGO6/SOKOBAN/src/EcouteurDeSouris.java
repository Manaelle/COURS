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
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import Dessin.Fenetre;

class EcouteurDeSouris implements MouseListener {
    Fenetre f;
    TerrainGraphique tg;
    Moteur m;

    EcouteurDeSouris(Fenetre f, TerrainGraphique tg, Moteur m) {
        this.f = f;
        this.tg = tg;
        this.m = m;
    }

    public void mousePressed(MouseEvent e) {
        int x,y;

        x = tg.calculeColonne(e.getX());
        y = tg.calculeLigne(e.getY());
        if (m.action(y, x)) {
            // Exemple d'utilisation du statut d'une case : plus on passe par
            // une case, plus celle-ci est foncée.
            //tg.setStatut(tg.getStatut(y,x).darker(),y,x);
            f.tracerSansDelai(tg);
            
        }
    }

    // Il faut aussi une implementation pour les autres methodes de l'interface
    public void mouseEntered(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}
    public void mouseClicked(MouseEvent e) {}
    public void mouseReleased(MouseEvent e) {}
}
