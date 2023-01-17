# sample: `MSX2` (software) horizontal scroll demo

<https://user-images.githubusercontent.com/659805/212805495-c3de91a9-d855-4473-ad51-eb49aea389c0.mp4>

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
