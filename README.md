# One Crooked Editor

Crooked Editor is an educational project, and its designed to embrace breaking things. No AI.

looking for [documentation](https://github.com/Ethan-Heimer/One-Crooked-Editor/wiki)?

# Updates!
## 08/5/2026 - multithreading
v0.007 adds multithreading, runtime diagnostics, and memory leak detection. This update, while not adding new editor features,
will be the foundation to a bigger rendering update.

Crooked Editor is now also multithreaded! main.cpp got even more messy but 
hopefully v0.008 will clean it up. Crooked editor contains an IO thread and an Editor thread. Ncurses is not a thread safe library, 
and becuse im using it for IO, instead of a deticated rendering thread i have to use a general IO thread. we'll see if this holds up.

Crooked Editor now is built with gcc's address sanitizer to detect memory leaks when built in debug mode. the `build.sh` script will
build Crooked Editor in debug mode, `install.sh` will not.

## 08/4/2026 - undo/redo
I pushed all of the updates into v0.006... whoops. thats alright as v0.006 was very broken to begin with
BUT, Crooked editor has never worked better! v0.006(.2, i guess) Introduces Undo and Redo. But before I talk about
that, I want to hightlight what 'libeditor' is, keywords found inside the library.

### Libeditor concepts
One Crooked Editor uses libeditor to better handle SOC. libeditor provides the general concept behind what a *Model* 
editor should do, and gives the client interfaces and contracts to implement apon that. libeditor does not provide default
implementations for its concepts as it shouldn't do the clients job, it should just tell the client how
to get the job done. libeditor contains the following concepts for the client to expand apon:

 - states [modes]
 - actions
 - mutators
 - editables [files, documents, buffers...]

States are simple, they are simply the possable modes that the editor can be in. Crooked Editor implements a 'Normal' mode and a Insert Mode.
Actions are instructions that execute on key presses. These usually take the form of lambdas but if an object overrides the () operator it can
be an action. libeditor defines a tree structure for each state that handles executing keyboard shortcuts.
'Mutators' is the concept that text can only be edited through some object that understand how to do an initial mutation, how to
undo that mutation, and how to redo a mutation. this ensures that every edit is undoable, as well as customizable. All mutations are tracked
throughout the editors uptime, and can be undid and redid. editable materials are immutable outside of mutators. 
editables are more self explanitary, these files, buffers, whatever that the editor can edit. The client implements possable editables,
and provieds the implementation on how to load and save an editable, and how to edit and navigate it.

### features

anyway, new features:

 - u to undo in normal mode
 - r to redo in normal mode

undo undoes the entire edit made in insert mode.

### removed features
 - o, i just have to add a mutator for this and itll come back eventually.

### whats in the future?
v0.007 will focus on miltithreaded rendering.

## 07/28/2026 - libeditor 2 and breaking things
v0.006 is yet another archetectural update, but i finally feel like its at a place i like. v0.007 will be a feature update.
v0.006 also breaks the rendering loop and things to not look as good as they did in v0.005. use v0.005 if you actually would
like to edit text with this editor for now :).

## 07/15/2026 - libeditor
v0.005 is a huge archetectural update, and lays the foundations for undo/redo and complex keybindings.
I've intrduced a new internal library called **libeditor** in this version. libeditors job is to provide an 
abstraction and interface over text editing, and will be improved in future versions. Long term, I would like libeditor
to be designed well enough to where it could be used as a general purpose text editing library so other text 
editors can build off of it as well (this is good library design anyway so...). Currently, libeditor is NOT that, so 
v0.006 will hopefully improve it. As libeditor will change a lot from this update to v0.006, I'll save the new features it brings
for that update log.

## New Commands
[Normal Mode]
 - o to insert a new line and go into insert mode in one command
 - :w to save
 - :q to quit
 - :wq to save and quit

you might notice that im emulating vim keybings- this will not be perminate. I plan to embed a scripting language to allow for 
user configable keybindings.

## 06/23/2026 - Dependency Injection
v0.004 Doesn't offer any new features, it instead focuses on archetecture changes. 
v0.003 Had some underlying issues with how relationships between objects were represented,
and contained circular dependencies in key parts of the program. I want to implement the 
command pattern in v0.005, and before I can do that I needed a better feel of how objects will get
their depenancies. 

you'll notice a lot of factories in main.cpp. In the case where an object needs to create its own
dependancy and it cant be injected, a factory interface is provided.

The code isn't perfect and I can think of a few ways to Improve it during v0.005, but its good enough
to be able to implement the comman pattern for undo and redo operations.

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




