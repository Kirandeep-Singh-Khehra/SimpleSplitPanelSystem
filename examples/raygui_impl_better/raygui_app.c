#include "raygui_impl.c"

void SimplePanelCallBackUpdate(Panel panel, void *data, ...) {
  // TODO: Update
}

void SimplePanelCallBackDraw1(Panel panel, void *data, ...) {
  Rectangle rec = GetPanelRectangle(panel);
  GuiButton((Rectangle){rec.x + 10, rec.y + 34, 200, 50}, "This is first View");
}

void SimplePanelCallBackDraw2(Panel panel, void *data, ...) {
  Rectangle rec = GetPanelRectangle(panel);
  GuiButton((Rectangle){rec.x + 10, rec.y + 34, 200, 50}, "This is second View");
}

PanelView panelViews[10] = {
  {
    .name = "Panel View 1",
    .data = NULL,
    .callbacks = (PanelCallback[]){NULL, SimplePanelCallBackUpdate, SimplePanelCallBackDraw1, NULL}
  },
  {
    .name = "Panel View 2",
    .data = NULL,
    .callbacks = (PanelCallback[]){NULL, SimplePanelCallBackUpdate, SimplePanelCallBackDraw2, NULL}
  }
};

int panelViewCount = 2;
