#ifndef DILAY_TOOL_SCULPT_BEHAVIOR
#define DILAY_TOOL_SCULPT_BEHAVIOR

#include <glm/fwd.hpp>
#include "macro.hpp"

class ConfigProxy;
class QDoubleSpinBox;
class SculptBrush;
class State;
class ViewCursor;
class ViewProperties;
class ViewToolTip;

class ToolSculptBehavior {
  public:
    DECLARE_BIG3_VIRTUAL (ToolSculptBehavior, ConfigProxy&)

    virtual SculptBrush&    brush      () const = 0;
            ViewCursor&     cursor     () const;
            QDoubleSpinBox& radiusEdit () const;

    void setupBrush      ();
    void setupProperties (ViewProperties&);
    void setupToolTip    (ViewToolTip&);
    bool mouseMoveEvent  (State&, const glm::ivec2&, bool);

  protected:
    ConfigProxy& config () const;

  private:
    IMPLEMENTATION

    virtual void runSetupBrush      () = 0;
    virtual void runSetupProperties (ViewProperties&) = 0;
    virtual void runSetupToolTip    (ViewToolTip&) = 0;
    virtual bool runMouseMoveEvent  (State&, const glm::ivec2&, bool) = 0;
};

#define DECLARE_TOOL_BEHAVIOR(name)                                               \
  class name : public ToolSculptBehavior {                                        \
    public:  DECLARE_BIG3 (name, ConfigProxy&)                                    \
             SculptBrush& brush () const;                                         \
    private: IMPLEMENTATION                                                       \
             void runSetupBrush      ();                                          \
             void runSetupProperties (ViewProperties&);                           \
             void runSetupToolTip    (ViewToolTip&);                              \
             bool runMouseMoveEvent  (State&, const glm::ivec2&, bool);           \
  };

#define DELEGATE_TOOL_BEHAVIOR(name)                                              \
  DELEGATE_BIG3_BASE (name, (ConfigProxy& c), (this), ToolSculptBehavior, (c))    \
  GETTER_CONST (SculptBrush&, name, brush)                                        \
  DELEGATE     (void, name, runSetupBrush)                                        \
  DELEGATE1    (void, name, runSetupProperties, ViewProperties&)                  \
  DELEGATE1    (void, name, runSetupToolTip, ViewToolTip&)                        \
  DELEGATE3    (bool, name, runMouseMoveEvent, State&, const glm::ivec2&, bool)

#endif
