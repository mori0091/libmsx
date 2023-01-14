# sample: `MSX` (software) scroll demo

`MSX` (TMS9918A VDP) has no hardware scrolling functionality.

So (software) smooth scrolling functionality is realized by combination of the
following two methods.

- Character pattern animation by rewriting the pattern generator table.
- 8-dot scroll by rewriting the pattern name table.
