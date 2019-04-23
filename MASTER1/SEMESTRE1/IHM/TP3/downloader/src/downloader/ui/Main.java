/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package downloader.ui;

/**
 *
 * @author benaissm
 */

import java.util.ArrayList;
import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.VBox;



public class Main extends Application {
 
    public void start(Stage stage) {
		
                BorderPane main = new BorderPane();
                
                //--------- AJOUT CHAMP D'AJOUT D'URL -----------
                BorderPane addDL = new BorderPane();
                
                TextField newDL = new TextField("http://iihm.imag.fr/index.html");
                Button add = new Button("Add");

                addDL.setCenter(newDL);
                addDL.setRight(add);
                
                //--------- AJOUT ZONE CHARGEMENT -----------
                ScrollPane root = new ScrollPane();
                root.setFitToHeight(true);
                root.setFitToWidth(true);
                
                VBox vb = new VBox();

		root.setContent(vb);
		
		stage.setTitle("downloader");
                stage.setHeight(250);
                stage.setWidth(500);
                
                main.setCenter(root);
                main.setBottom(addDL);
                Scene s = new Scene(main);

                //--------- AJOUT BARRE CHARGEMENT -----------
                add.setOnAction(value ->  {
                    DownloadThread t = new DownloadThread(newDL.getText(), vb);
                    vb.getChildren().addAll(t.getNode());
                    Thread p = new Thread(t, "ThreadDL"); //va lancer le runnable principal de DownloadThread (= t1.run(), qui va lancer le petit runnable du progressbar)
                    p.start();
                });
                
                stage.setScene(s);
		stage.show();

    }
        
	
    public static void main(String[] args) {
		launch(args);
    }
}