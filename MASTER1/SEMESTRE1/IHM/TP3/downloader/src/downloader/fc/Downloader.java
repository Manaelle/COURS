package downloader.fc;

import java.net.URL;
import java.net.URLConnection;
import java.net.MalformedURLException;

import java.io.File;
import java.io.BufferedInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import javafx.beans.property.ReadOnlyDoubleWrapper;
import javafx.beans.property.ReadOnlyDoubleProperty;


public class Downloader implements Runnable {
	public static final int CHUNK_SIZE = 1024;

	URL url;
	int content_length;
	BufferedInputStream in;
	
	String filename;
	File temp;
	FileOutputStream out;
	
	ReadOnlyDoubleWrapper progress = new ReadOnlyDoubleWrapper(this, "progress", 0);

	int size = 0;
	int count = 0;
        
        //AJOUTE : pour gérer pause/play
	private volatile boolean running = true;
        private volatile boolean paused = false;
        
        private final Object pauseLock = new Object();
        //-------------------------------------------------------------------
        //Création d'un thread de téléchargement, afin de DL une page web dont l'url est uri
        //cf socket pour comprendre le fonctionnement
	public Downloader(String uri) {
		try {
			url = new URL(uri); //URL() permet de récupérer pleins de données relatives à l'url donné (port, protocole... et le fichier .html !)
			
			URLConnection connection = url.openConnection(); //ouverture d'une connection avec l'url (ouverture de socket)
			content_length = connection.getContentLength();
			
			in = new BufferedInputStream(connection.getInputStream()); //buffer d'émission
			
			String[] path = url.getFile().split("/"); 
			filename = path[path.length-1]; //récupération du nom du fichier dans le path
			temp = File.createTempFile(filename, ".part"); //permet de créer un fichier temporaire, qui deviendra un fichier final après DL
			out = new FileOutputStream(temp); //buffer de réception
		}
		catch(MalformedURLException e) { throw new RuntimeException(e); }
		catch(IOException e) { throw new RuntimeException(e); }
	}
        
        //-------------------------------------------------------------------
	public String toString() {
		return url.toString();
	}
	
        //-------------------------------------------------------------------
        //lecture de la progression du téléchargement
	public ReadOnlyDoubleProperty progressProperty() {
		return progress.getReadOnlyProperty();
	}
	
        //-------------------------------------------------------------------
        //démarrage du téléchargement
	protected String download() throws InterruptedException {
		byte buffer[] = new byte[CHUNK_SIZE];
		
                //----  GESTION PAUSE ----------
                while (running && count >= 0){
                    synchronized (pauseLock) {
                        if (!running) { 
                            break;
                        }
                        if (paused) {
                            try {
                                pauseLock.wait(); 
                            } catch (InterruptedException ex) {
                                 System.err.println("ERREUR ! Problème de mise en attente du thread.");
                            }
                        }
                        if (!running) { 
                            break;
                        }
                    }
                    //-------------------------------
                            try {
                                out.write(buffer, 0, count); //on écrit dans le buffer de réception OUT la sortie de socket (count = le nombre d'octets à écrire)
                            }
                            catch(IOException e) { continue; }

                            size += count; //nombre d'octets écrits
                            progress.setValue(1.*size/content_length);
                            Thread.sleep(1000); //permet de temporiser

                            try {
                                    count = in.read(buffer, 0, CHUNK_SIZE); //lecture du fichier url (CHUNK_SIZE octets seront lus = taille du buffer)
                            }
                            catch(IOException e) { continue; }
                    }
                
                
                    //---------------------
                    //Cas où à la fin, le fichier reste incomplet
                    if(size < content_length) {
                            temp.delete();
                            System.err.println("Téléchargement interrompu !");
                            return filename;
                    }

                    temp.renameTo(new File(filename)); //fin de DL, on a notre fichier téléchargé !  
                    return filename;
	}
        
        //-------------------------------------------------------------------
	public void run() {
		try {
			download();
		}
		catch(InterruptedException e) {
			throw new RuntimeException(e);
		}
	}
        
        //-------------------------------------------------------------------
        public void pause(){
            paused = true;
            
        }
        
        public void play(){
            synchronized (pauseLock) {
                paused = false;
                pauseLock.notifyAll();
            }
        }
        
        public void stop() {
        running = false;
    }
            
        
};
