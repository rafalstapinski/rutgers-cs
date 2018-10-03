/*
 * Authors: Rafal Stapinski & Michael Tai
 * */

package application;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.stage.Stage;
import view.Controller;
import view.Song;
import javafx.scene.Scene;
import javafx.scene.layout.VBox;

public class SongLib extends Application {
	
	
	@Override
	public void start(Stage primaryStage) {
		try {
			FXMLLoader loader = new FXMLLoader();
			loader.setLocation(getClass().getResource("/view/songlib.fxml"));
			VBox root = (VBox)loader.load();
			Controller controller = loader.getController();
			controller.start(primaryStage);
			Scene scene = new Scene(root);
			primaryStage.setScene(scene);
			primaryStage.setTitle("Song Library");
			primaryStage.setResizable(false);  
			primaryStage.show();
		
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		launch(args);
	}
}
