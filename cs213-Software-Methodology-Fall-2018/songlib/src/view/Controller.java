package view;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;

public class Controller {
	
	ArrayList<Song> songs = new ArrayList<Song>();
	String songFilePath = "src/songs.csv";
	int selectedSongIndex = 0;
	
	@FXML
	TextField selected_title_field;
	@FXML
	TextField selected_artist_field;
	@FXML
	TextField selected_album_field;
	@FXML
	TextField selected_year_field;
	@FXML
	Button new_song;
	@FXML
	Button save_song;
	@FXML
	ListView<String> song_list;
	
	public void start(Stage mainStage) throws IOException {
		this.loadSongs();
		
		if (this.songs.size() > 0) {
			this.refreshSongs();
			this.song_list.getSelectionModel().select(0);
			this.selectSong(mainStage);
		}
		
		this.song_list.getSelectionModel().selectedIndexProperty().addListener((obs, oldVal, newVal) -> selectSong(mainStage));
	}
	
	public void selectSong(Stage mainStage) {

		this.selectedSongIndex = this.song_list.getSelectionModel().getSelectedIndex();
		Song selectedSong = this.songs.get(this.selectedSongIndex);
		
		this.selected_title_field.setText(selectedSong.title);
		this.selected_artist_field.setText(selectedSong.artist);
		this.selected_album_field.setText(selectedSong.album);
		this.selected_year_field.setText(selectedSong.year);

	}
	
	// call from click on new_song Button
	public void createSong() {
		// create temporary new song in this.songs at position 0
		// update this.selectedSongIndex to 0
	}
	
	// call from click on save_song Button
	public void saveSong() {
		// get selected_xxx_field values
		// set them to song at this.selectedSongIndex
		// call this.saveSongs
		// call this.refreshSongs
		
		// figure out if we should update selected to 0 or the new contents 
		// should be easy with this.songs.indexOf to get the new index
	}

	
	public void refreshSongs() {
		Collections.sort(this.songs);
				
		ArrayList<String> songs = new ArrayList<String>();
		
		for (Song song : this.songs) {
			songs.add(song.title + " by " + song.artist);
		}
		
		ObservableList<String> obsList = FXCollections.observableArrayList(songs);
		this.song_list.setItems(obsList);
	}
	
	public void loadSongs() throws IOException {
		
		String line;
		
		try (BufferedReader br = new BufferedReader(new FileReader(this.songFilePath))) {
			while ((line = br.readLine()) != null) {		
				String[] songInfo = line.split(",", -1);
				this.songs.add(new Song(songInfo[0], songInfo[1], songInfo[2], songInfo[3]));	
			}
		}
		
		Collections.sort(this.songs);
		
	}
	
	public void saveSongs() throws IOException {
		
		PrintWriter pw = new PrintWriter(this.songFilePath);
		pw.close();
		
		FileWriter fw = new FileWriter(this.songFilePath);
		
		for (Song song: this.songs) {
			fw.write(song.title + "," +  song.artist + "," +  song.album + "," + song.year + "\n");
		}
				
		fw.close();
	}
}
