# MP3-Tag-Reader
Implementing an application to extract tags from the given MP3 file using C functions.

#Overview:
The MP3 Tag Reader is a command-line C application that reads and displays MP3 (ID3) tag information from MP3 files. This software is desktop-based and not web-based, designed for individuals who wish to view and collect MP3 tag metadata such as title, artist, album, year, and genre. The project can be extended to include editing capabilities for modifying MP3 tag information.

#About ID3 Tags:
ID3 is a metadata container most often used with the MP3 audio file format. It allows information like the song title, artist, album, track number, and other details to be stored inside the MP3 file itself.
There are two main versions:

ID3v1: Found at the end of an MP3 file, occupying the last 128 bytes.

ID3v2: Found at the beginning of the MP3 file, more flexible but more complex to parse.

#ID3v1 Structure:

Signature: "TAG" (3 bytes)

Title: 30 bytes

Artist: 30 bytes

Album: 30 bytes

Year: 4 bytes

Comment: 30 bytes

Genre: 1 byte
If a field doesn’t use all allocated bytes, the remaining bytes are padded with binary 0.

#ID3v2 Structure:

Starts with "ID3x" (x = sub-version number: 2, 3, or 4)

Contains a tag header followed by one or more frames, each with its own header and content.

Typically appears at the beginning of the MP3 file.

#Features:
Detect and Read ID3 Version

Automatically detects whether the MP3 file uses ID3v1 or ID3v2 tags.

View Tag Details

Displays song title, artist, album, year, comment, and genre in a readable format.

Extra Feature – Save to CSV (New)

After displaying tag details, the program asks:
"Do you want to save the details to a CSV file? (y/n)"

If the user selects "yes," all details are stored in a .csv file in a structured format for future reference.

This feature allows building a collection of MP3 metadata easily.

Future Scope (Optional Extension)

Implement a tag editor to modify MP3 tag information directly from the command line.
# ID3v2 Frame header: 
 .ID3v2/file identifier “ID3” 
 
 .ID3v2 version $03 00 (2, 3 or 4) 
 
 .ID3v2 flags %abc00000 
 
 .ID3v2 size 4 * %0xxxxxxx (Total size of ID3 tag) 
# ID3v2.2 Frame header:
 .Frame ID $xx xx xx(Three characters) [e.g.: TAL Album / Movie / Show title]
 
 .TOA Original artist(s) / performer(s) 
 
 .Size $xx xx xx(Three characters)
# ID3v2.3 or ID3v2.4 Frame header:
 .Frame ID $xx xx xx xx (Four characters) [e.g.: TAL Album/Movie/Show title]
 
 .TIT2 Title / song name /content description 
 
 .Size $xx xx xx xx (Four Characters)
 
 .Flags $xx xx
# Sample Output



![mtr_2](https://github.com/user-attachments/assets/3874efaa-0645-4ecc-9139-abe85f3426ab)


![mtr_3](https://github.com/user-attachments/assets/10ef0fc0-b300-44a8-9212-0d3c7b80612d)


<img width="1197" height="522" alt="Screenshot 2025-08-13 220503" src="https://github.com/user-attachments/assets/c78867d0-8cde-4301-a327-8bc2472371f1" />

<img width="1390" height="436" alt="Screenshot 2025-08-13 220710" src="https://github.com/user-attachments/assets/6c55408f-819d-47da-bbf1-64e225de7ab2" />

