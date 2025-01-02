/******************************************************************\
 * SplitPanel - Simple backend implementation of dynamic split
 *   based panel system in c.
 *
 * Source:
 *  - https://github.com/Kirandeep-Singh-Khehra/SimpleSplitPanelSystem
 *
 * Authors:
 *  - Kirandeep Singh (@Kirandeep-Singh-Khehra)
 *
\******************************************************************/

#ifndef __K_SPLIT_PANEL_H_
#define __K_SPLIT_PANEL_H_
#include <stdlib.h>

#define new_(x) (x*)malloc(sizeof(x))

/****** DATA_STRUCTURES ******/

struct Panel;
typedef void (*PanelCallback)(struct Panel panel, void *data, ...);

/* 2D point implementation */
typedef struct Point {
  float *x;
  float *y;
} Point;

typedef struct Panel {
  Point origin;
  Point end;

  void *data;
  PanelCallback *callbacks;
  int numOfCallbacks;
} Panel;

typedef enum PanelSplitType {
  PANEL_SPLIT_TYPE_VERTICAL,
  PANEL_SPLIT_TYPE_HORIZONTAL,
} PanelSplitType;

/****** FUNCTION_DECALRATIONS ******/

/*
Creates a point.

Input:
  `float x`: abscissa value of point.
  `float y`: ordinate value of point.

Output:
  Returns a `Point`.
*/
Point CreatePoint(float x, float y) {
  Point p;
  p.x = new_(float);
  p.y = new_(float);

  *p.x = x;
  *p.y = y;

  return p;
}

/*
Creates a panel from given points.

Input:
  `Point origin`: Coordinates of top left corner of panel.
  `Point end`: Coordinates of bottom right corner of panel.

Output:
  Returns an instance of `struct panel`.
*/
Panel CreatePanel(Point origin, Point end);

/*
Splits a `Panel` vertically or horizontally and return new `Panel` formed.
New panel is formed on right of given panel for horizontal split and on
 bottom for given panel for vertical split.

Input:
  `Panel *panel`: pointer to Panel to split.
  `enum PanelSplitType splitType`: Direction of split.
      Choose from `PANEL_SPLIT_TYPE_VERTICAL` and `PANEL_SPLIT_TYPE_HORIZONTAL`

Output:
  Returns an instance of `struct Panel`.
*/
Panel SplitPanel(Panel *panel, PanelSplitType splitType);

/****** IMPLEMENTATION ******/

Panel CreatePanel(Point origin, Point end) {
  Panel panel = {0};

  panel.origin.x = new_(float);
  panel.origin.y = new_(float);

  *panel.origin.x = *origin.x;
  *panel.origin.y = *origin.y;

  panel.end.x = new_(float);
  panel.end.y = new_(float);

  *panel.end.x = *end.x;
  *panel.end.y = *end.y;

  panel.callbacks = NULL;
  panel.numOfCallbacks = 0;

  return panel;
}

Panel SplitPanel(Panel *panel, PanelSplitType splitType) {
  if (!panel || !panel->origin.x || !panel->end.x || !panel->end.y
  || (splitType != PANEL_SPLIT_TYPE_VERTICAL && splitType != PANEL_SPLIT_TYPE_HORIZONTAL)) {
    return (Panel){0};
  }

  Panel newPanel = {0};
  newPanel.origin.x = new_(float);
  newPanel.origin.y = new_(float);

  newPanel.end.x = panel->end.x;
  newPanel.end.y = panel->end.y;

  panel->end.x = new_(float);
  panel->end.y = new_(float);

  switch(splitType) {
    case PANEL_SPLIT_TYPE_HORIZONTAL:
      panel->end.x = newPanel.end.x;
      panel->end.y = new_(float);
      *panel->end.y = (*panel->origin.y + *newPanel.end.y) / 2.0f;

      newPanel.origin.x = panel->origin.x;
      newPanel.origin.y = panel->end.y;
      break;
    case PANEL_SPLIT_TYPE_VERTICAL:
      panel->end.y = newPanel.end.y;
      panel->end.x = new_(float);
      *panel->end.x = (*panel->origin.x + *newPanel.end.x) / 2.0f;;

      newPanel.origin.y = panel->origin.y;
      newPanel.origin.x = panel->end.x;
      break;
  }

  newPanel.callbacks = NULL;
  newPanel.numOfCallbacks = 0;

  return newPanel;
}

void AddPanelCallback(Panel *panel, void (*callback)(Panel, void *, ...)) {
    if (panel->callbacks == NULL) {
      panel->callbacks = (PanelCallback*)malloc(sizeof(PanelCallback));
      panel->numOfCallbacks = 1;
    } else {
      panel->callbacks = (PanelCallback*)realloc(panel->callbacks, (panel->numOfCallbacks + 1) * sizeof(void (*)(Panel, void *, ...)));
    }
    if (panel->callbacks == NULL) {
        exit(1);
    }
    panel->callbacks[panel->numOfCallbacks++] = callback;
}

#endif

