#include "../../split_panel.h"
#include "raygui_panel_utils.c"
#include "raygui_boilerplate.h"
#include <raylib.h>

#define MAX_PANEL_COUNT 10
#define MAX_PANEL_VIEWS 10
#define MAX_PANEL_NAME_LENGTH 32

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

typedef enum PanelCallBackType {
  CALLBACK_START,
  CALLBACK_UPDATE,
  CALLBACK_DRAW,
  CALLBACK_END
} PanelCallBackType;

typedef struct ContextMenuData {
    int open;
    int scrollIndex;
    int active;
    int focus;
} ContextMenuData;

typedef struct PanelData {
    void *data;
    ContextMenuData menu;
} PanelData;

PanelState focussedPanelState = PANEL_STATE_IDLE;
int focussedPanelIndex = -1;
Point focussedPoint = {0};
PanelSplitType currentSplitType = -1;

typedef struct PanelView {
  char name[MAX_PANEL_NAME_LENGTH];
  void *data;
  PanelCallback *callbacks;
} PanelView;

PanelView panelViews[MAX_PANEL_VIEWS];
int panelViewCount;

void OnStart() {
  windowOrigin = CreatePoint(0.0f, 0.0f);
  windowEnd = CreatePoint(WINDOW_WIDTH, WINDOW_HEIGHT);
  panels[0] = CreatePanel(windowOrigin, windowEnd);
  panels[0].data = new_(PanelData);
  ((PanelData*)panels[0].data)->menu.open = false;
  ((PanelData*)panels[0].data)->menu.scrollIndex = 0;
  ((PanelData*)panels[0].data)->menu.active = 0;
  ((PanelData*)panels[0].data)->menu.focus = 0;

  ((PanelData*)panels[0].data)->data = panelViews[0].data;
  panels[0].callbacks = panelViews[0].callbacks;
  panels[0].numOfCallbacks = 4;

  PanelRunCallBack(panels[0], CALLBACK_START);

  panel_count = 1;

  windowOrigin = panels[0].origin;
  windowEnd = panels[0].end;
}

void OnUpdate() {
  *windowOrigin.x = 0.0f;
  *windowOrigin.y = 0.0f;

  *windowEnd.x = *windowOrigin.x + GetScreenWidth();
  *windowEnd.y = *windowOrigin.y + GetScreenHeight();

  for (int i = 0; i < panel_count; i++) {
    PanelRunCallBack(panels[i], CALLBACK_UPDATE);

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
        
        panels[panel_count - 1].data = new_(PanelData);
        ((PanelData*)panels[panel_count - 1].data)->menu = ((PanelData*)panels[focussedPanelIndex].data)->menu;
        ((PanelData*)panels[panel_count - 1].data)->data = ((PanelData*)panels[focussedPanelIndex].data)->data;

        panels[panel_count - 1].callbacks = panels[focussedPanelIndex].callbacks;
        panels[panel_count - 1].numOfCallbacks = 4;

        PanelRunCallBack(panels[panel_count - 1], CALLBACK_START);

        focussedPoint.x = panels[focussedPanelIndex].end.x;
        focussedPoint.y = NULL;
      } else {
        focussedPanelState = PANEL_STATE_MOVING;
        currentSplitType = PANEL_SPLIT_TYPE_HORIZONTAL;
        panels[panel_count ++] = SplitPanel(&panels[focussedPanelIndex], currentSplitType);

        panels[panel_count - 1].data = new_(PanelData);
        ((PanelData*)panels[panel_count - 1].data)->menu = ((PanelData*)panels[focussedPanelIndex].data)->menu;
        ((PanelData*)panels[panel_count - 1].data)->data = ((PanelData*)panels[focussedPanelIndex].data)->data;

        panels[panel_count - 1].callbacks = panels[focussedPanelIndex].callbacks;
        panels[panel_count - 1].numOfCallbacks = 4;

        PanelRunCallBack(panels[panel_count - 1], CALLBACK_START);

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

void DrawPanel(Panel *panel) {
    Rectangle rec = (Rectangle){*panel->origin.x, *panel->origin.y, (*panel->end.x - *panel->origin.x), (*panel->end.y - *panel->origin.y)};
    
    GuiPanel(rec, panelViews[((PanelData*)panel->data)->menu.active].name);
    GuiLabel(GetPanelAchorRectangle(*panel), GuiIconText(0x6B, ""));
    BeginScissorMode(rec.x, rec.y + 24.0f, rec.width, rec.height - 24.0f);

    PanelRunCallBack(panels[0], CALLBACK_DRAW);

    EndScissorMode();

    char list[panelViewCount * MAX_PANEL_NAME_LENGTH];
    list[0] = '\0';
    int i;
    for (i = 0; i < panelViewCount; i++) {
      strcat(list, panelViews[i].name);
      if (i < panelViewCount - 1) strcat(list, ";");
    }

    Rectangle menuRec = GetPanelRectangle(*panel);
    menuRec = (Rectangle){*panel->end.x - 200.0f, *panel->origin.y + 24, 200.0f, 100.0f};

    if(((PanelData*)panel->data)->menu.open) {
      int prevActive = ((PanelData*)panel->data)->menu.active;
      GuiListView(menuRec, list, &((PanelData*)panel->data)->menu.scrollIndex, &((PanelData*)panel->data)->menu.active);//, &((PanelData*)panel.data)->menu.focus);
      if (prevActive != ((PanelData*)panel->data)->menu.active) {
        PanelRunCallBack(*panel, CALLBACK_END);

        int currActive = ((PanelData*)panel->data)->menu.active;

        ((PanelData*)panel->data)->data = panelViews[currActive].data;
        panel->callbacks = panelViews[currActive].callbacks;
        panel->numOfCallbacks = 4;

        PanelRunCallBack(*panel, CALLBACK_START);
      }
    }

    if (IsPanelAnchorHovered(*panel) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        /*
        Panel View 1
        Panel View 2
        ...
        Panel View n
        */
        ((PanelData*)panel->data)->menu.open = true;
    } else if (IsPanelHovered(*panel) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ((PanelData*)panel->data)->menu.open = false;
    }
}

void OnDraw() {
  for (int i = 0; i < panel_count; i++) {
    DrawPanel(&panels[i]);
  }
}

void OnEnd() {
  for (int i = 0; i < panel_count; i++) {
    PanelRunCallBack(panels[i], CALLBACK_END);
  }
}

