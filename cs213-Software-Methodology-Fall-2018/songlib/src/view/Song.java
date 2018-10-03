package view;

public class Song implements Comparable<Song> {
	
	String title;
	String artist;
	String album;
	String year;
	
	public Song(String title, String artist, String album, String year) {
		
		this.title = title;
		this.artist = artist;
		this.album = album;
		this.year = year;
	}
	
	public Song() {
		this.title = "";
		this.artist = "";
		this.album = "";
		this.year = "";
	}

	@Override
	public int compareTo(Song o) {
		if (this.title.toLowerCase().equals(o.title.toLowerCase())) {
			return this.artist.toLowerCase().compareTo(o.artist.toLowerCase());
		}
		return this.title.toLowerCase().compareTo(o.title.toLowerCase());
	}
	
	@Override
	public String toString() {
		return this.title + " " + this.artist;
	}

}
