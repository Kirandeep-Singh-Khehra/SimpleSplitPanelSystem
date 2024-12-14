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
typedef struct Point {
  float *x;
  float *y;
} Point;

Point CreatePoint(float x, float y) {
  Point p;
  p.x = new_(float);
  p.y = new_(float);

  *p.x = x;
  *p.y = y;

  return p;
}

typedef struct Panel {
  Point *origin;
  Point *end;

  void (*panelContent)(void *);
} Panel;

typedef enum PanelSplitType {
  PANEL_SPLIT_TYPE_NONE,
  PANEL_SPLIT_TYPE_VERTICAL,
  PANEL_SPLIT_TYPE_HORIZONTAL,

  PANEL_SPLIT_TYPE_COUNT
} PanelSplitType;

/****** FUNCTION_DECALRATIONS ******/

/*
Creates a panel from given points.

Input:
  Point origin: Coordinates of top left corner of panel.
  Point end: Coordinates of bottom right corner of panel.

Output:
  Returns an instance of struct panel.
*/
Panel CreatePanel(Point origin, Point end);

/*
Splits a `Panel` vertically or horizontally and return new `Panel` formed.
New panel is formed on right of given panel for horizontal split and on
 bottom for given panel for vertical split.

Input:
  Panel *panel: pointer to Panel to split.
  enum PanelSplitType splitType: Direction of split.
      Choose from PANEL_SPLIT_TYPE_VERTICAL and PANEL_SPLIT_TYPE_HORIZONTAL

Output:
  Returns an instance of `struct Panel`.
*/
Panel* SplitPanel(Panel *panel, PanelSplitType splitType);

/****** IMPLEMENTATION ******/

Panel CreatePanel(Point origin, Point end) {
  Panel panel = {};
  panel.origin = new_(Point);
  *panel.origin = origin;
  panel.end = new_(Point);
  *panel.end = end;

  return panel;
}

Panel* SplitPanel(Panel *panel, PanelSplitType splitType) {
  if (!panel || (splitType != PANEL_SPLIT_TYPE_VERTICAL && splitType != PANEL_SPLIT_TYPE_HORIZONTAL)) {
    return NULL;
  }

  Panel *newPanel = new_(Panel);
  newPanel->origin = new_(Point);
  newPanel->end = new_(Point);
  *newPanel->origin = CreatePoint(0.0f, 0.0f);
  *newPanel->end = CreatePoint(0.0f, 0.0f);

  newPanel->end = panel->end;
  panel->end = new_(Point);
  *panel->end = CreatePoint(0.0f, 0.0f);

  switch(splitType) {
    case PANEL_SPLIT_TYPE_HORIZONTAL:
      panel->end->x = newPanel->end->x;
      panel->end->y = new_(float);
      *panel->end->y = (*panel->origin->y + *newPanel->end->y) / 2.0f;

      newPanel->origin->x = panel->origin->x;
      newPanel->origin->y = panel->end->y;
      break;
    case PANEL_SPLIT_TYPE_VERTICAL:
      panel->end->y = newPanel->end->y;
      panel->end->x = new_(float);
      *panel->end->x = (*panel->origin->x + *newPanel->end->x) / 2.0f;;

      newPanel->origin->y = panel->origin->y;
      newPanel->origin->x = panel->end->x;
      break;
  }

  return newPanel;
}

#endif

