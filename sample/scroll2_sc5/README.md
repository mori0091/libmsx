# sample: `MSX2` (software) horizontal scroll demo ; GRAPHIC 4 (SCREEN 5)

<https://user-images.githubusercontent.com/659805/213215110-3a01a382-d9e1-49c3-96b2-1eeb5c128697.mp4>

`MSX2` (V9938 VDP) has vertical scrolling functionality.
But it hasn't horizontal scrolling functionality.

So (software) smooth horizontal scrolling functionality is realized usually on
GRAPHIC 2 (SCREEN 2) or GRAPHIC 3 (SCREEN 4) mode.

But how about on GRAPHIC 4 (SCREEN 5) bitmap graphics mode ?

- 1-dot scroll
  - SET ADJUST (Display area adjust functionality)
- 8-dot scroll
  - By **switching** the pattern name table (double buffer), and
  - Copy image to the other incrementally by using VDP commands.
