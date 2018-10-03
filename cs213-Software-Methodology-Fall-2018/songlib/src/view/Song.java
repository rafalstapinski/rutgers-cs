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

	@Override
	public int compareTo(Song o) {
		if (this.title.equals(o.title)) {
			return this.artist.compareTo(o.artist);
		}
		return this.title.compareTo(o.title);
	}

}
