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
enum Case {
    LIBRE(0), OBSTACLE(2), SAC(4), POUSSEUR(8), BUT(16), SAC_SUR_BUT(20),
    POUSSEUR_SUR_BUT(24), INVALIDE(32);

    int contenu;

    Case(int i) {
        contenu = i;
    }

    public boolean contient(Case c) {
        return (contenu & c.contenu) == c.contenu;
    }

    public boolean estLibre() {
        return (contenu == 0) || (contenu == 16);
    }

    public Case ajout(Case c) {
        switch (contenu | c.contenu) {
        case 4:
            return SAC;
        case 8:
            return POUSSEUR;
        case 20:
            return SAC_SUR_BUT;
        case 24:
            return POUSSEUR_SUR_BUT;
        default:
            throw new RuntimeException("Ajout de " + c + " sur " + this +
                      " impossible");
        }
    }

    public Case retrait(Case c) {
        switch (contenu & ~c.contenu) {
        case 0:
            return LIBRE;
        case 16:
            return BUT;
        default:
            throw new RuntimeException("Retrait de " + c + " de " + this +
                      " impossible");
        }
    }
}
