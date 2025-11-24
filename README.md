# Editor Sounds

Adds sound effects to the editor! 

## Custom Sound Packs

You can set a custom sound pack in the mod's settings.

### How To

To create a custom sound pack, you need to add sounds with the name of the event you want the sound to play on. These sounds can be `.ogg` or `.mp3` files.

You can also add random sound variants by adding `_var_#` at the end of the file name.

For example, if I want to have variance on object placement, I would name the files like this:

`place-object_var_1.ogg`

`place-object_var_2.ogg`

`place-object_var_3.ogg`

### Events

- `select` - Object is selected.

- `deselect` - Object is deselected

- `place-object` - Object is placed.

- `delete` - Object is deleted.

- `copy` - Object is copied.

- `paste` - Object is pasted.

- `duplicate` - Object is duplicated.

- `undo` - Action is undone.

- `redo` - Action is redone.

- `move` - Object is moved one grid space. Overridden if the following are set:

  - `move-down` - Object is moved down one grid space.

  - `move-up` - Object is moved up one grid space.

  - `move-left` - Object is moved left one grid space.

  - `move-right` - Object is moved right one grid space.

- `move-half` - Object is moved half a grid space. Overridden if the following are set:

  - `move-half-down` - Object is moved down half a grid space.

  - `move-half-up` - Object is moved up half a grid space.

  - `move-half-left` - Object is moved left half a grid space.

  - `move-half-right` - Object is moved right half a grid space.

- `move-small` - Object is moved 1/15th of a grid space. Overridden if the following are set:

  - `move-small-down` - Object is moved down 1/15th of a grid space.

  - `move-small-up` - Object is moved up 1/15th of a grid space.

  - `move-small-left` - Object is moved left 1/15th of a grid space.

  - `move-small-right` - Object is moved right 1/15th of a grid space.

- `move-tiny` - Object is moved 1/60th of a grid space. Overridden if the following are set:

  - `move-tiny-down` - Object is moved down 1/60th of a grid space.

  - `move-tiny-up` - Object is moved up 1/60th of a grid space.

  - `move-tiny-left` - Object is moved left 1/60th of a grid space.

  - `move-tiny-right` - Object is moved right 1/60th of a grid space.

- `move-big` - Object is moved five grid grid spaces. Overridden if the following are set:

  - `move-big-down` - Object is moved down five grid spaces.

  - `move-big-up` - Object is moved up five grid spaces.

  - `move-big-left` - Object is moved left five grid spaces.

  - `move-big-right` - Object is moved right five grid spaces.

- `rotate` - Object is rotated or flipped. Overridden if the following are set:

  - `rotate-clockwise` - Object is rotated 90 degrees clockwise.

  - `rotate-counter-clockwise` - Object is rotated 90 degrees counter clockwise.

  - `rotate-45-clockwise` - Object is rotated 45 degrees clockwise.

  - `rotate-45-counter-clockwise` - Object is rotated 45 degrees counter clockwise.

  - `flip-x` - Object is flipped horizontally.

  - `flip-y` - Object is flipped vertically.

- `generic-button` - Sound used for toolbar-categories and toolbar-toggles. Overridden if the following are set:

  - `toolbar-categories` - Clicking on the Create, Edit, Delete, and View buttons.

  - `toolbar-toggles` - Clicking on the Swipe, Rotate, Free Move, and Snap buttons.

- `prev-page` - Clicking on the previous page in the toolbar,

- `next-page` - Clicking on the next page in the toolbar.

- `base-layer` - Clicking the base layer arrow (uses prev-page by default).

- `next-free-layer` - Clicking the next free layer arrow (uses next-page by default).

- `prev-layer` Clicking the next layer arrow (uses prev-page by default).

- `next-layer` Clicking the next layer arrow (uses next-page by default).

- `tabs-prev-page` Clicking the prev page arrow if you have Vanilla Pages installed (uses prev-page by default).

- `tabs-next-page` Clicking the next page arrow if you have Vanilla Pages installed (uses next-page by default).

- `switch-tab` - Switching between tabs in the toolbar.

- `zoom-in` - Zooming in the editor.

- `zoom-out` - Zooming out the editor.

- `lock-layer` - Locking the layer you are on.

- `unlock-layer` - Unlocking the layer you are on.

- `link` - Linking a group of objects together.

- `unlink` - Unlinking a group of objects.

- `toggle-link` - Toggling link controls if you have Improved Link Controls installed.

- `slider-tick` - Sliding the slider at the top of the editor.

- `grid-decrease` - Decreasing the grid size with BetterEdit.

- `grid-increase` - Increasing the grid size with BetterEdit.

- `toggle-ui` - Toggling the UI with BetterEdit.