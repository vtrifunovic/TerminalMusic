# TerminalMusic
C version of Terminal Music that can be compiled into a binary

I suffered through using the default dbus api so this thing wouldn't have any dependencies.

## Controls:
 - passing `--color` flag renders album covers using terminal colors
 - passing `--big_text` flag renders text with .k9 fonts (Walmart figlet)
    - Will change this later to allow for font changing
 - **config.cfg** file determines what keybinds to use. If deleted will be recreated

## External Tools
**fig2k9.py** is a python script that converts figlet fonts (.flf) into my fonts (.k9). 
Found it easier to do it this way. 

## Fonts
k9 fonts are simple files where `$` indicate the start of a charachter, next charachter is what the charachter is (this isn't necessary mainly for debugging). 
The charachter is then created (can't use `$` in it), and `$$` indicates that the charachter has been completed. Charchters should be defined in their ascii order
starting with space. Example of defining the 'a' charachter:

```
$a
    ___
   /   \
  / / \ \
 / /---\ \
/_/     \_\$$
```

Comments are defined with a `#` at the beginning of a line and end when the newline character is hit.
