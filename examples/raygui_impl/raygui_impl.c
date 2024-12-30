#include <raylib.h>

#include "../../split_panel.h"
#include "raygui_panel_utils.c"
#include "raygui_boilerplate.h"

#define MAX_PANEL_COUNT 10

Panel panels[MAX_PANEL_COUNT];
int panel_count = 0;

Point windowOrigin;
Point windowEnd;

typedef enum PanelState {
  PANEL_STATE_IDLE,
  PANEL_STATE_READY_TO_SPLIT,
  PANEL_STATE_DECIDING_SPLIT,
  PANEL_STATE_READY_TO_RESIZE,
  PANEL_STATE_MOVING
} PanelState;

PanelState focussedPanelState = PANEL_STATE_IDLE;
int focussedPanelIndex = -1;
Point focussedPoint = {0};
PanelSplitType currentSplitType = -1;

void SimplePanelCallBack(Panel panel, void *data, ...) {
  Rectangle scissor = GetPanelRectangle(panel);

  GuiButton((Rectangle){ *panel.origin.x + 10, *panel.origin.y + 10, 120, 30 }, "#191#Sample Button");
}

void OnStart() {
  windowOrigin = CreatePoint(0.0f, 0.0f);
  windowEnd = CreatePoint(WINDOW_WIDTH, WINDOW_HEIGHT);
  panels[0] = CreatePanel(windowOrigin, windowEnd);
  panels[0].callback = SimplePanelCallBack;
  panel_count = 1;
}

void OnUpdate() {
  for (int i = 0; i < panel_count; i++) {
    UpdatePanel(panels[i]);

    if (IsPanelHovered(panels[i]) &&
        focussedPanelState != PANEL_STATE_DECIDING_SPLIT &&
        focussedPanelState != PANEL_STATE_MOVING) {
      focussedPanelIndex = i;
    }
  }

  switch (focussedPanelState) {
  case PANEL_STATE_IDLE:
    if (IsPanelAnchorHovered(panels[focussedPanelIndex])) {
      focussedPanelState = PANEL_STATE_READY_TO_SPLIT;
    } else if (IsPanelReadyToResize(panels[focussedPanelIndex])) {
      focussedPanelState = PANEL_STATE_READY_TO_RESIZE;
    }
    break;
  case PANEL_STATE_READY_TO_SPLIT:
    if (!IsPanelAnchorHovered(panels[focussedPanelIndex])) {
      focussedPanelState = PANEL_STATE_IDLE;
    } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      focussedPanelState = PANEL_STATE_DECIDING_SPLIT;
    }
    break;
  case PANEL_STATE_DECIDING_SPLIT:
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !IsPanelAnchorHovered(panels[focussedPanelIndex])) {
      if ((GetMouseY() - *panels[focussedPanelIndex].origin.y) < (*panels[focussedPanelIndex].end.x - GetMouseX())) {
        focussedPanelState = PANEL_STATE_MOVING;
        currentSplitType = PANEL_SPLIT_TYPE_VERTICAL;
        panels[panel_count ++] = SplitPanel(&panels[focussedPanelIndex], currentSplitType);
        panels[panel_count - 1].callback = SimplePanelCallBack;
        focussedPoint.x = panels[focussedPanelIndex].end.x;
        focussedPoint.y = NULL;
      } else {
        focussedPanelState = PANEL_STATE_MOVING;
        currentSplitType = PANEL_SPLIT_TYPE_HORIZONTAL;
        panels[panel_count ++] = SplitPanel(&panels[focussedPanelIndex], currentSplitType);
        panels[panel_count - 1].callback = SimplePanelCallBack;
        focussedPoint.x = NULL;
        focussedPoint.y = panels[panel_count - 1].origin.y;
      }
    } else if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      focussedPanelState = PANEL_STATE_IDLE;
    }
    break;
  case PANEL_STATE_READY_TO_RESIZE:
    if(IsPanelAnchorHovered(panels[focussedPanelIndex])) {
      focussedPanelState = PANEL_STATE_READY_TO_SPLIT;
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    } else if (IsPanelReadyToResize(panels[focussedPanelIndex])) {
      switch(GetClosestEdgeToPointer(panels[focussedPanelIndex])) {
        case PANEL_EDGE_TOP:
          focussedPoint.x = NULL;
          focussedPoint.y = panels[focussedPanelIndex].origin.y;
          SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
          break;
        case PANEL_EDGE_BOTTOM:
          focussedPoint.x = NULL;
          focussedPoint.y = panels[focussedPanelIndex].end.y;
          SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
          break;
        case PANEL_EDGE_LEFT:
          focussedPoint.x = panels[focussedPanelIndex].origin.x;
          focussedPoint.y = NULL;
          SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
          break;
        case PANEL_EDGE_RIGHT:
          focussedPoint.x = panels[focussedPanelIndex].end.x;
          focussedPoint.y = NULL;
          SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
          break;
      }
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        focussedPanelState = PANEL_STATE_MOVING;
      }
    } else {
      focussedPanelState = PANEL_STATE_IDLE;
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    break;
  case PANEL_STATE_MOVING:

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (focussedPoint.x) *focussedPoint.x = GetMouseX();
      if (focussedPoint.y) *focussedPoint.y = GetMouseY();
    } else {
      focussedPanelState = PANEL_STATE_IDLE;
    }
    break;
  }
}

void OnDraw() {
  for (int i = 0; i < panel_count; i++) {
    DrawPanel(panels[i]);
  }
}

void OnEnd() {}

