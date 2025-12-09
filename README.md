# 🎵 mp3-tag-editor — Tiny ID3 Tag Wizard ✨

Give your MP3s a makeover in seconds — view and edit ID3 tags from the terminal. Fast, simple, and just a few bytes away from perfection. 🚀

Why you'll love it
- Lightweight C program — tiny footprint, no dependencies. 🧩
- View common ID3v2 frames (Title, Artist, Album, Year, Genre, Composer, Comments). 🔎
- Edit tags in-place safely via a temp file workflow. 🔁
- Great for batch scripts or quick fixes. ⚡

Quick Start — Build & Run
1. Build:
   ```
   make
   ```
2. View tags:
   ```
   ./mp3 -v song.mp3
   ```
3. Edit a tag:
   ```
   ./mp3 -e song.mp3 -t "New Song Title"
   ```

Edit options
- -t : Title 📝
- -a : Album 💿
- -r : Artist 🎤
- -g : Genre 🎚️
- -y : Year 📅
- -c : Composer 🎼

Examples
- View:
  ```
  ./mp3 -v my_favorite_track.mp3
  ```
- Change title:
  ```
  ./mp3 -e my_favorite_track.mp3 -t "Sunset Drive"
  ```

Safety & Notes
- The program writes edits to a temporary file `temp.mp3` and replaces the original. Always keep a backup of important files. ⚠️💾
- Supports ID3v2.3 and ID3v2.4 frame size formats.
- Not a full ID3 library — designed for common tags and learning purposes. 🛠️

Contributing
- Found a bug or want a new feature? Open an issue or submit a PR. Small improvements and tests are welcome. 🤝

Enjoy tweaking your tags! If you'd like, I can:
- Add a quick shell script for batch edits,
- Create a small test MP3 with sample tags,
- Or add a prettier CLI help screen.

Happy tagging! 🎧🎶
