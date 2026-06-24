# One Crooked Editor

Crooked Editor is an educational project, and its designed to embrace breaking things and iteration. 
From this commit forward- the code for this project will be written with this editor, including this readme! No AI.

looking for [documentation](https://github.com/Ethan-Heimer/One-Crooked-Editor/wiki)?

# Updates!
## 06/23/2026 - A Modal Editor

v0.003 Transforms Crooked Editor into a modal editor!

## New Modes:
 - Normal Mode
 - Insert Mode

Press I to enter *Insert* mode and ESC to enter *Normal Mode*

*Normal Mode* is the default mode, and like vim, you can use it to position the cursor around the editor.
*Insert Mode* allows you to edit text in the editor.

## New Commands [Normal Mode]
 - H, J, K, L to move the cursor around the editor
 - I to enter *Insert Mode*

## New Commands [Insert Mode]
 - ESC to enter *Normal Mode*
 - Tab inserts 4 spaces at the current position

## Known Bugs
 - Unrecongized keys can pollute the file with patterns like '^E'

## 06/14/2026 - Initial Changes!

v0.002 Fixes some basic visual bugs. aside form a case where the position of the cursor is outside
the bounds of the screen (which shoudn't happen), the cursor will not desync. 

Line numbers have been added, and to do so (as well as fix the desync bug), I've made the doubly-linked-list 
that represents the lines of text assign indexes to nodes. This may mean that this linked list structure is 
worse performing that just using a dynamic array (due to pointer chasing and arrays having better traversal. Insertion and deletion might be faster depending on CPU
optimizations too), however i'll worry about it if performance starts to tank.

Other bugs fixed to make the editor's 'ui' work better- as well as a slight performance tweak for rendering documents.

### New Commands
- `ctrl+w` - save (no longer quits)
- `ctrl+x` - quit

## 06/14/2026 - Initial Commit!

To get this project going, my goal was to develop a 'Minimal viable project' to that I could start iterating. 
At this stage, the editor works!... with very little features in place. 

Use the arrow keys to move around the editor.
Use ctrl+w to save.
Thats about all! 

A file - or document - at this stage is represented by a doubly-linked-list of gap-buffers to optimize for insertion operations.
Each line is its own node in this linked list, and is also its own gap buffer.

The goal for the next 'phase' of this project is to turn it into a modal editor.

### Known bugs
 - The cursor can 'desync' from its real position in the editor, meaning the visual cursor may be in one place but the physical place is in another. 
