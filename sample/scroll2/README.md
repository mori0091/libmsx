# sample: `MSX2` (software) horizontal scroll demo

`MSX2` (V9938 VDP) has vertical scrolling functionality.
But it hasn't horizontal scrolling functionality.

So (software) smooth horizontal scrolling functionality is realized by
combination of the following methods.

- 1-dot scroll (one of the following)
  - Character pattern animation by **rewriting** the pattern generator table,
  - Character pattern animation by **switching** the pattern generator table,
  - SET ADJUST (Display area adjust functionality)
- 8-dot scroll (one of the following)
  - by **rewriting** the pattern name table.
  - by **switching** the pattern name table. (double buffer)
