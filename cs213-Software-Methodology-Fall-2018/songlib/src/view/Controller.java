package view;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.scene.input.MouseEvent;
import javafx.scene.text.Text;
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
	boolean creatingSong = false;
	ObservableList<String> obsList = FXCollections.observableArrayList();
	
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
	Button delete_song;
	@FXML
	ListView<String> song_list;
	@FXML
	Text warning_text;
	
	public void start(Stage mainStage) throws IOException {
		this.loadSongs();
		
		if (this.songs.size() > 0) {
			this.refreshSongs();
			this.song_list.setItems(obsList);
			this.song_list.getSelectionModel().select(0);
			this.selectSong();
		} else {
			this.selected_title_field.setDisable(true);
			this.selected_artist_field.setDisable(true);
			this.selected_album_field.setDisable(true);
			this.selected_year_field.setDisable(true);
		}
	}
	
	
	public void selectSong() {
		
		this.warning_text.setText("");
		
		if (this.creatingSong) {
			this.creatingSong = false;
			this.songs.remove(0);
		}
		
		try {
			this.selectedSongIndex = this.song_list.getSelectionModel().getSelectedIndex();
			this.displaySongInfo();
		} catch (ArrayIndexOutOfBoundsException e) {
			return;
		}
		
	}
	
	public void displaySongInfo() {
		Song selectedSong = this.songs.get(this.selectedSongIndex);
		this.selected_title_field.setText(selectedSong.title);
		this.selected_artist_field.setText(selectedSong.artist);
		this.selected_album_field.setText(selectedSong.album);
		this.selected_year_field.setText(selectedSong.year);
	}
	
	public void createSong(ActionEvent a) {
		
		this.selected_title_field.setDisable(false);
		this.selected_artist_field.setDisable(false);
		this.selected_album_field.setDisable(false);
		this.selected_year_field.setDisable(false);
		this.save_song.setDisable(false);
		this.delete_song.setDisable(false);
		
		if (!this.creatingSong) {
			this.warning_text.setText("");
			this.songs.add(0, new Song());
		}
		
		this.song_list.getSelectionModel().clearSelection();
		this.creatingSong = true;
		this.selectedSongIndex = 0;
		this.selected_title_field.setText("");
		this.selected_artist_field.setText("");
		this.selected_album_field.setText("");
		this.selected_year_field.setText("");
		
		
	}
	
	public void deleteSong(ActionEvent a) {
		
		this.songs.remove(this.selectedSongIndex);
		this.refreshSongs();
		
		this.creatingSong = false;
		
		this.selectedSongIndex = this.selectedSongIndex == 0 ? 0 : this.selectedSongIndex - 1;
		this.song_list.getSelectionModel().select(this.selectedSongIndex);
		this.selectSong();
		
		if (this.songs.size() == 0) {
			this.selected_title_field.clear();
			this.selected_artist_field.clear();
			this.selected_album_field.clear();
			this.selected_year_field.clear();
			this.selected_title_field.setDisable(true);
			this.selected_artist_field.setDisable(true);
			this.selected_album_field.setDisable(true);
			this.selected_year_field.setDisable(true);
			this.save_song.setDisable(true);
			this.delete_song.setDisable(true);
		}
	}
	
	public void saveSong(ActionEvent a) {
		
		this.warning_text.setText("");
				
		String title = this.selected_title_field.getText();
		String artist = this.selected_artist_field.getText();
		String album = this.selected_album_field.getText();
		String year = this.selected_year_field.getText();
						
		Song selectedSong = this.songs.get(this.selectedSongIndex);
		
		if (title.equals("") || artist.equals("")) {
			this.warning_text.setText("Can't save this song. Title and artist mussed be filled in!");
			return;
		}
		
		for (Song song : this.songs) {
			if (selectedSong.equals(song)) {
				continue;
			} else if (title.toLowerCase().equals(song.title.toLowerCase()) && artist.toLowerCase().equals(song.artist.toLowerCase())) {
				this.warning_text.setText("Can't save this song. A song by this artist already exists!");
				return;
			}
		}
		
		this.songs.remove(this.selectedSongIndex);
		Song newSong = new Song(title, artist, album, year);
		
		this.songs.add(newSong);
		this.refreshSongs();
		this.selectedSongIndex = this.songs.indexOf(newSong);
		this.song_list.getSelectionModel().select(this.selectedSongIndex);
		this.displaySongInfo();
		this.creatingSong = false;
		
	}

	
	public void refreshSongs() {
		Collections.sort(this.songs);
				
		ArrayList<String> songs = new ArrayList<String>();
		
		for (Song song : this.songs) {
			songs.add(song.title + " by " + song.artist);
		}
		this.obsList.setAll(songs);
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
