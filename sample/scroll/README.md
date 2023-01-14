# sample: `MSX` (software) scroll demo

<https://user-images.githubusercontent.com/659805/212444487-58c41f8a-0115-487b-a6de-b1fccdc3b0fe.mp4>

`MSX` (TMS9918A VDP) has no hardware scrolling functionality.

So (software) smooth scrolling functionality is realized by combination of the
following two methods.

- Character pattern animation by rewriting the pattern generator table.
- 8-dot scroll by rewriting the pattern name table.
