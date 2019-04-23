/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package downloader.ui;

import downloader.fc.Downloader;
import javafx.beans.value.ChangeListener;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import static javafx.application.Platform.runLater;
import javafx.beans.value.ObservableValue;
import javafx.scene.layout.VBox;

/**
 *
 * @author benaissm
 */
public class DownloadThread implements Runnable {
    
    BorderPane zone;
    Label nomDL;
    ProgressBar pb;
    Downloader d;
    Boolean paused;
    
    public DownloadThread(String uri, VBox vb) {
        
        zone = new BorderPane();

        nomDL = new Label(uri);
        pb = new ProgressBar();
        d = new Downloader(uri);
        
        paused = false;
        
        //--------------------- BOUTONS PAUSES/SUPPR ----------------------
        Button delete = new Button("X");
        Button pause = new Button("||");

        HBox zb = new HBox();
        zb.setSpacing(10);
        zb.setPadding(new Insets(0,10,0,0));
        zb.getChildren().addAll(pause, delete);
        
        //---------- MISE EN PLACE D'UNE BARRE DE TELECHARGEMENT -----------
        BorderPane truc = new BorderPane();
        truc.setCenter(pb);
        truc.setRight(zb);
        pb.prefWidthProperty().bind(truc.widthProperty().subtract(50)); //Permet d'adapter la longueur du ProgressBar à la taille de la fenêtre
        pb.setPadding(new Insets(5,10,5,10));
        
        
        // ------------------- MISE A JOUT DE LA BARRE ---------------------
        ChangeListener l2 = new ChangeListener<Double>() {
            @Override
            public void changed(ObservableValue<? extends Double> observable, Double oldProgress, Double newProgress) {
                
                //Nous avons 2 Runnables ici, un pour regarder les changements de données de Download (dans main.java) , l'autre ci dessous pour gérer la barre de téléchagement (setProgress)
                runLater(new Runnable() {
                        // C'est CE runnable qui permet vraiment de gérer la barre ET les données !
                            @Override
                            public void run() {
                                pb.setProgress(newProgress);
                            }
                 });
                
            }
        };
        
        d.progressProperty().addListener(l2);
        
        //-------------------------------------------------------------------
         //ACTION DU BOUTON "SUPPRIMER"
        delete.setOnAction(value ->  {
            d.stop();
            d.progressProperty().removeListener(l2);
            vb.getChildren().remove(zone);
            Thread.currentThread().interrupt();
        });
        
        //ACTION DU BOUTON "PAUSE"
        pause.setOnAction(value ->  {
           if(paused == false){
               d.pause();
               pause.setText(">");
               paused = true;
           }
           else{
               d.play();
               pause.setText("||");
               paused = false;
           }
            
            
        });
        //------------------------------------------------------------------
        zone.setTop(nomDL);
        zone.setBottom(truc);
        
        
        
    }

    public BorderPane getNode() {
        return zone;
    }


    @Override
    public void run() {
        
        d.run();
             
    }
}
