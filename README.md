# SimpleSplitPanelSystem
A single header file implementation of backend implementation of simple dynamic split based panel system in c.

## Features
1. Create a split based resizable panel system.
2. Panel attributes shares memory address for same properties. So, modify any panel and it will adjust others.

## Function docs
### `Panel CreatePanel(Point origin, Point end);`
Creates a panel from given points.

Input:
  `Point origin`: Coordinates of top left corner of panel.
  `Point end`: Coordinates of bottom right corner of panel.

Output:
  Returns an instance of `struct panel`.

### `Panel SplitPanel(Panel *panel, PanelSplitType splitType);`
Splits a `Panel` vertically or horizontally and return new `Panel` formed.
New panel is formed on right of given panel for horizontal split and on
 bottom for given panel for vertical split.

Input:
  `Panel *panel`: pointer to Panel to split.
  `enum PanelSplitType splitType`: Direction of split.
      Choose from PANEL_SPLIT_TYPE_VERTICAL and PANEL_SPLIT_TYPE_HORIZONTAL

Output:
  Returns an instance of `struct Panel`.

## Example usage
1. Include the `split_panel.h` file in your code.
2. Initialize the panel using folowing code.
```c
Point origin = CreatePoint(0.0f, 0.0f); // Top left corner
Point end = CreatePoint(100.0f, 100.0f); // Top right corner

// Create panel
Panel basePanel = CreatePanel(origin, end)
```
3. To Split the panel use `SplitPanel()` function.
```c
// Split panel vertically
Panel newPanel = SplitPanel(&basePanel, PANEL_SPLIT_TYPE_VERTICAL);

// Split panel horizontally
Panel newPanel = SplitPanel(&basePanel, PANEL_SPLIT_TYPE_HORIZONTAL);
```
4. To modify the split simply modify any point of any panel.
```c
// Modify origin's 'x' of 'basePanel'.
*basePanel.origin.x = 10.0f;

// Other panels also get the changes as common ordinates share address.
printf("%f", *newPanel.origin.x); // 10.0f
```

## [GUI simple example](https://github.com/Kirandeep-Singh-Khehra/SimpleSplitPanelSystem/blob/main/examples/raygui_impl/raygui_impl.c) using [raygui](https://github.com/raysan5/raygui)
<img src="/examples/raygui_impl/raygui_impl.png" alt="RayGUI Panel Demo"/>

## [GUI example better](https://github.com/Kirandeep-Singh-Khehra/SimpleSplitPanelSystem/blob/main/examples/raygui_impl/raygui_impl.c) using [raygui](https://github.com/raysan5/raygui)
<img src="/examples/raygui_impl_better/raygui_impl_better.png" alt="RayGUI Panel Demo"/>

## Roadmap
1. Remove as much pointer usage as possible.
2. Add ability to remove panel.
3. Add more descreptive split types. Ex: `SPLIT_LEFT`, `SPLIT_RIGHT`, `SPLIT_TOP` and `SPLIT_BOTTOM`.
4. Add Pool system to take load of memory managment from user.
5. Add docking(pulling out panel).

**Enjoy panelling**

