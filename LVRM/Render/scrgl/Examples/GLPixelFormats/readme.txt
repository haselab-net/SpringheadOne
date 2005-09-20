PixelFormats - OpenGL demo program for BCB3
---------------------------------------------
by Lucian Wischik. www.wischik.com/lu/programmer.html#opengl


This program enumerates pixel formats and lets you switch between
them dynamically. In the list, the abbreviations are as follows:
  MCD - a Mini Client Driver. (hardware acceleration, usually NT)
  ICD - an Installable Client Driver. (hardware acceleration)
  software - uses Microsoft's software renderer
  W - this pixel format can render to a window
  B - to a bitmap
  D - doublebuffered
  pal - works in paletized mode


Notes:

To get the number of pixel formats available we set up a valid
PFD and call DescribePixelFormat(1). If you tried it with (0,NULL)
then it would crash on 3dfx.

We do a rough heuristic to figure out which pixel-format to suggest
to the user. That's what the 'qual' stuff in btnListClick is doing.

Attempting to do OpenGL stuff using the Canvas->Handle of the form
itself appears not to work. That's why we use Frame->Handle
(Frame is a TPanel) for enumming pixel formats, and render->Handle
for the rendering. (render is a dynamically created TPanel).
Although the TPanel::Canvas property is hidden, we can still
get the TPanel::Handle property and call the API GetDC(.) on it.

Just for fun, if you click on the rendering panel then it will
create a mini panel there. That's so you can witness controls
being superimposed on a GL panel. Left click on them to get rid
of them.

It is not possible to set the pixel format of the same HDC to
several different things: it can be set once only, and that's it.
Therefore, when the user clicks on a pixel-format, we first
destroy the existing 'render' panel. Then we create a new one,
and because it is fresh it's okay to set its pixel format.

