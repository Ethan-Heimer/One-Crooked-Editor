# One Crooked Editor

Crooked Editor is an educational project, and its designed to embrace breaking things and iteration. 
From this commit forward- the code for this project will be written with this editor, includeing this readme! No AI.

# Updates!
## 06/14/2026 - Initial Commit!

To get this project going, my goal was to develop a 'Minimal viable project' to that I could start iterating. 
At this stage, the editor works!... with very little features in place. 

Use the arrow keys to move around the editor.
Use ctrl+w to save.
Thats about all! 

A file - or document - at this stage is represented by a doubly-linked-list of gap-buffers to optimize for insertion operations.
Each line is its own node in this linked list, and is also its own gap buffer.

The goal for the next 'phase' of this project is to turn it into a modal editor.

## Known bugs
 - The cursor can 'desync' from its real position in the editor, meaning the visual cursor may be in one place but the physical place is in another. 
